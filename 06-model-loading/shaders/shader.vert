#version 330 core

layout (location = 0) in vec3 attribPosition;
layout (location = 1) in vec3 attribNormal;
layout (location = 2) in vec2 attribTexCoord;

out vec3 Normal;
out vec2 TexCoord;

uniform mat4 modelViewProjection;     // Model View Projection matrix

void main()
{
    gl_Position = modelViewProjection * vec4(attribPosition, 1.0f);

    Normal = attribNormal;
    TexCoord = attribTexCoord;
}