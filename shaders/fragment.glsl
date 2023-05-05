#version 330 core

in vec3 fs_color;
in vec2 fs_texCoord;

uniform vec3 ColorAmbient;
uniform vec3 ColorDiffuse;
uniform vec3 ColorSpecular;

// Texture Units
uniform sampler2D Texture0; // Ambient
uniform sampler2D Texture1; // Diffuse
uniform sampler2D Texture2; // Specular

out vec4 out_color;

void main()
{
    out_color = texture(Texture0, fs_texCoord);
    // out_color = vec4(ColorAmbient + ColorDiffuse, 1);
}