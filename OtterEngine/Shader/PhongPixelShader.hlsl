cbuffer cbuf {
    float4 lightPosition;
    float4 lightColor;
};

struct Interpolant {
    float4 position : SV_Position;
    float4 worldPosition : POSITION0;
    float4 color : COLOR0;
    float3 normal : NORMAL0;
};

struct Pixel {
    float4 color : SV_Target;
};

static const float3 ambient = { 0.15f, 0.15f, 0.15f };
static const float3 diffuseColor = { 0.8f, 0.8f, 0.5f };
static const float diffuseIntensity = 1.0f;
static const float attenuationConst = 1.0f;
static const float attenuationLinear = 0.045f;
static const float attenuationQuadratic = 0.0075f;

Pixel main(Interpolant interpolant) {
    
    const float3 lightVector = (lightPosition - interpolant.worldPosition).xyz;
    const float distance = length(lightVector);
    const float3 direction = lightVector / distance;
    
    const float attenuation = 1.0f / (attenuationConst + attenuationLinear * distance + attenuationQuadratic * (distance * distance));
    const float3 diffuse = (lightColor.xyz * diffuseIntensity * attenuation * max(0.0f, dot(direction, interpolant.normal)));
     
    Pixel output;
    output.color = interpolant.color * float4(saturate(diffuse + ambient), 1.0f); // saturate: Clamps x to the range [0, 1]
    
    return output;
}