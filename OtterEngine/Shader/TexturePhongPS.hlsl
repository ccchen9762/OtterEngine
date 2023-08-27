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
    bool hasSpecularMap;
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

Pixel main(Interpolant input)
{
	Pixel output;
    
    input.normal = normalize(input.normal);
    
    const float3 lightVector = (lightPosition - input.worldPosition).xyz;
    const float distance = length(lightVector);
    const float3 lightUnitVector = lightVector / distance;
    
    // k_d * l_d * (l dot n)
    const float4 diffuseSample = texDiffuse.Sample(samDiffuse, input.texcoord);
    const float3 diffuse = diffuseSample.xyz * lightColor.rgb * max(0.0f, dot(lightUnitVector, input.normal));
    
    // specular
    // 2 * l dot n * n - l
    const float3 reflection = 2 * dot(lightUnitVector, input.normal) * input.normal - lightUnitVector;
    // k_s * l_s * (r dot v)^shiness
    float3 specular; 
    if (hasSpecularMap) {
        const float4 specularSample = texSpecular.Sample(samSpecular, input.texcoord);
        const float sampleShiness = pow(2, specularSample.a * 13.0f);
        specular = specularSample.rgb * lightColor.rgb * pow(max(0.0f, dot(normalize(cameraPosition - input.worldPosition), reflection)), sampleShiness);
    }
    else {
        specular = (lightColor.rgb * pow(max(0.0f, dot(normalize(cameraPosition - input.worldPosition), reflection)), shiness));
    }
     
    const float attenuation = 1.0f / (attenuationConst + attenuationLinear * distance + attenuationQuadratic * (distance * distance));
    output.color = float4(saturate(
        ambient.rgb * diffuseSample + attenuation * intensity * (diffuse + specular)),
        1.0f); // saturate: Clamps x to the range [0, 1]
        
	return output;
}