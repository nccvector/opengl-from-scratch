#version 330 core

in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D TextureDiffuse;
uniform sampler2D TextureNormal;

void main()
{
    //    gl_FragColor = vec4(abs(TexCoord.x), abs(TexCoord.y), 0, 1);

    //    gl_FragColor = vec4(abs(Normal.x), abs(Normal.y), abs(Normal.z), 1);
    gl_FragColor = texture(TextureDiffuse, TexCoord);
}