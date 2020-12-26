#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
struct ShaderComponent {
  unsigned int shader = 0;
  std::string shader_path;

  inline GLuint GetUniform(std::string name) {
    return glGetUniformLocation(this->shader, name.c_str());
  }
};
