#pragma once

// inline variables can only be used after c++ 17
// makes variables have only 1 instance over all translation units

// window configs
inline const wchar_t* kDefWndClassName = L"D3D11_WNDCLASS";
inline const wchar_t* kDefWndTitle = L"Otter Engine";

// buffer limit for message inputs
inline const unsigned int kInputBufferLimit = 16u;

// final render size
inline const unsigned int kRenderWidth = 1600u, kRenderHeight = 900u;
constexpr float kRenderRatio = static_cast<float>(kRenderWidth) / static_cast<float>(kRenderHeight);

inline const float kNearZ = 0.1f, kFarZ = 300.0f;

// imgui settings
inline bool kShowWindow = true, kShowAnotherWindow = true;

// math
inline const double kPI = 3.1415926;

// rendering settings
inline const unsigned int kVSync = 0u;

enum class RenderMethod {
	Gouraud = 0, Phong = 1
};
inline const RenderMethod kRenderMethod = RenderMethod::Phong;

// light settings ** related to constants.hlsli
inline const int kMaxLight = 10;