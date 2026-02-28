#pragma once
#include <Arduino.h>
#include <esp_log.h>
#include <driver/rmt.h>
// Serial0 (log) Config

const uint32_t Serial0_Baud = 115200;
const uint8_t Serial0_Message_Size = 255;
const uint8_t Serial0_Queue_Size = 20;

// Beta vTaskDelayMicroseconds, for better status, us>=100
inline void DelayUs(uint32_t us)
{
    TaskHandle_t currentTask = xTaskGetCurrentTaskHandle();

    esp_timer_handle_t timer;
    esp_timer_create_args_t timerArgs = {
        .callback = [](void *arg)
        {
            TaskHandle_t handle = (TaskHandle_t)arg;
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            xTaskNotifyGive(handle); 
        },
        .arg = (void *)currentTask,
        .dispatch_method = ESP_TIMER_TASK, 
        .name = "delay_us_task"};
    esp_timer_create(&timerArgs, &timer);
    esp_timer_start_once(timer, us);

    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    esp_timer_delete(timer);
}
