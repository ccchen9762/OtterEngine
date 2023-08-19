struct Interpolant {
    float4 position : SV_Position;
    float4 color : COLOR0;
};

struct Pixel {
    float4 color : SV_TARGET;
};

Pixel main(Interpolant input) {
    Pixel output;
    output.color = input.color;
    return output;
}