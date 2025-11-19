#pragma once

#include <esp_http_server.h>

extern int *g_clientFds;
extern size_t g_clientFdsCap;
extern size_t g_clientFdsLen;

esp_err_t appEspCbckEndptStream(struct httpd_req *const request);
void appEspCbckWifiIp(void *args, esp_event_base_t base, int32_t id, void *data);
void appEspCbckWifiLost(void *args, esp_event_base_t base, int32_t id, void *data);
void appEspCbckWifiStart(void *args, esp_event_base_t base, int32_t id, void *data);
