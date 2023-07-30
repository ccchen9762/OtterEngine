#pragma once

// inline variables can only be used after c++ 17
// makes variables have only 1 instance over all translation units

// window configs
inline const wchar_t* kDefWndClassName = L"D3D11_WNDCLASS";
inline const wchar_t* kDefWndTitle = L"Otter Engine";

// buffer limit for message inputs
inline const unsigned int kInputBufferLimit = 16u;

// final render size
inline const unsigned int kRenderWidth = 1280u, kRenderHeight = 720u;
constexpr float kRenderRatio = static_cast<float>(kRenderHeight) / static_cast<float>(kRenderWidth);

// imgui settings
inline bool kShowWindow = true, kShowAnotherWindow = true;

// math
inline const double kPI = 3.1415926;