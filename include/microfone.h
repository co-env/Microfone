#ifndef __MICROFONE_H__
#define __MICROFONE_H__

#include <stdio.h>
#include <stdlib.h>

#include "fastmath.h" //para fazer conta com log
#include <esp_types.h>

#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

/** Macros para os cálculos **/
#define MID_SUPPLY  1650
#define DEFAULT_VREF    1100        //using espefuse.py tools

/** ADC **/
esp_adc_cal_characteristics_t *adc_chars;

void adc1_config(adc_channel_t ch);

uint32_t get_voltage(adc_channel_t ch);

uint32_t get_voltage_variation(adc_channel_t ch);

float get_noise_level_db(adc_channel_t ch);

// void ldr_task (void* arg);

/**
 * @brief Function for reading the ambient sound level 
 * @return sound level in dB
 */
// uint32_t get_sound_level(adc_channel_t ch);

/*@*/

#endif  // __MICROFONE_H__