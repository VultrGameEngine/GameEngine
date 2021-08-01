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
#include <helpers/path.h>
#include <core/models/update_tick.h>
#include <ImGuiFileDialog/ImGuiFileDialog.h>
#include <core/models/event.h>

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

    static void close_dll(void *DLL)
    {
#ifdef _WIN32
#else
        dlclose(DLL);
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

    World *get_current_world(Engine *e)
    {
        return e->current_world;
    }

    void change_world(Engine *e, World *new_world)
    {
        World *old_world = e->current_world;
        e->current_world = new_world;

        // If there is an older scene, then we will flush it
        if (e->game != nullptr)
        {
            e->game->flush_scene(e);
        }

        if (old_world == nullptr)
            return;

        for (Entity entity : world_get_entity_manager(old_world).living_entites)
        {
            system_manager_entity_destroyed(e, e->system_manager, entity);
        }
        auto &entity_manager = world_get_entity_manager(new_world);
        auto &system_manager_world = world_get_system_manager(new_world);
        for (Entity entity : entity_manager.living_entites)
        {
            // Add the new entities to both the global systems and the new systems in the new world
            system_manager_entity_signature_changed(e, e->system_manager, entity, entity_manager.signatures[entity]);
            system_manager_entity_signature_changed(e, system_manager_world, entity, entity_manager.signatures[entity]);
        }

        destroy_world(old_world);
    }

    Engine *get_engine(GLFWwindow *window)
    {
        return static_cast<Engine *>(glfwGetWindowUserPointer(window));
    }

    void add_editor(Engine *e, void *editor)
    {
        e->editor = editor;
    }

    void destroy_entity(Engine *e, Entity entity)
    {
        auto *world = get_current_world(e);
        assert(world != nullptr && WORLD_DOESNT_EXIST_ERROR(destroy_entity));
        destroy_entity(e, world, entity);
        system_manager_entity_destroyed(e, e->system_manager, entity);
    }

    void engine_init(Engine *e, bool debug)
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
        glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
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
        e->window = glfwCreateWindow(mode->width, mode->height, "VultrEditor", nullptr, nullptr);
#else
        e->window = glfwCreateWindow(mode->width, mode->height, "VultrEditor", debug ? nullptr : glfwGetPrimaryMonitor(), nullptr);
#endif

        if (e->window == nullptr)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(e->window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            printf("Failed to initialize glad\n");
            return;
        }

        // Add our engine to the window, that way GLFW callbacks can have our engine
        glfwSetWindowUserPointer(e->window, static_cast<void *>(e));

        e->debug = debug;

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
            ImGui_ImplGlfw_InitForOpenGL(e->window, true);
            ImGui_ImplOpenGL3_Init("#version 410");
            ImGui::StyleColorsDark();
        }

        change_world(e, new_world(e->component_registry));
        engine_register_default_components(e);
        engine_init_default_systems(e);
    }

    void engine_load_game(Engine *e, const char *path)
    {
        void *DLL = load_dll(path);

        typedef void *(*GameInit_f)(void *);
        typedef void (*GameDestroy_f)(void *);
        GameInit_f init = (GameInit_f)get_function_pointer(DLL, "init");
        auto *error = dlerror();
        if (error != nullptr)
        {
            fprintf(stderr, "%s\n", error);
            exit(1);
        }
        GameDestroy_f destroy = (GameDestroy_f)get_function_pointer(DLL, "flush");
        error = dlerror();
        if (error != nullptr)
        {
            fprintf(stderr, "%s\n", error);
            exit(1);
        }

        e->game = static_cast<Game *>(init(e));
        e->dll = DLL;
        e->game->register_components(e);
    }

    void engine_load_game(Engine *e, Game *game)
    {
        e->game = game;
        e->game->register_components(e);
    }

    void engine_detach_game(Engine *e)
    {
    }

    void engine_flush_game(Engine *e)
    {
        // Deregister all components registered through game
        for (auto &type_name : e->component_registry.game_components)
        {
            auto type = e->component_registry.component_name_to_type[type_name];
            auto &component_manager = world_get_component_manager(e->current_world);
            component_manager_remove_component(component_manager, type);
        }
        component_registry_delete_game_components(e->component_registry);

        e->game->flush(e);
        delete e->game;
        e->game = nullptr;

        if (e->dll != nullptr)
        {
            dlclose(e->dll);
            e->dll = nullptr;
        }
    }

    template <typename T>
    void internal_register_component(Engine *e)
    {
        component_registry_register_component<T>(
            e->component_registry, [](Engine *e, Entity entity) { entity_add_component(e, entity, T::Create()); }, false);
    }

    void engine_register_default_components(Engine *e)
    {
        internal_register_component<StaticMeshComponent>(e);
        internal_register_component<MaterialComponent>(e);
        internal_register_component<TransformComponent>(e);
        internal_register_component<LightComponent>(e);
        internal_register_component<CameraComponent>(e);
        internal_register_component<ControllerComponent>(e);
        internal_register_component<SkyBoxComponent>(e);
    }

    void engine_init_default_systems(Engine *e)
    {
        MeshLoaderSystem::register_system(e);
        ShaderLoaderSystem::register_system(e);
        TextureLoaderSystem::register_system(e);
        ControllerSystem::register_system(e);
        CameraSystem::register_system(e);
        LightSystem::register_system(e);
        RenderSystem::register_system(e);
        RenderSystem::init(e);
        // GUISystem::register_system();
        InputSystem::register_system(e);
        FontSystem::register_system(e);
        FontSystem::init(e);
        InputSystem::init(e);

        ControllerSystem::init(e, e->window);
        glfwSetWindowFocusCallback(e->window, ControllerSystem::window_focus_callback);

        Vec2 dimensions = RenderSystem::get_dimensions(e, GAME);
    }

    void engine_init_game(Engine *e)
    {
        assert(e->game != nullptr && "Game has not been loaded!");
        e->game->perform_init_scene(e, e->current_world);
    }
    UpdateTick engine_update_game(Engine *e, float &last_time, bool play)
    {
        e->should_close = glfwWindowShouldClose(e->window);

        float t = glfwGetTime();
        float deltaTime = t - last_time;
        last_time = t;
        UpdateTick tick = UpdateTick(deltaTime, e->debug);

        if (e->game != nullptr && play)
            e->game->update(e, tick);

        // Only continuously update the meshes if we are planning on changing
        // these components at random (really will only happen in the editor) If
        // you need to change these components at runtime, destroy and then readd
        // the components
        if (e->debug)
        {
            ShaderLoaderSystem::update(e);
            TextureLoaderSystem::update(e);
            MeshLoaderSystem::update(e);
            ControllerSystem::update(e, tick.m_delta_time);
        }

        LightSystem::update(e);
        InputSystem::update(e, tick);
        RenderSystem::update(e, tick);
        return tick;
    }

    double engine_get_time_elapsed(Engine *e)
    {
        return glfwGetTime();
    }

    Signature entity_get_signature(Engine *e, Entity entity)
    {
        World *world = get_current_world(e);
        assert(world != nullptr && WORLD_DOESNT_EXIST_ERROR(entity_get_signature));
        return entity_manager_get_signature(world_get_entity_manager(world), entity);
    }

    void engine_send_update_event(Engine *e, EditEvent *event)
    {
        OnEdit on_edit = e->on_edit;
        if (on_edit != nullptr)
        {
            on_edit(e->editor, event);
        }
    }
    template <>
    void RenderComponent<MaterialComponent>(Engine *e, Vultr::Entity entity)
    {
        auto *_component = entity_get_component_unsafe<MaterialComponent>(e, entity);
        if (_component == nullptr)
            return;
        auto &component = *_component;
        static MaterialComponent copy;
        if (ImGui::CollapsingHeader("MaterialComponent"))
        {
            auto temp = component;
            RenderMemberResult res;
            const char *shader_options[] = {"Forward", "PBR", "Unlit", "Skybox", "Custom"};
            static int selected_shader_option = 0;

            if (component.shader_source == FORWARD_MATERIAL_SOURCE)
            {
                selected_shader_option = 0;
            }
            else if (component.shader_source == PBR_MATERIAL_SOURCE)
            {
                selected_shader_option = 1;
            }
            else if (component.shader_source == UNLIT_MATERIAL_SOURCE)
            {
                selected_shader_option = 2;
            }
            else if (component.shader_source == SKYBOX_MATERIAL_SOURCE)
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
                            case 0:
                            {
                                component = ForwardMaterial::Create("", "");
                                break;
                            }
                            case 1:
                            {
                                component = ForwardMaterial::Create("", "");
                                break;
                            }
                            case 2:
                            {
                                component = UnlitMaterial::Create(Color(255));
                                break;
                            }

                            case 3:
                            {
                                component = SkyboxMaterial::Create("", "", "", "", "", "");
                                break;
                            }
                            case 4:
                            {
                                auto source = component.shader_source;
                                if (source == FORWARD_MATERIAL_SOURCE || source == SKYBOX_MATERIAL_SOURCE || source == PBR_MATERIAL_SOURCE || source == UNLIT_MATERIAL_SOURCE)
                                {
                                    component = MaterialComponent();
                                    component.shader_source = ShaderSource("");
                                }
                                break;
                            }
                            default:
                            {
                                break;
                            }
                        }
                    }

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                res = was_edited();
                ImGui::EndCombo();
            }
            ImGui::PopID();

            for (u16 i = 0; i < component.texture_count; i++)
            {
                auto &texture = component.textures[i];
                ImGui::PushID(texture.location.c_str());
                res = RenderMember(texture.location, texture.file);
                ImGui::PopID();
            }

            for (u16 i = 0; i < component.uniform_count; i++)
            {
                auto &uniform = component.uniforms[i];
                ImGui::PushID(uniform.location.c_str());
                res = RenderMember(uniform.location, uniform);
                ImGui::PopID();
            }

            if (res.started_editing)
            {
                copy = temp;
                std::cout << "Started editing material component\n";
            }
            if (res.finished_editing)
            {
                auto *event = new ComponentEditEvent<MaterialComponent>();
                event->before = copy;
                event->after = component;
                event->entities = {entity};
                event->type = get_component_type<MaterialComponent>(e);
                engine_send_update_event(e, event);
                std::cout << "Finished editing material component\n";
            }

            if (ImGui::Button("Remove"))
            {
                entity_remove_component<MaterialComponent>(e, entity);
            }
        }
    }

    template <>
    void RenderComponent<TransformComponent>(Engine *e, Entity entity)
    {
        auto *component = entity_get_component_unsafe<TransformComponent>(e, entity);
        if (component == nullptr)
            return;
        static TransformComponent copy;
        auto temp = TransformComponent(*component);
        RenderMemberResult res;
        if (ImGui::CollapsingHeader("TransformComponent"))
        {
            ImGui::PushID("TransformComponent");
            res = RenderMember("position", component->position);
            res = RenderMember("rotation", component->rotation);
            res = RenderMember("scale", component->scale);
            if (res.started_editing)
            {
                copy = temp;
            }
            if (res.finished_editing)
            {
                auto *event = new ComponentEditEvent<TransformComponent>();
                event->before = copy;
                event->after = *component;
                event->entities = {entity};
                event->type = get_component_type<TransformComponent>(e);
                engine_send_update_event(e, event);
            }

            if (ImGui::Button("Remove"))
            {
                entity_remove_component<TransformComponent>(e, entity);
            }
            ImGui::PopID();
        }
    }
    template <>
    const char *get_struct_name<TransformComponent>()
    {
        return "TransformComponent";
    }

    template <>
    void RenderComponent<StaticMeshComponent>(Engine *e, Entity entity)
    {
        auto *component = entity_get_component_unsafe<StaticMeshComponent>(e, entity);
        if (component == nullptr)
            return;

        static StaticMeshComponent copy;
        if (ImGui::CollapsingHeader("StaticMeshComponent"))
        {
            auto temp = *component;
            RenderMemberResult res;
            ImGui::PushID("StaticMeshComponent");
            res = RenderMember("source", component->source);
            if (res.started_editing)
            {
                copy = temp;
            }
            if (res.finished_editing)
            {
                auto *event = new ComponentEditEvent<StaticMeshComponent>();
                event->before = copy;
                event->after = *component;
                event->entities = {entity};
                event->type = get_component_type<StaticMeshComponent>(e);
                engine_send_update_event(e, event);
            }
            if (ImGui::Button("Remove"))
            {
                entity_remove_component<StaticMeshComponent>(e, entity);
            }
            ImGui::PopID();
        }
    }
    template <>
    const char *get_struct_name<StaticMeshComponent>()
    {
        return "StaticMeshComponent";
    }
    template <>
    void RenderComponent<SkyBoxComponent>(Engine *e, Entity entity)
    {
        auto *component = entity_get_component_unsafe<SkyBoxComponent>(e, entity);
        if (component == nullptr)
            return;
        static SkyBoxComponent copy;
        if (ImGui::CollapsingHeader("SkyBoxComponent"))
        {
            RenderMemberResult res;
            auto temp = *component;
            ImGui::PushID("SkyBoxComponent");
            res = RenderMember("identifier", component->identifier);
            if (res.started_editing)
            {
                copy = temp;
            }
            if (res.finished_editing)
            {
                auto *event = new ComponentEditEvent<SkyBoxComponent>();
                event->before = copy;
                event->after = *component;
                event->entities = {entity};
                event->type = get_component_type<SkyBoxComponent>(e);
                engine_send_update_event(e, event);
            }
            if (ImGui::Button("Remove"))
            {
                entity_remove_component<StaticMeshComponent>(e, entity);
            }
            ImGui::PopID();
        }
    }
    template <>
    const char *get_struct_name<SkyBoxComponent>()
    {
        return "SkyBoxComponent";
    }
    template <>
    void RenderComponent<LightComponent>(Engine *e, Entity entity)
    {
        auto *component = entity_get_component_unsafe<LightComponent>(e, entity);
        if (component == nullptr)
            return;
        static LightComponent copy;
        if (ImGui::CollapsingHeader("LightComponent"))
        {
            RenderMemberResult res;
            auto temp = *component;
            ImGui::PushID("LightComponent");

            const char *light_options[] = {"Directional", "Point", "Spot"};
            static s8 selected_light_option = 0;

            if (component->type == LightComponent::DirectionalLight)
            {
                selected_light_option = 0;
            }
            else if (component->type == LightComponent::PointLight)
            {
                selected_light_option = 1;
            }
            else if (component->type == LightComponent::SpotLight)
            {
                selected_light_option = 2;
            }

            const char *light_type_label = light_options[selected_light_option];
            if (ImGui::BeginCombo("Type", light_type_label))
            {
                for (int n = 0; n < IM_ARRAYSIZE(light_options); n++)
                {
                    const bool is_selected = (selected_light_option == n);
                    if (ImGui::Selectable(light_options[n], is_selected))
                    {
                        selected_light_option = n;
                        switch (selected_light_option)
                        {
                            case 0:
                            {
                                component->type = LightComponent::DirectionalLight;
                                break;
                            }
                            case 1:
                            {
                                component->type = LightComponent::PointLight;
                                break;
                            }
                            case 2:
                            {
                                component->type = LightComponent::SpotLight;
                                break;
                            }
                            default:
                            {
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

            ImGui::PushID("ambient");
            res = RenderMember("Ambient", component->ambient);
            ImGui::PopID();

            ImGui::PushID("diffuse");
            res = RenderMember("Diffuse", component->diffuse);
            ImGui::PopID();

            ImGui::PushID("intensity");
            res = RenderMember("Intensity", component->intensity);
            ImGui::PopID();

            ImGui::PushID("specular");
            res = RenderMember("Specular", component->specular);
            ImGui::PopID();

            switch (selected_light_option)
            {
                case 0:
                {
                    break;
                }
                case 1:
                {
                    ImGui::PushID("constant");
                    res = RenderMember("Constant", component->constant);
                    ImGui::PopID();
                    ImGui::PushID("linear");
                    res = RenderMember("Linear", component->linear);
                    ImGui::PopID();
                    ImGui::PushID("quadratic");
                    res = RenderMember("Quadratic", component->quadratic);
                    ImGui::PopID();
                    break;
                }
                case 2:
                {
                    break;
                }
                default:
                {
                    break;
                }
            }
            if (res.started_editing)
            {
                copy = temp;
            }
            if (res.finished_editing)
            {
                auto *event = new ComponentEditEvent<LightComponent>();
                event->before = copy;
                event->after = *component;
                event->entities = {entity};
                event->type = get_component_type<LightComponent>(e);
                engine_send_update_event(e, event);
            }

            if (ImGui::Button("Remove"))
            {
                entity_remove_component<LightComponent>(e, entity);
            }
            ImGui::PopID();
        }
    }
    template <>
    const char *get_struct_name<LightComponent>()
    {
        return "LightComponent";
    }

    template <>
    void RenderComponent<ControllerComponent>(Engine *e, Entity entity)
    {
        auto *component = entity_get_component_unsafe<ControllerComponent>(e, entity);
        if (component == nullptr)
            return;
        static ControllerComponent copy;
        if (ImGui::CollapsingHeader("ControllerComponent"))
        {
            RenderMemberResult res;
            auto temp = *component;
            ImGui::PushID("ControllerComponent");
            res = RenderMember("sens", component->sens);
            if (res.started_editing)
            {
                copy = temp;
            }
            if (res.finished_editing)
            {
                auto *event = new ComponentEditEvent<ControllerComponent>();
                event->before = copy;
                event->after = *component;
                event->entities = {entity};
                event->type = get_component_type<ControllerComponent>(e);
                engine_send_update_event(e, event);
            }
            if (ImGui::Button("Remove"))
            {
                entity_remove_component<ControllerComponent>(e, entity);
            }
            ImGui::PopID();
        }
    }
    template <>
    const char *get_struct_name<ControllerComponent>()
    {
        return "ControllerComponent";
    }
    template <>
    void RenderComponent<CameraComponent>(Engine *e, Entity entity)
    {
        auto *component = entity_get_component_unsafe<CameraComponent>(e, entity);
        if (component == nullptr)
            return;
        static CameraComponent copy;
        if (ImGui::CollapsingHeader("CameraComponent"))
        {
            RenderMemberResult res;
            auto temp = *component;
            ImGui::PushID("CameraComponent");
            res = RenderMember("enabled", component->enabled);
            res = RenderMember("fov", component->fov);
            res = RenderMember("znear", component->znear);
            res = RenderMember("zfar", component->zfar);
            res = RenderMember("exposure", component->exposure);
            res = RenderMember("bloom_intensity", component->bloom_intensity);
            res = RenderMember("bloom_quality", component->bloom_quality);
            res = RenderMember("gamma_correction", component->gamma_correction);
            if (res.started_editing)
            {
                copy = temp;
            }
            if (res.finished_editing)
            {
                auto *event = new ComponentEditEvent<CameraComponent>();
                event->before = copy;
                event->after = *component;
                event->entities = {entity};
                event->type = get_component_type<CameraComponent>(e);
                engine_send_update_event(e, event);
            }
            if (ImGui::Button("Remove"))
            {
                entity_remove_component<CameraComponent>(e, entity);
            }
            ImGui::PopID();
        }
    }
    template <>
    const char *get_struct_name<CameraComponent>()
    {
        return "CameraComponent";
    }
    template <>
    const char *get_struct_name<MaterialComponent>()
    {
        return "MaterialComponent";
    }

} // namespace Vultr
