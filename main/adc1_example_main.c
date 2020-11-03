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

#include "fastmath.h" //para fazer conta com log

#define DEFAULT_VREF    1100        //using espefuse.py tools
#define RPOT            61.4
#define MIC_GAIN        20 * log10((RPOT+22)+1) // ganho final do módulo em dB

static esp_adc_cal_characteristics_t *adc_chars;
static const adc_channel_t mic_channel = ADC_CHANNEL_6;     //GPIO34 if ADC1, GPIO14 if ADC2

const double dBAnalogQuiet = 10; // envelope - calibrated value from analog input (48 dB equivalent)
const double dBAnalogModerate = 12;
const double dBAnalogLoud = 17;

/**
 * @brief Function for configuring ADC1
 * @param ch canal a ser configurado
 */
static void adc1_config(adc_channel_t ch){
    const adc_atten_t atten = ADC_ATTEN_DB_11; //atenuação equivalente a 
    const adc_unit_t unit = ADC_UNIT_1;

    //Configure ADC
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ch, atten);

    //"Characterize" ADC
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);
}

//> check

//!!!!
float get_sound_level(adc1_channel_t ch){
    int32_t adc_reading = adc1_get_raw((adc1_channel_t)ch);//? adc1_channel_t?
    int32_t adc_ac  = adc_reading;  
    
    float decibelsValueQuiet = 49.5;
    float decibelsValueModerate = 65;
    float decibelsValueLoud = 70;
    float valueDb;

    if (adc_reading < 13) {  //quiet
        decibelsValueQuiet += 20 * log10(x/dBAnalogQuiet);
        valueDb = decibelsValueQuiet;
    }
    else if ((adc_reading > 13) && ( adc_reading <= 23) ) { //moderate
        decibelsValueModerate += 20 * log10(x/dBAnalogModerate);
        valueDb = decibelsValueModerate;
    }
    else if(value > 22) { //loud
        decibelsValueLoud += 20 * log10(x/dBAnalogLoud);
        valueDb = decibelsValueLoud;
    }
}

//!!!!
/**
 * @brief Function for reading the ambient sound level 
 * @return sound level in dB
 */
// float get_sound_level(adc1_channel_t ch){
//     int32_t adc_reading = adc1_get_raw((adc1_channel_t)ch);//? adc1_channel_t?
//     int32_t adc_ac  = adc_reading; 
//     if(adc_reading>1940){

//         int32_t voltage_mv = esp_adc_cal_raw_to_voltage(adc_ac, adc_chars) - 1700; 
//         float power = powf(voltage_mv/1000.0,2)/2;
        
//         // float volts_db = 20.0 * log10(voltage_mv);
//         float power_db = 10.0 * log10(power);

//         float spl_db = power_db + 40 + MIC_GAIN;

//         printf("Raw: %d\tSound: %f dB\t sla %f\n", adc_reading, power_db, spl_db);
//         return spl_db;
//     }
//     else 
//     return 0;
// }

uint32_t get_sound_voltage(adc1_channel_t ch){
    uint32_t adc_reading = adc1_get_raw((adc1_channel_t)ch);//? adc1_channel_t?
    uint32_t voltage_mv = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars); 
    
    printf("Raw: %d\tVoltage: %d mV\n", adc_reading, voltage_mv);
    return voltage_mv;
}

void app_main()
{
    adc1_config(mic_channel); //>

    //Continuously sample ADC1
    while (1) {

        //* CALIBRAÇÃO
        // float soma = 0.0; 
        // for(int i=0; i<10; i++){

        //     // soma += get_sound_level(mic_channel);
        //     soma += adc1_get_raw((adc1_channel_t)mic_channel);
        //     vTaskDelay(pdMS_TO_TICKS(100));
        // }
        // printf("media:%.2f\n",soma/10.0);

        //* AMOSTRAGEM
        
        (void)get_sound_level(mic_channel);
        vTaskDelay(pdMS_TO_TICKS(100));

        // !!!
        // (void)get_sound_voltage(mic_channel);
        // vTaskDelay(pdMS_TO_TICKS(100));
    }
}


