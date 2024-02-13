#version 330 core

layout (location = 0) in vec4 aPos;
// layout (location = 1) in vec2 aTexCoord;

out vec3 position;

uniform mat4 transform;

void main()
{
    position = aPos.xyz;
    gl_Position = aPos;
//    uv = aTexCoord;
}