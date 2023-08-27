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

struct Vertex {
	float4 position	: SV_Position;
	float2 texcoord : TEXCOORD0;
    float3 normal   : NORMAL0;
};

struct Interpolant {
	float4 position	    : SV_Position;
	float2 texcoord     : TEXCOORD0;
    float4 diffuse      : COLOR0;
    float4 specular     : COLOR1;
};

Interpolant main(Vertex input) {
    Interpolant output;
    output.position = mul(input.position, modelViewProjection);
	output.texcoord = input.texcoord;
    
    const float3 worldPosition = mul(input.position, model);
    const float3 worldNormal = normalize(mul(input.normal, (float3x3) model));
    const float3 lightVector = (lightPosition - worldPosition).xyz;
    const float distance = length(lightVector);
    const float3 lightUnitVector = lightVector / distance;
    
    output.diffuse = float4(lightColor.rgb * max(0.0f, dot(lightUnitVector, worldNormal)), 1.0f); // waiting for k_d
    
    // specular
    const float3 reflection = 2 * dot(lightUnitVector, worldNormal) * worldNormal - lightUnitVector;
    output.specular.x = max(0.0f, dot(normalize(cameraPosition - worldPosition), reflection)); // waiting for shiness, k_s, l_s
    
    // save attenuation in 4th element of specular
    output.specular.w = 1.0f / (attenuationConst + attenuationLinear * distance + attenuationQuadratic * (distance * distance));
    
    return output;
}