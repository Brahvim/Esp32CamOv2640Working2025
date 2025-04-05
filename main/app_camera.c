#include <esp_log.h>

#include "app_macros.h"
#include "app_camera.h"
#include "app_camera_pins.h"

sensor_t *g_sensor;
camera_config_t g_camera_config = {

	.pin_d0 = 5,
	.pin_d1 = 18,
	.pin_d2 = 19,
	.pin_d3 = 21,
	.pin_d4 = 36,
	.pin_d5 = 39,
	.pin_d6 = 34,
	.pin_d7 = 35,

	.pin_xclk = 0,
	.pin_pclk = 22,

	.pin_href = 23,
	.pin_pwdn = 32,
	.pin_reset = -1,
	.pin_vsync = 25,

	.pin_sccb_scl = 27,
	.pin_sccb_sda = 26,

	.xclk_freq_hz = 20000000,
	.ledc_timer = LEDC_TIMER_0,
	.ledc_channel = LEDC_CHANNEL_0,

	.fb_count = 1,
	.jpeg_quality = 63,
	.frame_size = FRAMESIZE_VGA,
	.pixel_format = PIXFORMAT_JPEG, // `PIXFORMAT_JPEG` for streaming. `PIXFORMAT_RGB565` for face detection and recognition.
	.fb_location = CAMERA_FB_IN_PSRAM,
	.grab_mode = CAMERA_GRAB_WHEN_EMPTY,

};

static const char *s_tag = __FILE__;

void app_init_camera(void) {
	esp_err_t initError = esp_camera_init(&g_camera_config);
	g_sensor = esp_camera_sensor_get();

	ifl(g_sensor->id.PID != OV2640_PID) {

		ESP_LOGE("camera", "Unsupported camera sensor!");

	}

	ESP_LOGI(s_tag, "Pixel format: `%d`.", g_sensor->pixformat);

	ifu(ESP_OK != initError)
		ESP_LOGE(s_tag, "Camera didn't start.");
}
