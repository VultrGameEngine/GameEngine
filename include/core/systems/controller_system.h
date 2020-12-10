// Controller system handles input from the user translated to transform changes

#include "../../ecs/system/system.hpp"
#include "../components/transform_component.h"
#include "../components/controller_component.h"
#include "../../ecs/coordinator/coordinator.hpp"
#include <memory>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class ControllerSystem : public System
{
public:
    void Init(GLFWwindow *window, int width, int height);
    static std::shared_ptr<ControllerSystem> Get();
    void Update(float delta_time);
    void DestroyEntity(Entity entity) override {}
    static std::shared_ptr<ControllerSystem> RegisterSystem();

private:
    GLFWwindow *window;
    static std::shared_ptr<ControllerSystem> instance;
    static Signature signature;
    int width;
    int height;
};