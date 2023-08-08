struct Vertex {
	float4 position	: SV_Position;
	float4 texcoord : TEXCOORD0;
};

struct Interpolant {
	float4 position	: SV_Position;
	float4 texcoord : TEXCOORD0;
};

cbuffer ConstantBuffer {
	matrix transform;
};

Interpolant main(Vertex input) {
	Interpolant output;
	output.position = mul(input.position, transform); // mul using right multiply
	output.texcoord = input.texcoord;
	return output;
}