cbuffer transformation : register(b0) {
    matrix model;
    matrix modelViewProjection;
};

struct Vertex {
    float4 position : SV_Position;
    float4 color : COLOR0;
    float3 normal : NORMAL0;
};

struct Interpolant {
    float4 position : SV_Position;
    float4 worldPosition : POSITION0;
    float4 color : COLOR0;
    float3 normal : NORMAL0;
};

Interpolant main(Vertex input)
{
    Interpolant output;
    output.position = mul(input.position, modelViewProjection);
    output.worldPosition = mul(input.position, model);
    output.color = input.color;
    output.normal = mul(input.normal, (float3x3) model);
    
    return output;
}