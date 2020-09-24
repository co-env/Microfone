/* ADC1 Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#define DEFAULT_VREF    1100        //using espefuse.py tools

static esp_adc_cal_characteristics_t *adc_chars;
static const adc_channel_t channel = ADC_CHANNEL_6;     //GPIO34 if ADC1, GPIO14 if ADC2

static void adc_config(){
    const adc_atten_t atten = ADC_ATTEN_DB_11;
    const adc_unit_t unit = ADC_UNIT_1;

    //Configure ADC
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(channel, atten);

    //Characterize ADC
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);
}

static uint32_t adc_voltage_read(){
    uint32_t adc_reading = 0;
  
    adc_reading = adc1_get_raw((adc1_channel_t)channel);
        
    //Convert adc_reading to voltage in mV
    uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
    
    printf("Raw: %d\tVoltage: %dmV\n", adc_reading, voltage);

    return voltage;
}

void app_main()
{
    adc_config();
        
    //Continuously sample ADC1
    while (1) {
        (void)adc_voltage_read();
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}


