#pragma once

#include <esp_http_server.h>

extern struct httpd_uri g_appEndpts[];
extern httpd_handle_t g_httpd;

esp_err_t appInitHttp();
