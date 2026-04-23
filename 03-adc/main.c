#include "adc-task/adc-task.h"
#include "hardware/gpio.h"
#include "led-task/led-task.h"
#include "pico/stdlib.h"
#include "protocol-task/protocol-task.h"
#include "stdio-task/stdio-task.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define DEVICE_NAME "my-pico-device"
#define DEVICE_VRSN "v0.0.1"

void help_callback(const char* args);

void version_callback(const char* args) {
    printf("device name: '%s', firmware version: %s\n", DEVICE_NAME,
           DEVICE_VRSN);
}

void led_on_callback(const char* args) {
    printf("LED ON\n\n");
    led_task_state_set(LED_STATE_ON);
}

void led_off_callback(const char* args) {
    printf("LED OFF\n\n");
    led_task_state_set(LED_STATE_OFF);
}

void led_blink_callback(const char* args) {
    printf("LED BLINK\n\n");
    led_task_state_set(LED_STATE_BLINK);
}

void led_blink_set_period_ms_callback(const char* args) {
    uint period_ms = 0;
    sscanf(args, "%u", &period_ms);

    if (period_ms == 0) {
        printf("ERROR: period = 0\n\n");
        return;
    }

    led_task_set_blink_period_ms(period_ms);
}

void mem_callback(const char* args) {
    uint32_t* addr;
    sscanf(args, "%X", &addr);
    printf("%u", *addr);
}

void wmem_callback(const char* args) {
    uint32_t val, *addr;
    sscanf(args, "%X %u", &addr, &val);
    *addr = val;
}

void get_adc_callback(const char* args) {
    float voltage_V = voltage();
    printf("%f\n", voltage_V);
}

void get_temp_callback(const char* args){
    float temp_C = temperature();
    printf("%f\n", temp_C);
}

void tm_start_callback(const char* args){
    printf("TELEMETRY START\n\n");
    adc_task_set_state(ADC_TASK_STATE_RUN);
}

void tm_stop_callback(const char* args){
    printf("TELEMETRY STOP\n\n");
    adc_task_set_state(ADC_TASK_STATE_IDLE);
}

api_t device_api[] = {
    {"version", version_callback, "get device name and firmware version"},
    {"on", led_on_callback, "switch on led"},
    {"off", led_off_callback, "switch off led"},
    {"blink", led_blink_callback, "provide unblocking"},
    {"set_period", led_blink_set_period_ms_callback, "blinking with arguments"},
    {"help", help_callback, "print commands description"},
    {"mem", mem_callback, "read from memory"},
    {"wmem", wmem_callback, "write in memory"},
    {"get_adc", get_adc_callback, "voltage measurement"},
    {"get_temp", get_temp_callback, "temperature measurement"},
    {"tm_start", tm_start_callback, "start telemetry"},
    {"tm_stop", tm_stop_callback, "stop telemetry"},
    {NULL, NULL, NULL},
};

void help_callback(const char* args) {
    int ind = 0;
    while (device_api[ind].command_name != NULL &&
           strcmp(device_api[ind].command_name, "0") != 0) {
        printf("Команда %s : %s \n\n", device_api[ind].command_name,
               device_api[ind].command_help);
        ind++;
    }
}

int main() {
    stdio_init_all();
    stdio_task_init();
    led_task_init();
    protocol_task_init(device_api);
    adc_task_init();
    while (true) {
        protocol_task_handle(stdio_task_handle());
        led_task_handle();
        adc_task_handle();
    }
    return 0;
}