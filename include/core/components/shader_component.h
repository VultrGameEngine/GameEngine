#pragma once
#include "../../rendering/render_type.h"
#include "../systems/shader_loader_system.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
struct ShaderComponent {
  std::string path;
  Renderer::RenderType type = Renderer::Deferred;

  Shader *GetShader() { return ShaderLoaderSystem::GetShader(path); }
};
