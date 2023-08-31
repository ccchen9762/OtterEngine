cbuffer attributes : register(b4) {
    float4 objectColor;
};

struct Interpolant {
    float4 position : SV_Position;
};

struct Pixel {
    float4 color : SV_TARGET;
};

Pixel main(Interpolant input) {
    Pixel output;
    output.color = objectColor;
    return output;
}