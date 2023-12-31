#include "LoadImage.h"

#include "OtterEngine/Common/pch.h"
#include <wincodec.h>

std::vector<uint8_t> LoadBGRAImage(const wchar_t* filename, uint32_t& width, uint32_t& height) {
    Microsoft::WRL::ComPtr<IWICImagingFactory> wicFactory;
    DX::ThrowIfFailed(CoCreateInstance(CLSID_WICImagingFactory2, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory)));

    Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;
    DX::ThrowIfFailed(wicFactory->CreateDecoderFromFilename(filename, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, decoder.GetAddressOf()));

    Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
    DX::ThrowIfFailed(decoder->GetFrame(0, frame.GetAddressOf()));

    DX::ThrowIfFailed(frame->GetSize(&width, &height));

    WICPixelFormatGUID pixelFormat;
    DX::ThrowIfFailed(frame->GetPixelFormat(&pixelFormat));

    uint32_t rowPitch = width * sizeof(uint32_t);
    uint32_t imageSize = rowPitch * height;

    std::vector<uint8_t> image;
    image.resize(size_t(imageSize));

    if (memcmp(&pixelFormat, &GUID_WICPixelFormat32bppBGRA, sizeof(GUID)) == 0)
    {
        DX::ThrowIfFailed(frame->CopyPixels(nullptr, rowPitch, imageSize, reinterpret_cast<BYTE*>(image.data())));
    }
    else
    {
        Microsoft::WRL::ComPtr<IWICFormatConverter> formatConverter;
        DX::ThrowIfFailed(wicFactory->CreateFormatConverter(formatConverter.GetAddressOf()));

        BOOL canConvert = FALSE;
        DX::ThrowIfFailed(formatConverter->CanConvert(pixelFormat, GUID_WICPixelFormat32bppBGRA, &canConvert));
        if (!canConvert)
        {
            throw std::exception("CanConvert");
        }

        DX::ThrowIfFailed(formatConverter->Initialize(frame.Get(), GUID_WICPixelFormat32bppBGRA,
            WICBitmapDitherTypeErrorDiffusion, nullptr, 0, WICBitmapPaletteTypeMedianCut));

        DX::ThrowIfFailed(formatConverter->CopyPixels(nullptr, rowPitch, imageSize, reinterpret_cast<BYTE*>(image.data())));
    }

    return image;
}