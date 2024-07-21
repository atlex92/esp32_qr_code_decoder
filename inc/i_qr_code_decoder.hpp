#pragma once

#include <stdint.h>
#include <cstddef>
#include <vector>
#include <string>

enum eQrCodeReaderImageFormats {
    QR_IMAGE_GRAYSCALE = 0,
    QR_IMAGE_YUV422,
    QR_IMAGE_RGB565,
    QR_IMAGE_MAX    
};

struct Image {
    const uint8_t* data;
    const size_t size;
    const size_t width;
    const size_t height;
    eQrCodeReaderImageFormats format;
};

class IQrCodeDecoder {
public:
    virtual ~IQrCodeDecoder() = default;
    virtual std::vector<std::string> decode(const Image* image) = 0;
};