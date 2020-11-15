/* ADC1 Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "microfone.h"

// #define RPOT            61.4
// #define MIC_GAIN        20 * log10((RPOT+22)+1) // ganho final do módulo em dB

const adc_channel_t mic_channel = ADC_CHANNEL_6;     //GPIO34 in ADC1

// const double dBAnalogQuiet = 10; // envelope - calibrated value from analog input (48 dB equivalent)
// const double dBAnalogModerate = 12;
// const double dBAnalogLoud = 17;

static void sound_sensor_task(void *arg) {
    ESP_LOGI(TAG, "Mic main task initializing...");

    if (xSemaphoreTake(xSemaphore, portMAX_DELAY ) == pdTRUE ) {
        adc1_config(mic_channel);
        xSemaphoreGive(xSemaphore);
    }

    while(1) {
        (void)get_voltage_variation(mic_channel);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void app_main() {
    xTaskCreate(sound_sensor_task, "sound_sensor_main_task", 1024 * 2, (void *)0, 15, NULL);
}


