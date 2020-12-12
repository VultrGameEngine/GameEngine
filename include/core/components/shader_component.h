#pragma once
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
struct ShaderComponent
{
    unsigned int shader = 0;
    std::string shader_path;

    GLuint Model;
    GLuint View;
    GLuint Projection;
    GLuint LightPosition;
    GLuint ViewPosition;
    GLuint ObjectColor;
    GLuint LightColor;
    GLuint TextureSampler;
};