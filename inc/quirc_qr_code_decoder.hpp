#pragma once

#include "i_qr_code_decoder.hpp"
#include "quirc.h"

class QuircQrCodeDecoder : public IQrCodeDecoder {
public:
    QuircQrCodeDecoder();
    std::vector<std::string> decode(const Image* image) override;
    ~QuircQrCodeDecoder();
private:
    static constexpr const char* const TAG{"QuircQrCodeDecoder"};
    void initialize();
    quirc* quirc_{nullptr};
    uint8_t* image_{nullptr};
    quirc_data quirc_data_{};
};