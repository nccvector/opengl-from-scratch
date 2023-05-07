#version 330 core
layout (location = 0) in vec3 vs_position;
layout (location = 1) in vec3 vs_normal;
layout (location = 2) in vec2 vs_texCoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 fs_position;
out vec3 fs_normal;
out vec2 fs_texCoord;

void main()
{
    vec4 position = Model * vec4(vs_position, 1.0);
    vec4 normal = Model * vec4(vs_normal, 0.0);

    // final gl_Position
    gl_Position = Projection * View * position;

    // fragment shader inputs
    fs_position = position.xyz;
    fs_normal = normal.xyz;
    fs_texCoord = vs_texCoord;
}