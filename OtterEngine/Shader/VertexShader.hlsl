struct Vertex {
	float4 position	: SV_Position;
	float4 color	: COLOR0;
};

struct Interpolant {
	float4 position	: SV_Position;
	float4 color	: COLOR0;
};

cbuffer ConstantBuffer {
	// matrix is default column major, need to transpose
	// transpose on GPU side is slower than on CPU side
	//		-> transpose on CPU side to get a normal effect
	matrix transform;
};

Interpolant main (Vertex input) {
	Interpolant output;
	output.position = mul(input.position, transform); // mul using right multiply
	output.color = input.color;
	return output;
}