#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <driver/gpio.h>

#define MOTOR GPIO_NUM_3
#define MOTOR2 GPIO_NUM_2

void app_main() {

    gpio_set_direction(MOTOR,GPIO_MODE_OUTPUT);
    gpio_set_direction(MOTOR2,GPIO_MODE_OUTPUT);

    while(1)
    {
        gpio_set_level(MOTOR,1);
        gpio_set_level(MOTOR2,0);
        vTaskDelay(100);
        gpio_set_level(MOTOR,0);
        gpio_set_level(MOTOR2,0);
        vTaskDelay(100);
        gpio_set_level(MOTOR,0);
        gpio_set_level(MOTOR2,1);
        vTaskDelay(100);
    }
} 