#pragma once

// target Windows 10 and later
#define _WIN32_WINNT 0x0A00 
#include <sdkddkver.h>

// disable unnecessary macro in Windows.h
#define NOMINMAX

#include <Windows.h>

#include <wrl/client.h>

#include <vector>
#include <exception>
#include <iostream>

namespace DX {
    // Helper class for COM exceptions
    class com_exception : public std::exception
    {
    public:
        com_exception(HRESULT hr) noexcept : result(hr) {}

        const char* what() const override {
            static char s_str[64] = {};
            sprintf_s(s_str, "Failure with HRESULT of %08X", static_cast<unsigned int>(result));

            std::wstring wstr;
            const size_t len = strlen(s_str);
            wstr.reserve(len);
            mbstowcs_s(nullptr, &wstr[0], len + 1, s_str, len);

            unsigned int result = MessageBox(nullptr, L"COM EXCEPTION", wstr.c_str(), MB_OK);

            return s_str;
        }

    private:
        HRESULT result;
    };

    // Helper utility converts D3D API failures into exceptions.
    inline void ThrowIfFailed(HRESULT hr) {
        if (FAILED(hr)){
            throw com_exception(hr);
        }
    }
}