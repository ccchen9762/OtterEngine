cbuffer lights : register(b1) {
    float4 lightPosition;
    float4 lightColor;
    float4 ambient;
    float diffuseIntensity;
    float attenuationConst;
    float attenuationLinear;
    float attenuationQuadratic;
};

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
    
    const float attenuation = 1.0f / (attenuationConst + attenuationLinear * distance + attenuationQuadratic * (distance * distance));
    const float3 diffuse = (lightColor.xyz * diffuseIntensity * attenuation * max(0.0f, dot(direction, input.normal)));
     
    output.color = tex.Sample(sam, input.texcoord) * float4(saturate(diffuse + ambient), 1.0f); // saturate: Clamps x to the range [0, 1]
        
	return output;
}