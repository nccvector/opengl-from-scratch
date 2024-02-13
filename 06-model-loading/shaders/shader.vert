#version 330 core

layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec2 aTexCoord;

out vec3 position;

uniform mat4 modelViewProjection;     // Model View Projection matrix

void main()
{
    position = aPos.xyz;

    gl_Position = vec4(aPos.xyz, 1.0);

//    gl_Position = modelViewProjection * vec4(aPos/10, 1.0f);
//    uv = aTexCoord;
}