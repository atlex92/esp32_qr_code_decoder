#pragma once

#include "i_qr_code_decoder.hpp"
#include "esp_code_scanner.h"

class EspressifQrCodeDecoder : public IQrCodeDecoder {
public:
    EspressifQrCodeDecoder();
    std::vector<std::string> decode(const Image* image) override;
    ~EspressifQrCodeDecoder();
private:
    static constexpr const char* const TAG{"EspressifQrCodeDecoder"};
    esp_image_scanner_t* scanner_{};
};