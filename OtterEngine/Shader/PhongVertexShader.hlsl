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

Interpolant main(Vertex vertex)
{
    Interpolant interpolant;
    interpolant.position = mul(vertex.position, modelViewProjection);
    interpolant.worldPosition = mul(vertex.position, model);
    interpolant.color = vertex.color;
    interpolant.normal = mul(vertex.normal, (float3x3)model);
    
    return interpolant;
}