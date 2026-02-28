#pragma once
#include "configuration.h"
#include "logger.hpp"
const uint32_t delay_ms=3000;
void taskE(void *parameters)
{
    int id = (int)parameters;
    char tag[16];
    sprintf(tag, "TASK_%02d", id);
    int core = xPortGetCoreID();
    while (true)
    {
        uint32_t start = micros();
        logE("[Core %d] Critical Error occurred!", core);
        uint32_t end = micros();
        uint32_t cost = end - start;
        if (cost > 1000) {
            ets_printf("\n!! ALERT: Task_%02d on Core %d BLOCKED for %u us !!\n\n", id, core, cost);
        }
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }
}

void taskW(void *parameters)
{
    int id = (int)parameters;
    char tag[16];
    sprintf(tag, "TASK_%02d", id);
    int core = xPortGetCoreID();
    while (true)
    {
        uint32_t start = micros();
        logW("[Core %d] Warning: something might be wrong.", core);
        uint32_t end = micros();
        uint32_t cost = end - start;
        if (cost > 1000) {
            ets_printf("\n!! ALERT: Task_%02d on Core %d BLOCKED for %u us !!\n\n", id, core, cost);
        }
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }
}

void taskI(void *parameters)
{
    int id = (int)parameters;
    char tag[16];
    sprintf(tag, "TASK_%02d", id);
    int core = xPortGetCoreID();
    while (true)
    {
        uint32_t start = micros();
        logI("[Core %d] Info: system is running.", core);
        uint32_t end = micros();
        uint32_t cost = end - start;
        if (cost > 1000) {
            ets_printf("\n!! ALERT: Task_%02d on Core %d BLOCKED for %u us !!\n\n", id, core, cost);
        }
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }
}

void taskD(void *parameters)
{
    int id = (int)parameters;
    char tag[16];
    sprintf(tag, "TASK_%02d", id);
    int core = xPortGetCoreID();
    while (true)
    {
        uint32_t start = micros();
        logD("[Core %d] Debug: sensor value = %d", core, random(0, 100));
        uint32_t end = micros();
        uint32_t cost = end - start;
        if (cost > 1000) {
            ets_printf("\n!! ALERT: Task_%02d on Core %d BLOCKED for %u us !!\n\n", id, core, cost);
        }
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }
}

void taskV(void *parameters)
{
    int id = (int)parameters;
    char tag[16];
    sprintf(tag, "TASK_%02d", id);
    int core = xPortGetCoreID();
    while (true)
    {
        uint32_t start = micros();
        logV("[Core %d] Verbose: detailed trace data...", core);
        uint32_t end = micros();
        uint32_t cost = end - start;
        if (cost > 1000) {
            ets_printf("\n!! ALERT: Task_%02d on Core %d BLOCKED for %u us !!\n\n", id, core, cost);
        }
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }
}