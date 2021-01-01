#pragma once
#include "../../core/components/shader_cache.h"
#include "../../ecs/system/system.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <array>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string.h>
#include <vector>

struct ShaderProgramSource {
  std::string VertexSource;
  std::string FragmentSource;
};
static ShaderProgramSource ParseShader(const std::string &filepath) {
  // Opens file
  std::ifstream stream(filepath);

  enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

  std::string line;
  std::stringstream ss[2];
  ShaderType type = ShaderType::NONE;
  while (getline(stream, line)) {
    // Has found "#shader" on line
    if (line.find("#shader") != std::string::npos) {
      // Set mode to vertex
      if (line.find("vertex") != std::string::npos) {
        type = ShaderType::VERTEX;
      }
      // Set mode to fragment
      else if (line.find("fragment") != std::string::npos) {
        type = ShaderType::FRAGMENT;
      }
    }
    // For any other line of code
    else {
      // Add line to the appropriate string stream
      ss[(int)type] << line << '\n';
    }
  }
  return {ss[0].str(), ss[1].str()};
}

class ShaderLoaderSystem : public System {

public:
  static std::shared_ptr<ShaderLoaderSystem> Get();
  void InitShaders();
  static std::shared_ptr<ShaderLoaderSystem> RegisterSystem();
  static Shader *GetShader(std::string path);
  void OnDestroyEntity(Entity entity) override;
  void OnCreateEntity(Entity entity) override;

  static unsigned int CreateShader(const std::string &path) {
    // Parse the shader
    ShaderProgramSource source = ParseShader(path);
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, source.VertexSource);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, source.FragmentSource);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
  }

  static unsigned int CompileShader(unsigned int type,
                                    const std::string &source) {
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int res;
    glGetShaderiv(id, GL_COMPILE_STATUS, &res);

    if (res == GL_FALSE) {
      int length;
      glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
      char *message = new char[length];
      glGetShaderInfoLog(id, length, &length, message);
      std::cout << "Failed to compile "
                << (type == GL_VERTEX_SHADER ? "vertext" : "fragment")
                << " shader: " << message << std::endl;
      glDeleteShader(id);
      return 0;
    }

    return id;
  }

private:
  Signature signature;
  std::unordered_map<std::string, Shader *> loaded_shaders;
};
