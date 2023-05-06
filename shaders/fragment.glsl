#version 330 core

in vec3 fs_color;
in vec2 fs_texCoord;

// ================================================================================
// LIGHT VARS
struct PointLight
{
    float Intensity;
    vec3 Color;
    vec3 Position;
};

struct DirectionalLight
{
    float Intensity;
    vec3 Color;
    vec3 Direction;
};

struct SpotLight
{
    float Intensity;
    vec3 Color;
    vec3 Position;
    vec3 Direction;
    vec3 Angle;
};

struct SimplexAreaLight
{
    float Intensity;
    vec3 Color;
    vec3[3] Vertices;
};

const int MAX_POINT_LIGHTS = 1024;
const int MAX_DIRECTIONAL_LIGHTS = 1024;
const int MAX_SPOT_LIGHTS = 1024;
const int MAX_SIMPLEX_AREA_LIGHTS = 1024;

uniform int NumActiveAreaLights;
uniform int NumActiveDirectionalLights;
uniform int NumActiveSpotLights;
uniform int NumActiveSimplexAreaLights;

uniform PointLight[MAX_POINT_LIGHTS] PointLights;
uniform DirectionalLight[MAX_DIRECTIONAL_LIGHTS] DirectionalLights;
uniform SpotLight[MAX_SPOT_LIGHTS] SpotLights;
uniform SimplexAreaLight[MAX_SIMPLEX_AREA_LIGHTS] SimplexAreaLights;
// LIGHT VARS
// ================================================================================

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
    out_color *= vec4(PointLights[0].Color, 1.0f);
    // out_color = vec4(ColorAmbient + ColorDiffuse, 1);
}