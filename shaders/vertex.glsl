#version 330 core
layout (location = 0) in vec3 vs_position;
layout (location = 1) in vec3 vs_vertexColor;
layout (location = 2) in vec2 vs_texCoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 fs_color;
out vec2 fs_texCoord;

void main()
{
    gl_Position = Projection * View * Model * vec4(vs_position, 1.0);

    fs_color = vs_texCoord.xyx;
    fs_texCoord = vs_texCoord;
}