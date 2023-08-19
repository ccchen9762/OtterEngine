cbuffer transformation : register(b0) {
    matrix model;
    matrix modelViewProjection;
};

cbuffer lights : register(b1) {
    float4 lightPosition;
    float4 lightColor;
};

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

static const float3 ambient = { 0.15f, 0.15f, 0.15f };
static const float diffuseIntensity = 1.0f;
static const float attenuationConst = 1.0f;
static const float attenuationLinear = 0.045f;
static const float attenuationQuadratic = 0.0075f;

Interpolant main(Vertex input) {
    Interpolant output;
    output.position = mul(input.position, modelViewProjection);
	output.texcoord = input.texcoord;
    
    const float3 worldPosition = mul(input.position, model);
    const float3 worldNormal = mul(input.normal, (float3x3) model);
    const float3 lightVector = (lightPosition - worldPosition).xyz;
    const float distance = length(lightVector);
    const float3 direction = lightVector / distance;
    
    const float attenuation = 1.0f / (attenuationConst + attenuationLinear * distance + attenuationQuadratic * (distance * distance));
    const float3 diffuse = (lightColor.xyz * diffuseIntensity * attenuation * max(0.0f, dot(direction, worldNormal)));
     
    output.light = float4(saturate(diffuse + ambient), 1.0f); // saturate: Clamps x to the range [0, 1]
    
    return output;
}