#pragma once

#include <esp_camera.h>

extern sensor_t *g_sensor;
extern camera_config_t g_camera_config;

void app_init_camera(void);
