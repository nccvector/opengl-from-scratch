#version 330 core

in vec3 Normal;
in vec2 TexCoord;

uniform float time;
uniform sampler2D TextureDiffuse;
uniform sampler2D TextureNormal;

void main()
{
//        gl_FragColor = vec4(abs(TexCoord.x), abs(TexCoord.y), 0, 1);

        float slowTime = 0.5f * sin(time / 100.0f) + 0.5f;

        vec3 color = abs(Normal);
        vec3 alterColor = 1 - color;
        vec3 finalColor = color * slowTime + alterColor * (1 - slowTime);

        gl_FragColor = vec4(finalColor, 1);

//    vec4 texColor = texture(TextureDiffuse, TexCoord);
//
//    if (texColor.a < 0.1) {
//        discard;
//    }
//
//    gl_FragColor = texColor;
}