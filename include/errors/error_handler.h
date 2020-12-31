#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class ErrorHandler {
public:
  static void ErrorCallback(GLenum source, GLenum type, GLuint id,
                            GLenum severity, GLsizei length,
                            const GLchar *message, const void *userParam);

private:
};
