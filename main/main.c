#include <freertos/FreeRTOS.h> // MUST BE first!
#include <freertos/task.h>
#include <esp_log.h>
#include <stdio.h>

#include "app_macros.h"
#include "app_camera.h"

static const char *s_tag = __FILE__;

void app_main(void) {
	app_init_camera();

	while (1) {

		ESP_LOGI(s_tag, "Taking picture...");
		camera_fb_t *fb = esp_camera_fb_get();

		ESP_LOGI(s_tag, "Picture taken! Bytes: `%zu`.", fb->len);
		esp_camera_fb_return(fb);

		// uint8_t const *pic_data_raw = fb->buf;
		vTaskDelay(pdMS_TO_TICKS(5000)); // 5s wait.

	}
}
