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
    float2 padding;
}

struct Interpolant {
	float4 position	: SV_Position;
    float4 worldPosition : POSITION0;
	float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

struct Pixel {
	float4 color	: SV_Target;
};

Texture2D texDiffuse	: register(t0);
SamplerState samDiffuse	: register(s0);

Texture2D texSpecular    : register(t1);
SamplerState samSpecular : register(s1);

Texture2D texNormal : register(t2);
SamplerState samNormal : register(s2);

float3 CalculateLight(int i, Interpolant input, float3 lightUnitVector, float4 diffuseSample) {
    // diffuse
    // k_d * l_d * (l dot n)
    const float3 diffuse = diffuseSample.rgb * lightColorsPoint[i].rgb * max(0.0f, dot(lightUnitVector, input.normal));
        
    // specular
    // 2 * l dot n * n - l
    // k_s * l_s * (r dot v)^shiness
    const float3 reflection = 2 * dot(lightUnitVector, input.normal) * input.normal - lightUnitVector;
    const float3 specular = lightColorsPoint[i].rgb * pow(max(0.0f, dot(normalize(cameraPosition - input.worldPosition), reflection)), shiness);
        
    return diffuse + specular;
}

Pixel main(Interpolant input){
    
	Pixel output;
    
    if (hasNormalMap) {
        const float4 normalSample = texNormal.Sample(samNormal, input.texcoord);
        input.normal.x = (normalSample.x * 2.0f - 1.0f);
        input.normal.y = (-normalSample.y * 2.0f + 1.0f);
        
        input.normal = normalize(input.normal);
    }
    
    const float4 diffuseSample = texDiffuse.Sample(samDiffuse, input.texcoord);
    
    // directional lights
    output.color.rgb = float3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < totalDir; i++) {
        const float3 lightUnitVector = normalize(-lightDirectionsDir[i]);
        output.color.rgb += CalculateLight(i, input, lightUnitVector, diffuseSample);
    }
    const float3 dirLights = intensityDir * (output.color.rgb + ambientDir.rgb * diffuseSample.rgb);
    
    // point lights
    output.color.rgb = float3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < totalPoint; i++) {
        const float3 lightVector = (lightPositionsPoint[i] - input.worldPosition).xyz;
        const float distance = length(lightVector);
        const float3 lightUnitVector = lightVector / distance;
        const float attenuation = 1.0f / (attenuationCPoint + attenuationLPoint * distance + attenuationQPoint * (distance * distance));
        output.color.rgb += CalculateLight(i, input, lightUnitVector, diffuseSample) * attenuation;
    }
    const float3 pointLights = intensityPoint * (output.color.rgb + ambientPoint.rgb * diffuseSample.rgb);
    
    output.color = saturate(float4(dirLights + pointLights, 1.0f));
    
	return output;
}