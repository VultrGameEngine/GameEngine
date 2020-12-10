#pragma once
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
struct ShaderComponent
{
    unsigned int shader = 0;
    std::string shader_path;

    GLuint ViewProjection_location;
    GLuint ViewMatrix;
    GLuint ModelMatrix;
    GLuint LightID;
};