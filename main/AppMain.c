#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <nvs_flash.h>
#include <esp_event.h>
#include <esp_wifi.h>
#include <socket.h>

#include <stdio.h>

#include "AppMacros.h"
#include "AppCamera.h"
#include "AppHttp.h"
#include "AppWifi.h"
#include "AppLog.h"

size_t g_clientFdsCap = 1;
size_t g_clientFdsLen = 0;
int *g_clientFds = NULL;

void app_main(void) {
	esp_err_t err = nvs_flash_init();
	g_clientFds = malloc(sizeof(int));
	if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {

		ESP_ERROR_CHECK(nvs_flash_erase());
		ESP_ERROR_CHECK(nvs_flash_init());

	}

	ESP_ERROR_CHECK(esp_event_loop_create_default());

	appInitWifi();
	appInitCam();

	while (true) {

		// ESP_LOGI(s_tag, "Taking picture...");
		camera_fb_t *fb = esp_camera_fb_get();
		// ESP_LOGI(s_tag, "Picture taken! Bytes: `%zu`.", fb->len);

		for (size_t i = 0; i < g_clientFdsLen; i++) {

			int fd = g_clientFds[i];

			if (httpd_ws_get_fd_info(g_httpd, fd) == HTTPD_WS_CLIENT_WEBSOCKET) {

				esp_err_t err = httpd_ws_send_frame_async(
					g_httpd, fd, &((httpd_ws_frame_t) {

					/**/.type = HTTPD_WS_TYPE_BINARY,
						.payload = fb->buf,
						.len = fb->len,
						.final = true,

				}));

				// #if 0
				if (!err) {

					struct sockaddr_storage ss;
					socklen_t len = sizeof(ss);

					if (!getpeername(fd, (void*) &ss, &len)) {

						switch (ss.ss_family) {

							case AF_INET: {

								struct sockaddr_in *in = (void*) &ss;

								ESP_LOGI(
									s_tag,
									"Sent image to client %d@%s:%d",
									i,
									ip4addr_ntoa((void*) &in->sin_addr),
									ntohs(in->sin_port)
								);


							} break;

							case AF_INET6: {

								struct sockaddr_in6 *in = (void*) &ss;

								ESP_LOGI(
									s_tag,
									"Sent image to client %d@[%s]:%d",
									i,
									ip6addr_ntoa((void*) &in->sin6_addr),
									ntohs(in->sin6_port)
								);

							} break;

						}

					}

					continue;

				}
				// #endif
			}

			int const t = g_clientFds[g_clientFdsLen - 1];
			// g_clientFds[i] = fd = t;
			g_clientFds[i] = t;
			g_clientFdsLen--;
			i--;
			// goto check;

		}

		esp_camera_fb_return(fb);
		// vTaskDelay(5000 / portTICK_PERIOD_MS); // 5s wait.
		// vTaskDelay(8 / portTICK_PERIOD_MS); // 64ms wait, i.e. 4 FPS.

	}
}

esp_err_t appEspCbckEndptStream(struct httpd_req *const p_request) {
	// if (p_request->method != HTTP_POST) {
	//
	// 	return ESP_ERR_HTTPD_INVALID_REQ;
	//
	// }

	int const reqFd = httpd_req_to_sockfd(p_request);
	if (httpd_ws_get_fd_info(g_httpd, reqFd) != HTTPD_WS_CLIENT_WEBSOCKET) {

		return ESP_ERR_HTTPD_INVALID_REQ;

	}

	if (g_clientFdsLen == g_clientFdsCap) {

		g_clientFds = reallocarray(
			g_clientFds,
			g_clientFdsCap = 2 * g_clientFdsCap,
			sizeof(int)
		);

	}

	g_clientFds[g_clientFdsLen++] = reqFd;
	return ESP_OK;
}

void appEspCbckWifiIp(void *p_args, esp_event_base_t p_base, int32_t p_id, void *p_data) {
	ESP_LOGI(s_tag, "Got IP!");
	ip_event_got_ip_t const *const ipEvt = p_data;
	ESP_LOGI(s_tag, "Got IP `" IPSTR "`.", IP2STR(&ipEvt->ip_info.ip));

	// Best time to start HTTPD is NOW!:
	appInitHttp();
}

void appEspCbckWifiLost(void *p_args, esp_event_base_t p_base, int32_t p_id, void *p_data) {
	ESP_LOGI(s_tag, "Wi-FI Lost...");
	esp_wifi_connect();
}

void appEspCbckWifiStart(void *p_args, esp_event_base_t p_base, int32_t p_id, void *p_data) {
	ESP_LOGI(s_tag, "Wi-FI Started.");
	esp_wifi_connect();
}
