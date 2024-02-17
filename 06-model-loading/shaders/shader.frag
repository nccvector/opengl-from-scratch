#version 330 core

in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

// uniform sampler2D colorTexture;

void main()
{
    //    FragColor = vec4(abs(Normal.x), abs(Normal.y), abs(Normal.z), 1);
    FragColor = vec4(abs(TexCoord.x), abs(TexCoord.y), 0, 1);
    //    FragColor = texture(colorTexture, uv);
}