#pragma once

#include <esp_http_server.h>

extern struct httpd_uri g_appUriStream;

esp_err_t appUriHandlerStream(struct httpd_req *request);
