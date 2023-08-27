struct Interpolant {
	float4 position	: SV_Position;
	float2 texcoord : TEXCOORD0;
    float4 light    : COLOR0;
};

struct Pixel {
	float4 color	: SV_Target;
};

Texture2D tex		: register(t0);
SamplerState sam	: register(s0);

Pixel main(Interpolant input)
{
	Pixel output;
    output.color = tex.Sample(sam, input.texcoord) * input.light;
	return output;
}