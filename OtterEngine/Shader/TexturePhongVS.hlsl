#include "constants.hlsli"

cbuffer transformation : register(b0) {
    matrix model;
    matrix modelViewProjection;
};

struct Vertex {
	float4 position	: SV_Position;
	float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

struct Interpolant {
    float4 position : SV_Position;
    float4 worldPosition : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

Interpolant main(Vertex input) {
    Interpolant output;
    output.position = mul(input.position, modelViewProjection);
    output.worldPosition = mul(input.position, model);
    output.texcoord = input.texcoord;
    output.normal = normalize(mul(input.normal, (float3x3) model));
    
	return output;
}