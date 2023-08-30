cbuffer lights : register(b1) {
    float4 lightPosition;
    float4 lightColor;
    float4 ambient;
    float intensity;
    float attenuationConst;
    float attenuationLinear;
    float attenuationQuadratic;
};

cbuffer attributes : register(b3) {
    float shiness;
    bool hasSpecularMap;
    float2 padding;
}

struct Interpolant {
    float4 position     : SV_Position;
    float2 texcoord     : TEXCOORD0;
    float4 diffuse      : COLOR0;
    float4 specular     : COLOR1;
};

struct Pixel {
	float4 color	: SV_Target;
};

Texture2D texDiffuse : register(t0);
SamplerState samDiffuse : register(s0);

Texture2D texSpecular : register(t1);
SamplerState samSpecular : register(s1);

Texture2D texNormal : register(t2);
SamplerState samNormal : register(s2);

Pixel main(Interpolant input)
{
	Pixel output;
    
    const float4 diffuseSample = texDiffuse.Sample(samDiffuse, input.texcoord);
    const float4 specularSample = texSpecular.Sample(samSpecular, input.texcoord);
    
    const float3 diffuse = input.diffuse.rgb * diffuseSample;
    
    const float3 specular = lightColor.rgb * pow(input.specular.x, shiness);
    
    output.color = float4(saturate(
        ambient.rgb * diffuseSample + input.specular.w * intensity * (diffuse + specular)),
        1.0f); // saturate: Clamps x to the range [0, 1]
	return output;
}