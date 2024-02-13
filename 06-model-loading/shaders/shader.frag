#version 330 core

in vec3 position;
// in vec2 uv;

out vec4 FragColor;

// uniform sampler2D colorTexture;

void main()
{
    FragColor = vec4(1, 0, 0, 1);
//    FragColor = texture(colorTexture, uv);
}