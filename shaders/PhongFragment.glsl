#version 330 core

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

// ================================================================================
// TEXTURE AND VEC3 VARS
const int INDEX_AMBIENT = 0;
const int INDEX_DIFFUSE = 1;
const int INDEX_SPECULAR = 2;
const int MAX_INDICES = 8;

uniform sampler2D[MAX_INDICES] TextureSampler2DArray;
uniform bool[MAX_INDICES] TextureActiveStatusArray;
uniform vec3[MAX_INDICES] TextureVec3FloatArray;
uniform float[MAX_INDICES] TextureFloatArray;
// TEXTURE VARS
// ================================================================================

// ================================================================================
// SHADER PIPELINE INPUTS AND OUTPUTS
in vec3 fs_position;
in vec3 fs_normal;
in vec2 fs_texCoord;

out vec4 out_color;
// INPUTS AND OUTPUTS
// ================================================================================

void main()
{
    vec3 ambientColor = TextureVec3FloatArray[INDEX_AMBIENT].xyz;
    if (TextureActiveStatusArray[INDEX_AMBIENT]) {
        ambientColor = texture(TextureSampler2DArray[INDEX_AMBIENT], fs_texCoord).xyz;
    }

    vec3 diffuseColor = TextureVec3FloatArray[INDEX_DIFFUSE].xyz;
    if (TextureActiveStatusArray[INDEX_DIFFUSE]) {
        diffuseColor = texture(TextureSampler2DArray[INDEX_DIFFUSE], fs_texCoord).xyz;
    }

    vec3 specularColor = TextureVec3FloatArray[INDEX_SPECULAR].xyz;
    if (TextureActiveStatusArray[INDEX_SPECULAR]) {
        specularColor = texture(TextureSampler2DArray[INDEX_SPECULAR], fs_texCoord).xyz;
    }

    float Ka = 1.0f;   // Ambient reflection coefficient
    float Kd = 1.0f;   // Diffuse reflection coefficient
    float Ks = 1.0f;   // Specular reflection coefficient
    float shininessVal = 33.0f; // Shininess

    vec3 N = normalize(fs_normal);
    vec3 L = normalize(PointLights[0].Position - fs_position);
    float lambertian = max(dot(N, L), 0.0f);
    float specular = 0.0f;
    if (lambertian > 0.0f) {
        vec3 R = reflect(-L, N);      // Reflected light vector
        vec3 V = normalize(-fs_position); // Vector to viewer
        // Compute the specular term
        float specAngle = max(dot(R, V), 0.0f);
        specular = pow(specAngle, shininessVal);
    }

    vec4 color = vec4(Ka * ambientColor +
    Kd * lambertian * diffuseColor +
    Ks * specular * specularColor, 1.0);

    out_color = color;
}