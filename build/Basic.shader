#shader vertex
#version 330 core
#extension GL_ARB_separate_shader_objects: enable

// Takes in position as a vec4 to pipe into the vertex positions
layout(location = 0) in vec4 vposition;
layout(location = 1) in vec4 vcolor;

void main()
{
    fcolor = vcolor
    gl_Position = vposition;
}

#shader fragment
#version 330 core
#extension GL_ARB_separate_shader_objects: enable

// Sets an output color as a vec4
layout(location = 0) out vec4 color;

void main()
{
    //Sets color to blue
    color = vec4(0.2, 0.3, 0.8, 1.0);
}