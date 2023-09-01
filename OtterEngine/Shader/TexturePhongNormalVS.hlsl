#include "constants.hlsli"

cbuffer transformation : register(b0) {
    matrix model;
    matrix modelViewProjection;
};

struct Vertex {
    float4 position : SV_Position;
    float2 texcoord : TEXCOORD0;
    float3 normal   : NORMAL0;
    float3 tangent  : NORMAL1;
    float3 bitangent: NORMAL2;
};

struct Interpolant {
    float4 position : SV_Position;
    float4 worldPosition : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 tangent : NORMAL1;
    float3 bitangent : NORMAL2;
};

Interpolant main(Vertex input) {
    Interpolant output;
    output.position = mul(input.position, modelViewProjection);
    output.worldPosition = mul(input.position, model);
    output.texcoord = input.texcoord;
    output.normal = normalize(mul(input.normal, (float3x3) model));
    output.tangent = normalize(mul(input.tangent, (float3x3) model));
    output.bitangent = normalize(mul(input.bitangent, (float3x3) model));
    
    return output;
}