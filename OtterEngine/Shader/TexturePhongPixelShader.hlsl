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

struct Interpolant {
	float4 position	: SV_Position;
    float4 worldPosition : POSITION0;
	float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

struct Pixel {
	float4 color	: SV_Target;
};

Texture2D tex		: register(t0);
SamplerState sam	: register(s0);

Pixel main(Interpolant input)
{
	Pixel output;
    
    const float3 lightVector = (lightPosition - input.worldPosition).xyz;
    const float distance = length(lightVector);
    const float3 direction = lightVector / distance;
    
    const float3 diffuse = (lightColor.xyz * max(0.0f, dot(direction, input.normal)));
    
    // specular
    const float3 reflection = 2 * input.normal * dot(lightVector, input.normal) - lightVector;
    const float3 specular = (lightColor.xyz * pow(max(0.0f, dot(normalize(cameraPosition - input.worldPosition), normalize(reflection))), shiness));
    
    const float attenuation = 1.0f / (attenuationConst + attenuationLinear * distance + attenuationQuadratic * (distance * distance));
     
    output.color = tex.Sample(sam, input.texcoord) * float4(saturate(ambient + (diffuse + specular) * intensity * attenuation), 1.0f); // saturate: Clamps x to the range [0, 1]
        
	return output;
}