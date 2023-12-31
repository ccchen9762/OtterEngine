#include "constants.hlsli"

cbuffer camera : register(b1) {
    float4 cameraPosition;
}

cbuffer LightDirectional : register(b2) {
    int totalDir;
    int3 paddingDir;
    float4 lightDirectionsDir[MAX_LIGHTS];
    float4 lightColorsDir[MAX_LIGHTS];
    float4 ambientDir;
    float intensityDir;
    float attenuationCDir;
    float attenuationLDir;
    float attenuationQDir;
};

cbuffer LightPoint : register(b3) {
    int totalPoint;
    int3 paddingPoint;
    float4 lightPositionsPoint[MAX_LIGHTS];
    float4 lightColorsPoint[MAX_LIGHTS];
    float4 ambientPoint;
    float intensityPoint;
    float attenuationCPoint;
    float attenuationLPoint;
    float attenuationQPoint;
};

cbuffer attributes : register(b4) {
    float shiness;
    bool hasSpecularMap;
    bool hasNormalMap;
    float padding;
}

struct Interpolant {
    float4 position : SV_Position;
    float4 worldPosition : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 tangent : NORMAL1;
    float3 bitangent : NORMAL2;
};

struct Pixel {
    float4 color : SV_Target;
};

Texture2D texDiffuse : register(t0);
SamplerState samDiffuse : register(s0);

Texture2D texSpecular : register(t1);
SamplerState samSpecular : register(s1);

Texture2D texNormal : register(t2);
SamplerState samNormal : register(s2);

float3 CalculateLight(int i, Interpolant input, float3 lightUnitVector, float4 diffuseSample, float3 lightColor) {
    // diffuse
    // k_d * l_d * (l dot n)
    const float3 diffuse = 2*diffuseSample.rgb * lightColor * max(0.0f, dot(lightUnitVector, input.normal));
        
    // specular
    // 2 * l dot n * n - l
    // k_s * l_s * (r dot v)^shiness
    const float3 reflection = 2 * dot(lightUnitVector, input.normal) * input.normal - lightUnitVector;
    const float4 specularSample = texSpecular.Sample(samSpecular, input.texcoord);
    const float sampleShiness = pow(2, specularSample.a * 4.0f);
    const float3 specular = float3(specularSample.r, specularSample.r, specularSample.r) * lightColor *
        pow(max(0.0f, dot(normalize(cameraPosition - input.worldPosition), reflection)), sampleShiness);
    
    return diffuse + specular;
}

Pixel main(Interpolant input, bool isFrontFace: SV_IsFrontFace) {
    
    Pixel output;
    
    const float4 diffuseSample = texDiffuse.Sample(samDiffuse, input.texcoord);
    clip(diffuseSample.a < 0.01f ? -1 : 1); // discard if transparent
    
    if (hasNormalMap) {
        const float3x3 tangentMatrix = {
            input.tangent,
            input.bitangent,
            input.normal
        };
        
        const float4 normalSample = texNormal.Sample(samNormal, input.texcoord);
        input.normal.x = (normalSample.x * 2.0f - 1.0f);
        input.normal.y = -(normalSample.y * 2.0f - 1.0f);
        input.normal.z = (normalSample.z * 2.0f - 1.0f);
        
        input.normal = mul(input.normal, tangentMatrix);
    }
    input.normal = normalize(input.normal);
    
    if (!isFrontFace) {
        input.normal *= -1;
    }
    
    // directional lights
    output.color.rgb = float3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < totalDir; i++) {
        const float3 lightUnitVector = normalize(-lightDirectionsDir[i]);
        output.color.rgb += CalculateLight(i, input, lightUnitVector, diffuseSample, lightColorsDir[i].rgb);
    }
    const float3 dirLights = intensityDir * (output.color.rgb + ambientDir.rgb * diffuseSample.rgb);
    
    // point lights
    output.color.rgb = float3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < totalPoint; i++) {
        const float3 lightVector = (lightPositionsPoint[i] - input.worldPosition).xyz;
        const float distance = length(lightVector);
        const float3 lightUnitVector = lightVector / distance;
        const float attenuation = 1.0f / (attenuationCPoint + attenuationLPoint * distance + attenuationQPoint * (distance * distance));
        output.color.rgb += CalculateLight(i, input, lightUnitVector, diffuseSample, lightColorsPoint[i].rgb) * attenuation;
    }
    const float3 pointLights = intensityPoint * (output.color.rgb + ambientPoint.rgb * diffuseSample.rgb);
   
    output.color = saturate(float4(dirLights + pointLights, 1.0f));
        
    return output;
}