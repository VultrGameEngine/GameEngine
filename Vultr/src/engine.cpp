#include <game.hpp>
#include <vultr.hpp>
#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif
#include <game.hpp>
#include <engine.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <core/component_renderer.h>
#include <errors/error_handler.h>
#include <gui/framework/basic.h>
#include <gui/layouts/test_layout.h>
#include <helpers/path.h>
#include <core/models/update_tick.h>
#include <ImGuiFileDialog/ImGuiFileDialog.h>

namespace Vultr
{
    static void *load_dll(const std::string &path)
    {
#ifdef _WIN32
        return LoadLibrary(path.c_str());
#else
        return dlopen(path.c_str(), RTLD_NOW);
#endif
    }

    static void *get_function_pointer(void *dll, const std::string &name)
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

    World *get_current_world()
    {
        return engine_global().current_world;
    }

    void change_world(World *new_world)
    {
        auto &e = engine_global();
        World *old_world = e.current_world;
        e.current_world = new_world;
        if (old_world == nullptr)
            return;
        for (Entity entity : world_get_entity_manager(old_world).living_entites)
        {
            system_manager_entity_destroyed(e.system_manager, entity);
        }
        auto &entity_manager = world_get_entity_manager(new_world);
        auto &system_manager_world = world_get_system_manager(new_world);
        for (Entity entity : entity_manager.living_entites)
        {
            // Add the new entities to both the global systems and the new systems in the new world
            system_manager_entity_signature_changed(e.system_manager, entity, entity_manager.signatures[entity]);
            system_manager_entity_signature_changed(system_manager_world, entity, entity_manager.signatures[entity]);
        }
        destroy_world(old_world);
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
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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
        e.window = glfwCreateWindow(mode->width, mode->height, "VultrEditor", nullptr, nullptr);
#else
        e.window = glfwCreateWindow(mode->width, mode->height, "VultrEditor", debug ? nullptr : glfwGetPrimaryMonitor(), nullptr);
#endif

        if (e.window == nullptr)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
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
            io.Fonts->AddFontFromFileTTF(Path::GetFullPath("fonts/Roboto-Regular.ttf").c_str(), 30);
            ImGui::StyleColorsDark();
        }

        change_world(new_world(e.component_registry));
        engine_register_default_components(e);
        engine_init_default_systems(e);
    }

    void engine_load_game(Engine &e, const char *path)
    {
        void *DLL = load_dll(path);

        typedef void *(*GameInit_f)(void *);
        typedef void (*GameDestroy_f)(void *);
        GameInit_f init = (GameInit_f)get_function_pointer(DLL, "init");
        GameDestroy_f destroy = (GameDestroy_f)get_function_pointer(DLL, "flush");

        e.game = (Game *)init(&e);
    }

    void engine_load_game(Engine &e, Game *game)
    {
        e.game = game;
    }

    void engine_register_default_components(Engine &e)
    {
        register_component<StaticMeshComponent>();
        register_component<MaterialComponent>();
        register_component<TransformComponent>();
        register_component<LightComponent>();
        register_component<CameraComponent>();
        register_component<ControllerComponent>();
        register_component<SkyBoxComponent>();
    }

    void engine_init_default_systems(Engine &e)
    {
        MeshLoaderSystem::register_system();
        ShaderLoaderSystem::register_system();
        TextureLoaderSystem::register_system();
        ControllerSystem::register_system();
        CameraSystem::register_system();
        LightSystem::register_system();
        RenderSystem::register_system();
        RenderSystem::init();
        GUISystem::register_system();
        InputSystem::register_system();
        FontSystem::register_system();
        FontSystem::init();
        InputSystem::init(e.window);

        ControllerSystem::init(e.window);
        glfwSetWindowFocusCallback(e.window, ControllerSystem::window_focus_callback);

        glm::vec2 dimensions = RenderSystem::get_dimensions(GAME);
        GUISystem::init(TestLayout());
    }

    void engine_init_game(Engine &e)
    {
        assert(e.game != nullptr && "Game has not been loaded!");
        e.game->Init();
    }
    void engine_update_game(Engine &e, float &last_time, bool play)
    {
        e.should_close = glfwWindowShouldClose(e.window);

        float t = glfwGetTime();
        float deltaTime = t - last_time;
        last_time = t;
        UpdateTick tick = UpdateTick(deltaTime, e.debug);

        if (e.game != nullptr && play)
            e.game->Update(tick);

        // Only continuously update the meshes if we are planning on changing
        // these components at random (really will only happen in the editor) If
        // you need to change these components at runtime, destroy and then readd
        // the components
        if (e.debug)
        {
            ShaderLoaderSystem::update();
            TextureLoaderSystem::update();
            MeshLoaderSystem::update();
            ControllerSystem::update(tick.m_delta_time);
        }

        InputSystem::update(tick);
        RenderSystem::update(tick);
    }

    double engine_get_time_elapsed(Engine &e)
    {
        return glfwGetTime();
    }

    Signature entity_get_signature(Entity entity)
    {
        World *world = get_current_world();
        assert(world != nullptr && WORLD_DOESNT_EXIST_ERROR(entity_get_signature));
        return entity_manager_get_signature(world_get_entity_manager(world), entity);
    }
} // namespace Vultr

