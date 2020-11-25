#include "microfone.h"

/**
 * @brief Function for configuring ADC1
 * @param ch canal a ser configurado
 */
void adc1_config(adc_channel_t ch){
    const adc_atten_t atten = ADC_ATTEN_DB_11; //atenuação equivalente a 
    const adc_unit_t unit = ADC_UNIT_1;

    //Configure ADC
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ch, atten);

    //"Characterize" ADC
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);
}

uint32_t get_voltage(adc_channel_t ch){
    uint32_t adc_reading = adc1_get_raw((adc1_channel_t)ch);//? adc1_channel_t?
    uint32_t voltage_mv = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars); 
    
    printf("Raw: %d\tVoltage: %d mV\n", adc_reading, voltage_mv);
    return voltage_mv;
}

uint32_t get_voltage_variation(adc_channel_t ch){
    uint32_t adc_reading = abs(adc1_get_raw((adc1_channel_t)ch) - MID_SUPPLY);//? adc1_channel_t?
    uint32_t voltage_mv = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars); 
    
    printf("Raw: %d\tVoltage: %d mV\n", adc_reading, voltage_mv);
    return voltage_mv;
}

void ldr_task (void* arg){
    adc1_config(mic_channel);
    printf("max voltage %d\t",esp_adc_cal_raw_to_voltage(4095,adc_chars));
    printf("vref:%d\n",adc_chars->vref);
    //Continuously sample ADC1
    while (1) {

        uint32_t voltage = get_voltage(mic_channel);
        float rldr = (voltage*9800.0)/(3600.0-voltage);
        float lux = 500000.0/rldr;
        printf("Lux: %f\n",lux);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}