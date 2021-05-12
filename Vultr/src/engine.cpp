#include <Vultr.hpp>
#ifdef WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif
#include <engine.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <core/component_renderer.h>
#include <gui/framework/basic.h>
#include <gui/layouts/test_layout.h>
#include <helpers/path.h>
#include <core/models/update_tick.h>

namespace Vultr
{
    static void *LoadDLL(const std::string &path)
    {
#ifdef _WIN32
        return LoadLibrary(path.c_str());
#else
        return dlopen(path.c_str(), RTLD_NOW);
#endif
    }

    static void *GetFunctionPointer(void *dll, const std::string &name)
    {
#ifdef _WIN32
        return GetProcAddress((HMODULE)dll, name.c_str());
#else
        return dlsym(dll, name.c_str());
#endif
    }

    Engine &engine_global()
    {
        static Engine engine;
        return engine;
    }

    void engine_init(Engine &e, bool debug)
    {
        if (!glfwInit())
        {

            printf("Failed to initialize glfw\n");
            return;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
#ifndef _WIN32
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);
#endif
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
        const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        if (debug)
        {
            glfwWindowHint(GLFW_RED_BITS, mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
        }
#ifdef _WIN32
        glfwWindowHint(GLFW_DECORATED, GL_FALSE);
        e.window = glfwCreateWindow(mode->width, mode->height, "VultrEditor",
                                    nullptr, nullptr);
#else
        e.window = glfwCreateWindow(mode->width, mode->height, "VultrEditor",
                                    glfwGetPrimaryMonitor(), nullptr);
#endif

        if (e.window == nullptr)
        {
            printf("Failed to initialize glfw window\n");
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(e.window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            printf("Failed to initialize glad\n");
            return;
        }

        e.debug = debug;

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(ErrorHandler::ErrorCallback, 0);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        if (debug)
        {
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO &io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            ImGui::StyleColorsDark();
            ImGui_ImplGlfw_InitForOpenGL(e.window, true);
            ImGui_ImplOpenGL3_Init("#version 410");
            io.Fonts->AddFontFromFileTTF(
                Path::GetFullPath("res/fonts/Roboto-Regular.ttf").c_str(), 30);
            ImGui::StyleColorsDark();
        }
    }

    void engine_load_game(Engine &e, const std::string &path)
    {
        void *DLL = LoadDLL(path);

        typedef Game *(*GameInit_f)(Engine *);
        typedef void (*GameDestroy_f)(Game *);
        GameInit_f init = (GameInit_f)GetFunctionPointer(DLL, "init");
        GameDestroy_f destroy = (GameDestroy_f)GetFunctionPointer(DLL, "flush");

        e.game = init(&e);
    }

    void engine_load_game(Engine &e, Game *game)
    {
        e.game = game;
    }

    void engine_register_default_components(Engine &e)
    {
        component_registry_register_component<StaticMeshComponent>(
            e.component_registry);
        component_registry_register_component<MaterialComponent>(
            e.component_registry);
        component_registry_register_component<TransformComponent>(
            e.component_registry);
        component_registry_register_component<LightComponent>(e.component_registry);
        component_registry_register_component<CameraComponent>(e.component_registry);
        component_registry_register_component<ControllerComponent>(
            e.component_registry);
        component_registry_register_component<SkyBoxComponent>(e.component_registry);
    }
    void engine_init_default_systems(Engine &e)
    {
        MeshLoaderSystem::RegisterSystem();
        ShaderLoaderSystem::RegisterSystem();
        TextureLoaderSystem::RegisterSystem();
        ControllerSystem::RegisterSystem();
        CameraSystem::RegisterSystem();
        LightSystem::RegisterSystem();
        RenderSystem::RegisterSystem();
        GUISystem::RegisterSystem();
        InputSystem::RegisterSystem();
        FontSystem::RegisterSystem();
        FontSystem::Init();
        InputSystem::Init(window);

        ControllerSystem::Init(window);
        glfwSetWindowFocusCallback(window, ControllerSystem::WindowFocusCallback);

        glm::vec2 dimensions = RenderSystemProvider::GetDimensions(GAME);
        GUISystem::Init(TestLayout());
    }

    void engine_init_game(Engine &e)
    {
        assert(e.game != nullptr && "Game has not been loaded!");
        e.game->Init();
    }
    void engine_update_game(Engine &e, float &last_time)
    {
        e.should_close = glfwWindowShouldClose(e.window);

        float t = glfwGetTime();
        float deltaTime = t - last_time;
        last_time = t;
        UpdateTick tick = UpdateTick(deltaTime, e.debug);

        if (e.game != nullptr)
            e.game->Update(tick);
        // Only continuously update the meshes if we are planning on changing these
        // components at random (really will only happen in the editor)
        // If you need to change these components at runtime, destroy and then readd
        // the components
        if (e.debug)
        {
            ShaderLoaderSystem::Update();
            TextureLoaderSystem::Update();
            MeshLoaderSystem::Update();
            ControllerSystem::Update(tick.m_delta_time);
        }
        InputSystem::Update(tick);
        RenderSystem::Update(tick);
    }
    double engine_get_time_elapsed(Engine &e)
    {
        return glfwGetTime();
    }
} // namespace Vultr
