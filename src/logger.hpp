#pragma once
#include "configuration.h"

static QueueHandle_t qh_log = NULL;

void taskLogger(void *parameters)
{
    qh_log = xQueueCreate(Serial0_Queue_Size, Serial0_Message_Size);
    char msg[Serial0_Message_Size];
    for (;;)
    {
        if (xQueueReceive(qh_log, msg, portMAX_DELAY) == pdPASS)
        {
            ets_printf("%s", msg);
        }
    }
}
#define _internal_log_post(lvl, letter, fmt, ...)                     \
    {                                                                 \
        if (qh_log != NULL)                                           \
        {                                                             \
            char _b[Serial0_Message_Size];                            \
            snprintf(_b, sizeof(_b), ARDUHAL_LOG_FORMAT(letter, fmt), \
                     ##__VA_ARGS__);                                  \
            xQueueSend(qh_log, _b, pdMS_TO_TICKS(0));                 \
        }                                                             \
    }

#define logE(fmt, ...) _internal_log_post(ESP_LOG_ERROR, E, fmt, ##__VA_ARGS__)
#define logW(fmt, ...) _internal_log_post(ESP_LOG_WARN, W, fmt, ##__VA_ARGS__)
#define logI(fmt, ...) _internal_log_post(ESP_LOG_INFO, I, fmt, ##__VA_ARGS__)
#define logD(fmt, ...) _internal_log_post(ESP_LOG_DEBUG, D, fmt, ##__VA_ARGS__)
#define logV(fmt, ...) _internal_log_post(ESP_LOG_VERBOSE, V, fmt, ##__VA_ARGS__)
