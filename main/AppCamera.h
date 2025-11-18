#pragma once

#include <esp_camera.h>

extern sensor_t *g_appCamSensor;
extern camera_config_t g_appCamConfig;

void appInitCam(void);
