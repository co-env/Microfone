#include "microfone.h"

#define MIC_SAMPLES (1024 * 2)

#define AMP_MAX 1650
#define QUIET_LEVEL 120.0  //! foi o valor de AMP medido quando tá silencio, usado na conta de log10


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
    
    return voltage_mv;
}

uint32_t get_voltage_variation(adc_channel_t ch){
    uint32_t adc_reading = abs(adc1_get_raw((adc1_channel_t)ch) - MID_SUPPLY);//? adc1_channel_t?
    uint32_t voltage_mv = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars); 
    
    return voltage_mv;
}

float get_noise_level_db(adc_channel_t ch) {
    uint32_t sound_max = 0;
    uint32_t sound_min = 4096;
    uint32_t sound_vol_avg = 0;
    uint32_t sound_vol_rms = 0;

    uint32_t amp = 0;

    for (int i = 0; i < MIC_SAMPLES; i++) {
        uint32_t k = adc1_get_raw(ch);
        // uint32_t amp = abs(k - AMP_MAX);

        sound_max = (k > sound_max) ? k : sound_max;
        sound_min = (k < sound_min) ? k : sound_min;
        amp = sound_max - sound_min;
        sound_vol_avg += amp;
        sound_vol_rms += (uint32_t)(amp * amp);
    }

    sound_vol_avg /= MIC_SAMPLES;
    sound_vol_rms /= MIC_SAMPLES;

    float sound_vol_rms_float = sqrt(sound_vol_rms);
    float sound_max_float = (float)sound_max;

    // float db = 20.0 * log10(sound_vol_rms_float / 200.0);
    // float db = 20.0 * log10((float)sound_vol_rms / 82000.0);
    // float db = 20.0 * log10(sound_max / 400.0);
    // float db = 10 * (sound_max / 416.0);
    float db = (amp > 0) ? (20.0 * log10((float)amp / QUIET_LEVEL)) : 40.0;

    db += 40.0;

    // ESP_LOGI(TAG, " %3f   %3u   %3u   %3u   %3u   %3u   %3f", db, sound_max, sound_min, amp, sound_vol_avg, sound_vol_rms, sound_vol_rms_float);

    return db;
}

// void ldr_task (void* arg){
//     adc1_config(mic_channel);
//     printf("max voltage %d\t",esp_adc_cal_raw_to_voltage(4095,adc_chars));
//     printf("vref:%d\n",adc_chars->vref);
//     //Continuously sample ADC1
//     while (1) {

//         uint32_t voltage = get_voltage(mic_channel);
//         float rldr = (voltage*9800.0)/(3600.0-voltage);
//         float lux = 500000.0/rldr;
//         printf("Lux: %f\n",lux);
//         vTaskDelay(pdMS_TO_TICKS(100));
//     }
// }