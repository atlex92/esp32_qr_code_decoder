#include <string>
#include "quirc_qr_code_decoder.hpp"
#include "espressif_qr_code_decoder.hpp"
#include "esp_camera.h"
#include "camera_config.h"
#include "esp_log.h"
#include "esp_timer.h"

static const char* const TAG{"qr_code_decoder_example"};

static const camera_config_t camera_config = {
    .pin_pwdn = CAM_PIN_PWDN,
    .pin_reset = CAM_PIN_RESET,
    .pin_xclk = CAM_PIN_XCLK,
    .pin_sccb_sda = CAM_PIN_SIOD,
    .pin_sccb_scl = CAM_PIN_SIOC,

    .pin_d7 = CAM_PIN_D7,
    .pin_d6 = CAM_PIN_D6,
    .pin_d5 = CAM_PIN_D5,
    .pin_d4 = CAM_PIN_D4,
    .pin_d3 = CAM_PIN_D3,
    .pin_d2 = CAM_PIN_D2,
    .pin_d1 = CAM_PIN_D1,
    .pin_d0 = CAM_PIN_D0,
    .pin_vsync = CAM_PIN_VSYNC,
    .pin_href = CAM_PIN_HREF,
    .pin_pclk = CAM_PIN_PCLK,

    //XCLK 20MHz or 10MHz for OV2640 double FPS (Experimental)
    .xclk_freq_hz = 20'000'000,
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,

    .pixel_format = PIXFORMAT_GRAYSCALE, //YUV422,GRAYSCALE,RGB565,JPEG
    .frame_size = FRAMESIZE_HVGA,    //QQVGA-UXGA, For ESP32, do not use sizes above QVGA when not JPEG. The performance of the ESP32-S series has improved a lot, but JPEG mode always gives better frame rates.

    .jpeg_quality = 10, //0-63, for OV series camera sensors, lower number means higher quality
    .fb_count = 2,       //When jpeg mode is used, if fb_count more than one, the driver will work in continuous mode.
    .fb_location = CAMERA_FB_IN_PSRAM,
    .grab_mode = CAMERA_GRAB_LATEST,
};

extern "C" void app_main(void*) {

    auto start{esp_timer_get_time()/1000U};
    const esp_err_t err{esp_camera_init(&camera_config)};
    auto end{esp_timer_get_time()/1000U};
    ESP_LOGI(TAG, "camera initialization took %d ms", (int)(end - start));

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Camera Init Failed");
        return;
    }

    int cntr{};

#ifdef CONFIG_QR_CODE_READER_IMPLEMENTATION_QUIRC
    IQrCodeDecoder* decoder{new QuircQrCodeDecoder};
#elif defined(CONFIG_QR_CODE_READER_IMPLEMENTATION_ESPRESSIF)
    IQrCodeDecoder* decoder{new EspressifQrCodeDecoder};
#endif

    while(true){
        start = esp_timer_get_time()/1000U;
        camera_fb_t* fb{esp_camera_fb_get()};
        if (!fb) {
            ESP_LOGE(TAG,"Camera capture failed");
        } else {
            Image img {
                .data = fb->buf,
                .size = fb->len,
                .width = fb->width,
                .height = fb->height
            };

            switch (fb->format) {
                case pixformat_t::PIXFORMAT_GRAYSCALE:
                    img.format = eQrCodeReaderImageFormats::QR_IMAGE_GRAYSCALE;
                    break;
                case pixformat_t::PIXFORMAT_YUV422:
                    img.format = eQrCodeReaderImageFormats::QR_IMAGE_YUV422;
                    break;
                case pixformat_t::PIXFORMAT_RGB565:
                    img.format = eQrCodeReaderImageFormats::QR_IMAGE_RGB565;
                    break;                
                default:
                    ESP_LOGE(TAG, "unsupported format: %d", (int)fb->format);
                    return;
                    break;
            }

            const std::vector<std::string> decoded_qr{decoder->decode(&img)};
            if(decoded_qr.size() > 0) {
                ESP_LOGI(TAG, "%u qr decoded!", decoded_qr.size());
                for(const std::string& qr_data : decoded_qr) {
                    ESP_LOGI(TAG, "qr text: %s", qr_data.c_str());
                    cntr++;
                }
            }
        }
        esp_camera_fb_return(fb);
        end = esp_timer_get_time()/1000U;
        ESP_LOGI(TAG, "cycle took %d ms", (int)(end - start));
        ESP_LOGI(TAG, "decode rate = %.1f", 1000000.F * cntr / esp_timer_get_time());
    } 
}