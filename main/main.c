#include <stdio.h>

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "app_macros.h"
#include "app_camera.h"
#include "app_camera_pins.h"

static const char *s_tag = "app_" __FILE__;

void app_main(void) {
	app_init_camera();

	while (1) {
		ESP_LOGI(s_tag, "Taking picture...");
		camera_fb_t *fb = esp_camera_fb_get();

		ESP_LOGI(s_tag, "Picture taken! Bytes: `%zu`.", fb->len);
		esp_camera_fb_return(fb);

		// uint8_t const *pic_data_raw = fb->buf;
		vTaskDelay(5000 / portTICK_PERIOD_MS); // 5s wait.
	}
}
