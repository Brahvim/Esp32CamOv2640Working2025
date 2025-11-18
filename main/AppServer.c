#include "AppLog.h"
#include "AppServer.h"

struct httpd_uri g_appUriStream = {

	.uri = "/stream",
	.user_ctx = NULL,
	.method = HTTP_GET,
	.is_websocket = true,
	.handler = appUriHandlerStream,
	.handle_ws_control_frames = false,

};

esp_err_t appUriHandlerStream(struct httpd_req *const p_request) {
	if (p_request->method != HTTP_GET) {

		return ESP_ERR_HTTPD_INVALID_REQ;

	}

	httpd_ws_frame_t const frame = {

		.len = 0,
		.final = true,
		.payload = NULL,
		.type = HTTPD_WS_TYPE_BINARY,

	};

	esp_err_t const err = httpd_ws_send_frame(p_request, &frame);

	return err;
}
