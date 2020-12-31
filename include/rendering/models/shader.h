#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <stdio.h>
#include <string>

namespace Renderer {
class Shader {
public:
  Shader(GLuint id) { this->id = id; }
  ~Shader() {}

  // Bind the shader
  void Bind() { glUseProgram(id); }

  // Unbind the shader
  void Unbind() { glUseProgram(0); }

  void SetUniformMatrix4fv(std::string uniform, const float *value) {
    glUniformMatrix4fv(GetUniformLocation(uniform), 1, GL_FALSE, value);
  }

  void SetUniform3f(std::string uniform, glm::vec3 value) {
    glUniform3f(GetUniformLocation(uniform), value.x, value.y, value.z);
  }

  void SetUniform1i(std::string uniform, int value) {
    glUniform1i(GetUniformLocation(uniform), value);
  }

  unsigned int GetUniformLocation(std::string uniform) {
    return glGetUniformLocation(this->id, uniform.c_str());
  }
  unsigned int GetID() { return id; }

private:
  // ID of the shader
  GLuint id;
};
}; // namespace Renderer
