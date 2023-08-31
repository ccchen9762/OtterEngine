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
    float2 padding;
}

struct Interpolant {
    float4 position : SV_Position;
    float4 worldPosition : POSITION0;
    float4 color : COLOR0;
    float3 normal : NORMAL0;
};

struct Pixel {
    float4 color : SV_Target;
};

float3 CalculateDirectional(int i, Interpolant input) {
    const float3 lightUnitVector = normalize(-lightDirectionsDir[i]);
        
    // diffuse
    const float3 diffuse = input.color.rgb * lightColorsDir[i].rgb * max(0.0f, dot(lightUnitVector, input.normal));
    
    // specular
    const float3 reflection = 2 * dot(lightUnitVector, input.normal) * input.normal - lightUnitVector;
    const float3 specular = lightColorsDir[i].rgb * pow(max(0.0f, dot(normalize(cameraPosition - input.worldPosition), reflection)), shiness);
    
    return diffuse + specular;
}

float3 CalculatePoint(int i, Interpolant input) {
    const float3 lightVector = (lightPositionsPoint[i] - input.worldPosition).xyz;
    const float distance = length(lightVector);
    const float3 lightUnitVector = lightVector / distance;
    
    // diffuse
    const float3 diffuse = input.color.rgb * lightColorsPoint[i].rgb * max(0.0f, dot(lightUnitVector, input.normal));
    
    // specular
    const float3 reflection = 2 * dot(lightUnitVector, input.normal) * input.normal - lightUnitVector;
    const float3 specular = lightColorsPoint[i].rgb * pow(max(0.0f, dot(normalize(cameraPosition - input.worldPosition), reflection)), shiness);
    
    const float attenuation = 1.0f / (attenuationCPoint + attenuationLPoint * distance + attenuationQPoint * (distance * distance));
    
    return (diffuse + specular) * attenuation;
}

Pixel main(Interpolant input) {
    
    Pixel output;
        
    output.color.rgb = float3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < totalDir; i++)
        output.color.rgb += CalculateDirectional(i, input);
    const float3 dirLights = intensityDir * (output.color.rgb + ambientDir.rgb * input.color);
    
    output.color.rgb = float3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < totalPoint; i++)
        output.color.rgb += CalculatePoint(i, input);
    const float3 pointLights = intensityPoint * (output.color.rgb + ambientPoint.rgb * input.color);
    
    output.color = saturate(float4(dirLights + pointLights, 1.0f));
    
    return output;
}