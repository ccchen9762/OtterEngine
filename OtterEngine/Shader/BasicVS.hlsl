cbuffer transformation : register(b0) {
	// matrix is default column major, need to transpose
	// transpose on GPU side is slower than on CPU side
	//		-> transpose on CPU side to get a normal effect
    matrix model;
    matrix modelViewProjection;
};

struct Vertex {
    float4 position : SV_Position;
};

struct Interpolant {
    float4 position : SV_Position;
};

Interpolant main(Vertex input) {
    Interpolant output;
    output.position = mul(input.position, modelViewProjection); // mul using right multiply
    return output;
}