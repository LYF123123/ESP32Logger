#pragma once
#include "logger.hpp"

void taskMemoryStatus(void *parameters)
{
    int core = xPortGetCoreID();
    for (;;)
    {
        logD("\n[Core %d] --- 🛠️  ESP32 Memory Status ---", core);
        size_t free_heap = esp_get_free_heap_size();
        size_t min_free = esp_get_minimum_free_heap_size();
        size_t max_block = heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);
        size_t psram_free = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
        
        logD("Free Heap Size: %d Byte (%.2f KB)", free_heap, free_heap / 1024.0);
        logD("Minimum Free Heap Size: %d Byte (%.2f KB)", min_free, min_free / 1024.0);
        logD("Largest Free Block: %d Byte (%.2f KB)", max_block, max_block / 1024.0);

        if (psram_free > 0)
        {
            logD("PSRAM : %d Byte (%.2f KB)\n", psram_free, psram_free / 1024.0);
        }

        if (max_block < 1024 * 4)
        {
            logD("⚠️  Warning: Memory fragmentation is severe or nearing exhaustion!");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}