using namespace Vultr;

template <>
void RenderComponent<MaterialComponent>(Vultr::Entity entity)
{
    auto *_component = entity_get_component_unsafe<MaterialComponent>(entity);
    if (_component == nullptr)
        return;
    auto &component = *_component;
    if (ImGui::CollapsingHeader("MaterialComponent"))
    {
        const char *shader_options[] = {"Forward", "PBR", "Unlit", "Skybox", "Custom"};
        static int selected_shader_option = 0;

#define FORWARD_MATERIAL "shaders/forward_material.glsl"
#define SKYBOX_MATERIAL "shaders/skybox.glsl"
#define PBR_MATERIAL "shaders/forward_material.glsl"
#define UNLIT_MATERIAL "shaders/unlit.glsl"

        if (component.shader_source.GetPath() == FORWARD_MATERIAL)
        {
            selected_shader_option = 0;
        }
        else if (component.shader_source.GetPath() == PBR_MATERIAL)
        {
            selected_shader_option = 1;
        }
        else if (component.shader_source.GetPath() == UNLIT_MATERIAL)
        {
            selected_shader_option = 2;
        }
        else if (component.shader_source.GetPath() == SKYBOX_MATERIAL)
        {
            selected_shader_option = 3;
        }
        else
        {
            selected_shader_option = 4;
        }

        ImGui::PushID("Shader");
        const char *shader_label = shader_options[selected_shader_option];
        if (ImGui::BeginCombo("Shader", shader_label))
        {
            for (int n = 0; n < IM_ARRAYSIZE(shader_options); n++)
            {
                const bool is_selected = (selected_shader_option == n);
                if (ImGui::Selectable(shader_options[n], is_selected))
                {
                    selected_shader_option = n;
                    switch (selected_shader_option)
                    {
                    case 0: {
                        component = ForwardMaterial::Create("");
                        break;
                    }
                    case 1: {
                        component = ForwardMaterial::Create("");
                        break;
                    }
                    case 2: {
                        component = UnlitMaterial::Create();
                        break;
                    }

                    case 3: {
                        component = SkyboxMaterial::Create("", "", "", "", "", "");
                        break;
                    }
                    case 4: {
                        auto path = component.shader_source.GetPath();
                        if (path == FORWARD_MATERIAL || path == SKYBOX_MATERIAL || path == PBR_MATERIAL || path == UNLIT_MATERIAL)
                        {
                            component = MaterialComponent();
                            component.shader_source = File("", SHADER_FILE_EXTENSIONS);
                        }
                        break;
                    }
                    default: {
                        break;
                    }
                    }
                }

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        ImGui::PopID();

        // Different rendering for different shaders
        switch (selected_shader_option)
        {
            // Forward
        case 0: {
            ImGui::PushID("texture");
            RenderMember("Albedo", component.textures[0]);
            ImGui::PopID();

            ImGui::PushID("objectColor");
            RenderMember("Tint", component.colors["objectColor"]);
            ImGui::PopID();
        }
        // PBR
        case 1: {
            break;
        }
        // Unlit
        case 2: {
            ImGui::PushID("lightColor");
            RenderMember("Color", component.colors["lightColor"]);
            ImGui::PopID();
            break;
        }

        // Skybox
        case 3: {
            for (auto &pair : component.textures)
            {
                ImGui::PushID(pair.name.c_str());
                RenderMember(pair.name, pair.path);
                ImGui::PopID();
            }
            break;
        }
        default: {
            ImGui::PushID("shader_source");
            RenderMember("shader_source", component.shader_source);
            ImGui::PopID();

            ImGui::PushID("textures");
            if (ImGui::CollapsingHeader("textures"))
            {
                RenderMember("textures", component.textures);
            }
            ImGui::PopID();

            ImGui::PushID("vec3s");
            if (ImGui::CollapsingHeader("vec3s"))
            {
                RenderMember("vec3s", component.vec3s);
            }
            ImGui::PopID();

            ImGui::PushID("vec4s");
            if (ImGui::CollapsingHeader("vec4s"))
            {
                RenderMember("vec4s", component.vec4s);
            }
            ImGui::PopID();

            ImGui::PushID("colors");
            if (ImGui::CollapsingHeader("colors"))
            {
                RenderMember("colors", component.colors);
            }
            ImGui::PopID();

            ImGui::PushID("ints");
            if (ImGui::CollapsingHeader("ints"))
            {
                RenderMember("ints", component.ints);
            }
            ImGui::PopID();

            ImGui::PushID("floats");
            if (ImGui::CollapsingHeader("floats"))
            {
                RenderMember("floats", component.floats);
            }
            ImGui::PopID();
            break;
        }
        }

        if (ImGui::Button("Remove"))
        {
            entity_remove_component<MaterialComponent>(entity);
        }
    }
}
// VULTR_REGISTER_COMPONENT(TransformComponent, position, rotation, scale);
template <>
void RenderComponent<TransformComponent>(Entity entity)
{
    auto *component = entity_get_component_unsafe<TransformComponent>(entity);
    if (component == nullptr)
        return;
    if (ImGui::CollapsingHeader("TransformComponent"))
    {
        ImGui::PushID("TransformComponent");
        RenderMember("position", component->position);
        RenderMember("rotation", component->rotation);
        RenderMember("scale", component->scale);
        if (ImGui::Button("Remove"))
        {
            entity_remove_component<TransformComponent>(entity);
        }
        ImGui::PopID();
    }
}
template <>
const char *Vultr::get_struct_name<TransformComponent>()
{
    return "TransformComponent";
}

template <>
void RenderComponent<StaticMeshComponent>(Entity entity)
{
    auto *component = entity_get_component_unsafe<StaticMeshComponent>(entity);
    if (component == nullptr)
        return;
    if (ImGui::CollapsingHeader("StaticMeshComponent"))
    {
        ImGui::PushID("StaticMeshComponent");
        RenderMember("source", component->source);
        if (ImGui::Button("Remove"))
        {
            entity_remove_component<StaticMeshComponent>(entity);
        }
        ImGui::PopID();
    }
}
template <>
const char *Vultr::get_struct_name<StaticMeshComponent>()
{
    return "StaticMeshComponent";
}
template <>
void RenderComponent<SkyBoxComponent>(Entity entity)
{
    auto *component = entity_get_component_unsafe<SkyBoxComponent>(entity);
    if (component == nullptr)
        return;
    if (ImGui::CollapsingHeader("SkyBoxComponent"))
    {
        ImGui::PushID("SkyBoxComponent");
        RenderMember("identifier", component->identifier);
        if (ImGui::Button("Remove"))
        {
            entity_remove_component<StaticMeshComponent>(entity);
        }
        ImGui::PopID();
    }
}
template <>
const char *Vultr::get_struct_name<SkyBoxComponent>()
{
    return "SkyBoxComponent";
}
template <>
void RenderComponent<LightComponent>(Entity entity)
{
    auto *component = entity_get_component_unsafe<LightComponent>(entity);
    if (component == nullptr)
        return;
    if (ImGui::CollapsingHeader("LightComponent"))
    {
        ImGui::PushID("LightComponent");
        RenderMember("some_param", component->some_param);
        if (ImGui::Button("Remove"))
        {
            entity_remove_component<LightComponent>(entity);
        }
        ImGui::PopID();
    }
}
template <>
const char *Vultr::get_struct_name<LightComponent>()
{
    return "LightComponent";
}

template <>
void RenderComponent<ControllerComponent>(Entity entity)
{
    auto *component = entity_get_component_unsafe<ControllerComponent>(entity);
    if (component == nullptr)
        return;
    if (ImGui::CollapsingHeader("ControllerComponent"))
    {
        ImGui::PushID("ControllerComponent");
        RenderMember("sens", component->sens);
        if (ImGui::Button("Remove"))
        {
            entity_remove_component<ControllerComponent>(entity);
        }
        ImGui::PopID();
    }
}
template <>
const char *Vultr::get_struct_name<ControllerComponent>()
{
    return "ControllerComponent";
}
template <>
void RenderComponent<CameraComponent>(Entity entity)
{
    auto *component = entity_get_component_unsafe<CameraComponent>(entity);
    if (component == nullptr)
        return;
    if (ImGui::CollapsingHeader("CameraComponent"))
    {
        ImGui::PushID("CameraComponent");
        RenderMember("enabled", component->enabled);
        RenderMember("fov", component->fov);
        RenderMember("znear", component->znear);
        RenderMember("zfar", component->zfar);
        if (ImGui::Button("Remove"))
        {
            entity_remove_component<CameraComponent>(entity);
        }
        ImGui::PopID();
    }
}
template <>
const char *Vultr::get_struct_name<CameraComponent>()
{
    return "CameraComponent";
}
template <>
const char *Vultr::get_struct_name<MaterialComponent>()
{
    return ("MaterialComponent");
}
