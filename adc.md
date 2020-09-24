ADC
====

Remover o jumper no pino 34 (ADC1_CH6) ao gravar

Funcionality
----
- ADC1 (8 channels, attached to GPIOs 32 - 39)
- ADC2 used within wi-fi module

unit work modes
- ADC-RTC 
- ADC-DMA mode

### ADC-RTC 

configured before reading
- ` adc1_config_width()` precision 
    > max resolution 12bits)
- `adc1_config_channel_atten()` attenuation

    > **attenuation**: This is a voltage scaling factor. By default, the allowable input range is 0-1V but with different attenuations we can scale the input voltage into this range. The available scales beyond the 0-1V 
    > *include 0-1.34V, 0-2V and 0-3.6V*.

  (tipo avr/arduino)

- `adc1_get_raw()` conversion result

- `adc_vref_to_gpio()` used to route internal reference voltage to a GPIO pin
  > calibrate ADC reading 

### Minimizing Noise 

Higher precision approach. 

> connect a 0.1uF capacitor to the ADC input pad in use

#### Calibration

> The `esp_adc_cal/include/esp_adc_cal.h` API provides functions to correct for differences in measured voltages caused by variation of ADC reference voltages (Vref) between chips

> characterizing one of the ADCs at a given attenuation to obtain a characteristics curve (ADC-Voltage curve) that takes into account the difference in ADC reference voltage. The characteristics curve is in the form of `y = coeff_a * x + coeff_b` and is used to convert ADC readings to voltages in mV
> 
> calibrations values stored in eFuse or provided by the user

Three sources of calibration values
- **Two Point** values represent each of the ADCs’ readings at 150 mV and 850 mV. To obtain more accurate calibration results these values should be measured by user and burned into eFuse BLOCK3.

- **eFuse Vref** represents the true ADC reference voltage. This value is measured and burned into eFuse BLOCK0 during factory calibration.

- **Default Vref** is an estimate of the ADC reference voltage provided by the user as a parameter during characterization. If Two Point or eFuse Vref values are unavailable, Default Vref will be used.