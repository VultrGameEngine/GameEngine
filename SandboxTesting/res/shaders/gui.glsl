#shader vertex
#version 330 core
#extension GL_ARB_separate_shader_objects: enable

layout(location = 0) in vec3 vposition;
layout(location = 1) in vec4 color;
layout(location = 2) in float texture; 

layout(location = 0) out vec4 fColor;
layout(location = 1) out vec2 uv;

void main()
{
    gl_Position = vec4(vposition, 1);
    uv = (vposition.xy + vec2(1, 1)) / 2.0;
    fColor = color;
}

#shader fragment
#version 330 core
#extension GL_ARB_separate_shader_objects: enable

layout(location = 0) in vec4 fColor;
layout(location = 1) in vec2 uv;

out vec4 FragColor;

void main()
{
    FragColor = vec4(1, 1, 1, 1);
}
