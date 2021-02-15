#include <Vultr.hpp>
#include <dlfcn.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

void *LoadDLL(const std::string &path)
{
    return dlopen(path.c_str(), RTLD_NOW);
}

void *GetFunctionPointer(void *dll, const std::string &name)
{
    return dlsym(dll, name.c_str());
}

using namespace Brick3D;
void Vultr::Init(bool debug)
{
    World::Init();
    World::RegisterComponent<StaticMeshComponent>();
    World::RegisterComponent<MaterialComponent>();
    World::RegisterComponent<TransformComponent>();
    World::RegisterComponent<LightComponent>();
    World::RegisterComponent<CameraComponent>();
    World::RegisterComponent<ControllerComponent>();
    World::RegisterComponent<SkyBoxComponent>();

    if (!glfwInit())
    {

        printf("Failed to initialize glfw\n");
        return;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

    this->window = glfwCreateWindow(1920, 1080, "VultrEditor", nullptr, nullptr);
    if (!window)
    {
        printf("Failed to initialize glfw window\n");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize glad\n");
        return;
    }

    this->debug = debug;

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEBUG_OUTPUT);
    if (debug)
    {
        glDebugMessageCallback(ErrorHandler::ErrorCallback, 0);
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable
    // Multi - Viewport /
    // Platform Windows
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
    ImGui::StyleColorsDark();

    ControllerSystem::Init(window);
    glfwSetWindowFocusCallback(window, ControllerSystem::WindowFocusCallback);

    MeshLoaderSystem::RegisterSystem();
    ShaderLoaderSystem::RegisterSystem();
    TextureLoaderSystem::RegisterSystem();
    ControllerSystem::RegisterSystem();
    CameraSystem::RegisterSystem();
    LightSystem::RegisterSystem();
    RenderSystem::RegisterSystem();
}

void Vultr::LoadGame(const std::string &path)
{
    void *DLL = LoadDLL(path);

    typedef Game *(*GameInit_f)(Vultr *);
    typedef void (*GameDestroy_f)(Game *);
    GameInit_f init = (GameInit_f)GetFunctionPointer(DLL, "init");
    GameDestroy_f destroy = (GameDestroy_f)GetFunctionPointer(DLL, "flush");

    game = init(this);
    game->Init();
}

void Vultr::UpdateGame(float &last_time)
{

    float t = glfwGetTime();
    float deltaTime = t - last_time;
    last_time = t;
    UpdateTick tick = UpdateTick(deltaTime, this->debug);

    game->Update(tick);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ControllerSystem::Update(tick.m_delta_time);
    RenderSystem::Update(tick);
    glfwPollEvents();
}

void Vultr::Destroy()
{
}
