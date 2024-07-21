#include "quirc_qr_code_decoder.hpp"
#include <assert.h>
#include <cstring>
#include "esp_log.h"

QuircQrCodeDecoder::QuircQrCodeDecoder() {
    initialize();
}

std::vector<std::string> QuircQrCodeDecoder::decode(const Image* image) {
    if(image->format != eQrCodeReaderImageFormats::QR_IMAGE_GRAYSCALE) {
        ESP_LOGE(TAG, "QUIRC qr decoder supports only QR_IMAGE_GRAYSCALE format!");
        return {};
    }

    std::vector<std::string> ret;

    quirc_resize(quirc_, image->width, image->height);
    image_ = quirc_begin(quirc_, NULL, NULL);
    memcpy(image_, image->data, image->size);
    quirc_end(quirc_);

    const auto qr_count{quirc_count(quirc_)};

    for (size_t i{}; i < qr_count; i++) {
        struct quirc_code code{};
        quirc_decode_error_t q_err{};
        quirc_extract(quirc_, i, &code);
        q_err = quirc_decode(&code, &quirc_data_);

        if (q_err) {
            const char *error = quirc_strerror(q_err);
            ESP_LOGE(TAG,"Decoding FAILED: %s\n", error);
        }

        else {
            ret.push_back(std::string{reinterpret_cast<const char*>(quirc_data_.payload)});
        }
    }

    return ret;   
}

void QuircQrCodeDecoder::initialize() {
    quirc_ = quirc_new();
    assert(quirc_);
}

QuircQrCodeDecoder::~QuircQrCodeDecoder() {
    quirc_destroy(quirc_);
}