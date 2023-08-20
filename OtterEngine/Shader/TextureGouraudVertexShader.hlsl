cbuffer transformation : register(b0) {
    matrix model;
    matrix modelViewProjection;
};

cbuffer lights : register(b1) {
    float4 lightPosition;
    float4 lightColor;
    float4 ambient;
    float intensity;
    float attenuationConst;
    float attenuationLinear;
    float attenuationQuadratic;
};

cbuffer camera : register(b2) {
    float4 cameraPosition;
}

cbuffer attributes : register(b3) {
    float shiness;
}

struct Vertex {
	float4 position	: SV_Position;
	float2 texcoord : TEXCOORD0;
    float3 normal   : NORMAL0;
};

struct Interpolant {
	float4 position	: SV_Position;
	float2 texcoord : TEXCOORD0;
    float4 light    : COLOR0;
};

Interpolant main(Vertex input) {
    Interpolant output;
    output.position = mul(input.position, modelViewProjection);
	output.texcoord = input.texcoord;
    
    const float3 worldPosition = mul(input.position, model);
    const float3 worldNormal = mul(input.normal, (float3x3) model);
    const float3 lightVector = (lightPosition - worldPosition).xyz;
    const float distance = length(lightVector);
    const float3 direction = lightVector / distance;
    
    const float3 diffuse = (lightColor.xyz * max(0.0f, dot(direction, worldNormal)));
    
    // specular
    const float3 reflection = 2 * input.normal * dot(lightVector, input.normal) - lightVector;
    const float3 specular = (lightColor.xyz * pow(max(0.0f, dot(normalize(cameraPosition - worldPosition), normalize(reflection))), shiness));
    
    const float attenuation = 1.0f / (attenuationConst + attenuationLinear * distance + attenuationQuadratic * (distance * distance));
    
    output.light = float4(saturate(ambient + (diffuse + specular) * intensity * attenuation), 1.0f); // saturate: Clamps x to the range [0, 1]

    return output;
}