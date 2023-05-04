#version 330 core

in vec3 fs_color;
in vec2 fs_texCoord;

uniform vec3 ColorAmbient;
uniform vec3 ColorDiffuse;
uniform vec3 ColorSpecular;
uniform sampler2D TextureAmbient;
uniform sampler2D TextureDiffuse;
uniform sampler2D TextureSpecular;

out vec4 out_color;

void main()
{
    out_color = texture(TextureDiffuse, fs_texCoord);
}