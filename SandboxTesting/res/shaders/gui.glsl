#shader vertex
#version 410 core
#extension GL_ARB_separate_shader_objects: enable

layout(location = 0) in vec3 vposition;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 uv;
layout(location = 3) in float texture; 

out vec4 fColor;
out vec2 fTextCoord;
out float fTexture;


void main()
{
    gl_Position = vec4(vposition, 1);
    fTextCoord = uv;
    fColor = color;
    fTexture = texture;
}

#shader fragment
#version 410 core
#extension GL_ARB_separate_shader_objects: enable

out vec4 FragColor;

in vec4 fColor;
in vec2 fTextCoord;
in float fTexture;

uniform sampler2D bound_texture;

void main()
{
  if(fTexture > -1) {
    int index = int(fTexture);
    FragColor = vec4(1, 1, 1, texture(bound_texture, fTextCoord).r);
  } else {
    FragColor = fColor;
  }
}
