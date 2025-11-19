#include "AppMacros.h"
#include "AppHttp.h"
#include "AppMain.h"
#include "AppLog.h"

struct httpd_uri g_appEndpts[] = {
    {

        .handle_ws_control_frames = false,
        .handler = &appEspCbckEndptStream,
        .is_websocket = true,
        .method = HTTP_GET,
        .user_ctx = NULL,
        .uri = "/stream",

    },
};
httpd_handle_t g_httpd = { 0 };

esp_err_t appInitHttp() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    esp_err_t err = httpd_start(&g_httpd, &config);

    if (err) {

        ESP_LOGE(s_tag, "Failed to start HTTP server due to an `%s`!", esp_err_to_name(err));
        return err;

    }

    for (size_t i = 0; i < sizeof(g_appEndpts) / sizeof(g_appEndpts[0]); i++) {

        err = httpd_register_uri_handler(g_httpd, g_appEndpts + i);

        if (err) {

            return err;

        }

    }

    return err;
}
