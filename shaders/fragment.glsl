#version 330 core

in vec3 fs_color;
in vec2 fs_texCoord;
uniform sampler2D Texture;

out vec4 out_color;

void main()
{
    out_color = texture(Texture, fs_texCoord);
}