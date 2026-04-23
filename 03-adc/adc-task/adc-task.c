#include "adc-task.h"

#include "hardware/adc.h"
#include "pico/stdlib.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

const uint PIN = 26;
const uint CHANNEL = 0;
const uint CHANNEL_TEMP = 4;

adc_task_state_t adc_state = ADC_TASK_STATE_IDLE;

uint64_t adc_ts = 0;
uint ADC_TASK_MEAS_PERIOD_US = 1000000;

void adc_task_init() {
    adc_init();
    adc_gpio_init(PIN);
    adc_set_temp_sensor_enabled(true);
}

float voltage() {
    adc_select_input(CHANNEL);
    uint16_t voltage_counts = adc_read();
    float voltage_V = (float)voltage_counts * (3.3f / 4095.0f);
    return voltage_V;
}

float temperature() {
    adc_select_input(CHANNEL_TEMP);
    uint16_t temp_counts = adc_read();
    float temp_V = (float)temp_counts * (3.3f / 4095.0f);
    float temp_C = 27.0f - (temp_V - 0.706f) / 0.001721f;
    return temp_C;
}

void adc_task_set_state(adc_task_state_t state) { adc_state = state; }

void adc_task_handle() {
    switch (adc_state) {
        case ADC_TASK_STATE_IDLE:
            break;
        case ADC_TASK_STATE_RUN:
            if (time_us_64() > adc_ts) {
                float voltage_V = voltage();
                float temp_C = temperature();
                printf("%f %f\n", voltage_V, temp_C);
                adc_ts = time_us_64() + ADC_TASK_MEAS_PERIOD_US;
            }
            // float voltage_V = voltage();
            // float temp_C = temperature();
            // printf("%f %f\n", voltage_V, temp_C);
            // adc_ts = time_us_64() + (ADC_TASK_MEAS_PERIOD_US / 2);
            // sleep_ms(1000);
            break;
        default:
            break;
    }
}