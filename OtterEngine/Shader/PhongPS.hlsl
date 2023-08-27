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
    float4 position : SV_Position;
    float4 worldPosition : POSITION0;
    float4 color : COLOR0;
    float3 normal : NORMAL0;
};

struct Pixel {
    float4 color : SV_Target;
};

Pixel main(Interpolant input) {
    
    Pixel output;
    
    input.normal = normalize(input.normal);
    
    const float3 lightVector = (lightPosition - input.worldPosition).xyz;
    const float distance = length(lightVector);
    const float3 lightUnitVector = lightVector / distance;
    
    // diffuse
    const float3 diffuse = input.color.rgb * lightColor.rgb * max(0.0f, dot(lightUnitVector, input.normal));
    
    // specular
    const float3 reflection = 2 * dot(lightUnitVector, input.normal) * input.normal - lightUnitVector;
    const float3 specular = lightColor.rgb * pow(max(0.0f, dot(normalize(cameraPosition - input.worldPosition), reflection)), shiness);
    
    const float attenuation = 1.0f / (attenuationConst + attenuationLinear * distance + attenuationQuadratic * (distance * distance));
    
    output.color = float4(saturate(
        ambient.rgb * input.color + attenuation * intensity * (diffuse + specular)),
        1.0f); // saturate: Clamps x to the range [0, 1]
    
    return output;
}