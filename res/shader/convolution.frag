in vec3 TexCoords;
out vec4 FragColor;

uniform samplerCube Skybox;
uniform vec2 Data;

#define PI 3.14159
#define SAMPLE_COUNT 256

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness);
vec2 Hammersley(uint i, uint N);
float DistributionGGX(vec3 N, vec3 H, float roughness);

void main()
{
    const vec3 normal = normalize(TexCoords);

    float weight = 0;
    vec3 sampleColor = vec3(0);

    for(int i = 0; i < SAMPLE_COUNT; i++)
    {
        const vec2 Xi = Hammersley(i, SAMPLE_COUNT);
        const vec3 H = ImportanceSampleGGX(Xi, normal, Data.x);
        const vec3 L = normalize(2.0 * dot(normal, H) * H - normal);

        const float nDotL = max(0, dot(normal, L));
        if(nDotL <= 0) continue;

        float D = DistributionGGX(normal, H, Data.x);
        float NdotH = max(dot(normal, H), 0.0);
        float pdf = D * NdotH / (4.0 * NdotH) + 0.0001;

        float saTexel  = 4.0 * PI / (6.0 * Data.y * Data.y);
        float saSample = 1.0 / (SAMPLE_COUNT * pdf + 0.0001);

        float mipLevel = Data.x == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel);

        sampleColor += textureLod(Skybox, L, mipLevel).rgb * nDotL;
        weight += nDotL;
    }

    FragColor = vec4(sampleColor / weight, 1);
}

float RadicalInverse_VdC(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.32830643653869633e-10; // / 0x100000000
}

vec2 Hammersley(uint i, uint N)
{
    return vec2(float(i)/float(N), RadicalInverse_VdC(i));
}

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
    float a = roughness * roughness;
    float phi = 2.0 * 3.14159 * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a * a - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;

    vec3 up = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent = normalize(cross(up, N));
    vec3 bitangent = cross(N, tangent);
    return tangent * H.x + bitangent * H.y + N * H.z;
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}