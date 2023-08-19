cbuffer transformation : register(b0) {
    matrix model;
    matrix modelViewProjection;
};

struct Vertex {
	float4 position	: SV_Position;
	float2 texcoord : TEXCOORD0;
};

struct Interpolant {
	float4 position	: SV_Position;
	float2 texcoord : TEXCOORD0;
};

Interpolant main(Vertex input) {
	Interpolant output;
    output.position = mul(input.position, modelViewProjection); // mul using right multiply
	output.texcoord = input.texcoord;
	return output;
}