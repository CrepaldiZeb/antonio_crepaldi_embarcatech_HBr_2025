#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

int main(void){
    stdio_init_all();

    //  ADC
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    // Super loop
    while(1){
        uint16_t read = adc_read();
        const float fator_de_conversao = 3.3f / (1<<12);
        float result = read * fator_de_conversao;
        float temp = 27 - (result -0.706)/0.001721;
        printf("Temperatura = %.1f C\n", temp);
        sleep_ms(1000);
    }
}