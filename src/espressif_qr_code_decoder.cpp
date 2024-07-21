#include "espressif_qr_code_decoder.hpp"
#include <assert.h>
#include <cstring>
#include "esp_log.h"

EspressifQrCodeDecoder::EspressifQrCodeDecoder() {

}

std::vector<std::string> EspressifQrCodeDecoder::decode(const Image* image) {
    
    std::vector<std::string> ret;

    esp_code_scanner_image_format_t fmt{};
    switch (image->format) {
        case eQrCodeReaderImageFormats::QR_IMAGE_GRAYSCALE:
            fmt = esp_code_scanner_image_format_t::ESP_CODE_SCANNER_IMAGE_GRAY;
            break;
        case eQrCodeReaderImageFormats::QR_IMAGE_RGB565:
            fmt = esp_code_scanner_image_format_t::ESP_CODE_SCANNER_IMAGE_RGB565;
            break;
        case eQrCodeReaderImageFormats::QR_IMAGE_YUV422:
            fmt = esp_code_scanner_image_format_t::ESP_CODE_SCANNER_IMAGE_YUV422;
            break;        
        default:
            break;
    }

    scanner_ = esp_code_scanner_create();
    assert(scanner_);

    esp_code_scanner_config_t config{ESP_CODE_SCANNER_MODE_FAST, fmt, image->width, image->height};
    const esp_err_t err{esp_code_scanner_set_config(scanner_, config)};
    if(ESP_OK != err) {
        ESP_LOGE(TAG, "esp_code_scanner_set_config error: %d", (int)err);
        esp_code_scanner_destroy(scanner_);
        return ret;
    }
    
    const int decoded_num{esp_code_scanner_scan_image(scanner_, image->data)};
    if(decoded_num > 0U) {
        esp_code_scanner_symbol_t qr_data{esp_code_scanner_result(scanner_)};
        while(true) {
            ret.push_back(std::string{qr_data.data});
            if(qr_data.next) {
                qr_data = *(qr_data.next);
            } else {
                break;
            }
        }

    }
    esp_code_scanner_destroy(scanner_);
    return ret;   
}

EspressifQrCodeDecoder::~EspressifQrCodeDecoder() {
}