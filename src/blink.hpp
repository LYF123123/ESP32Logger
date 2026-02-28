#pragma once
#include "configuration.h"
#include "logger.hpp"
void taskLedBlink(void *parameters)
{
    pinMode(2, OUTPUT);
    int core = xPortGetCoreID();

    rmt_config_t rmt_cof;
    rmt_cof.rmt_mode = RMT_MODE_TX;
    rmt_cof.channel = RMT_CHANNEL_0;
    rmt_cof.gpio_num = GPIO_NUM_2;
    rmt_cof.mem_block_num = 1;
    rmt_cof.clk_div = 80;                              // 80MHz/80=1Mhz(1us per tick)
    rmt_cof.tx_config.loop_en = false;                 
    rmt_cof.tx_config.carrier_en = false;              
    rmt_cof.tx_config.idle_output_en = true;           
    rmt_cof.tx_config.idle_level = RMT_IDLE_LEVEL_LOW; 

    rmt_config(&rmt_cof);
    rmt_driver_install(RMT_CHANNEL_0, 0, 0);
    logD("[Core %d] Debug: RMT LED Fade driver installed.", core);
    int FADE_TIME_MS = 20;

    int period = 200; // 5kHz 

    for (;;)
    {
        int total_pulses = FADE_TIME_MS * 1000 / period;
        int steps = 100;
        int p_per_s = total_pulses / steps;

        if (p_per_s < 1)
        {
            p_per_s = 1;
            steps = total_pulses; 
        }
        if (steps < 1)
            steps = 1;

        // --- lighting ---
        for (int i = 0; i <= steps; i++)
        {
            uint32_t high = (i * i * period) / (steps * steps);
            if (high == 0 && i > 0)
                high = 1;
            if (high >= period)
                high = period - 1;
            uint32_t low = period - high;

            rmt_item32_t item = {{{(uint16_t)high, 1, (uint16_t)low, 0}}};
            rmt_write_items(RMT_CHANNEL_0, &item, 1, true);
            for (int r = 1; r < p_per_s; r++)
            {
                rmt_write_items(RMT_CHANNEL_0, &item, 1, true);
            }
        }

        // --- Dark ---
        for (int i = steps; i >= 0; i--)
        {
            uint32_t high = (i * i * period) / (steps * steps);
            if (high == 0 && i > 0)
                high = 1;
            uint32_t low = period - high;

            rmt_item32_t item = {{{(uint16_t)high, 1, (uint16_t)low, 0}}};
            rmt_write_items(RMT_CHANNEL_0, &item, 1, true);
            for (int r = 1; r < p_per_s; r++)
            {
                rmt_write_items(RMT_CHANNEL_0, &item, 1, true);
            }
        }
        logD("[Core %d] One breath cycle complete", core);
        vTaskDelay(pdMS_TO_TICKS(1)); 
    }
}