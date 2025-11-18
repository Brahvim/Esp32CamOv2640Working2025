#include "AppLog.h"
#include "AppMacros.h"
#include "AppCamera.h"

sensor_t *g_appCamSensor;
camera_config_t g_appCamConfig = {

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
	.pin_reset = -1, // `-1` causes **software resets** only!
	.pin_vsync = 25,

	.pin_sccb_scl = 27, // Part of the SCCB DMA protocol...
	.pin_sccb_sda = 26, // Part of the SCCB DMA protocol...

	.xclk_freq_hz = 20000000, // 20M, i.e. 20MHz.
	.ledc_timer = LEDC_TIMER_0,
	.ledc_channel = LEDC_CHANNEL_0,

	.fb_count = 1,
	.jpeg_quality = 63,
	.frame_size = FRAMESIZE_VGA,
	.pixel_format = PIXFORMAT_JPEG, // `PIXFORMAT_JPEG` for streaming. `PIXFORMAT_RGB565` for face detection and recognition.
	.fb_location = CAMERA_FB_IN_PSRAM,
	.grab_mode = CAMERA_GRAB_WHEN_EMPTY,

};

void appInitCam(void) {
	ifu(ESP_OK != esp_camera_init(&g_appCamConfig)) {

		ESP_LOGE(s_tag, "Camera didn't start.");

	}

	g_appCamSensor = esp_camera_sensor_get();
	ESP_LOGI(s_tag, "Pixel format: `%d`.", g_appCamSensor->pixformat);
}
