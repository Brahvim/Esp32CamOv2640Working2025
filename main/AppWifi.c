#include <esp_wifi.h>

#include "AppMacros.h"
#include "SECRETS.H"
#include "AppWifi.h"
#include "AppMain.h"
#include "AppLog.h"

static wifi_config_t s_wifiConfig = {

	.sta = {

		.sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
		.password = PASS,
		.threshold = {

		   .rssi = -80,
		   .authmode = WIFI_AUTH_WPA2_PSK,

	   },
		.ssid = SSID,
		.pmf_cfg = {

			.capable = true,
			.required = false,

		},

	}

};

void appInitWifi() {
	ESP_ERROR_CHECK(esp_netif_init());
	esp_netif_create_default_wifi_sta();

	wifi_init_config_t wifiConfInit = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&wifiConfInit));

	ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT,
		WIFI_EVENT_STA_START,
		&appEspCbckWifiStart,
		NULL));

	ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT,
		WIFI_EVENT_STA_DISCONNECTED,
		&appEspCbckWifiLost,
		NULL));

	ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT,
		IP_EVENT_STA_GOT_IP,
		&appEspCbckWifiIp,
		NULL));

	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &s_wifiConfig));
	ESP_ERROR_CHECK(esp_wifi_start());
}
