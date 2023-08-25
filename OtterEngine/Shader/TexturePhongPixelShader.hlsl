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

Texture2D texSpecular : register(t1);
SamplerState samSpecular : register(s1);

Pixel main(Interpolant input)
{
	Pixel output;
    
    const float3 lightVector = (lightPosition - input.worldPosition).xyz;
    const float distance = length(lightVector);
    const float3 direction = lightVector / distance;
    const float attenuation = 1.0f / (attenuationConst + attenuationLinear * distance + attenuationQuadratic * (distance * distance));
    
    const float3 diffuse = attenuation * (lightColor.rgb * max(0.0f, dot(direction, input.normal)));
    
    // specular
    const float3 reflection = 2 * input.normal * dot(lightVector, input.normal) - lightVector;
    float3 specular;
    if (hasSpecularMap) {
        const float4 specularSample = texSpecular.Sample(samSpecular, input.texcoord);
        const float sampleShiness = pow(2, specularSample.a * 13.0f);
        specular = (specularSample.rgb * pow(max(0.0f, dot(normalize(cameraPosition - input.worldPosition), normalize(reflection))), sampleShiness));
    }
    else {
        const float4 specularSample = texSpecular.Sample(samSpecular, input.texcoord);
        specular = (lightColor.rgb * pow(max(0.0f, dot(normalize(cameraPosition - input.worldPosition), normalize(reflection))), shiness));
    }
    //const float4 specularSample = texSpecular.Sample(samSpecular, input.texcoord);
    //const float sampleShiness = pow(2, specularSample.a * 13.0f);
    //specular = (specularSample.rgb * pow(max(0.0f, dot(normalize(cameraPosition - input.worldPosition), normalize(reflection))), sampleShiness));

    specular *= attenuation;
     
    output.color = float4(saturate(tex.Sample(sam, input.texcoord) * 
        (ambient + diffuse * intensity) + specular * intensity),
    1.0f); // saturate: Clamps x to the range [0, 1]
        
	return output;
}