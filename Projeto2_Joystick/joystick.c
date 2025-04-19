#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

int main() {
    stdio_init_all();
    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);

    while (1) {
        adc_select_input(0);
        uint adc_y_raw = adc_read();
        adc_select_input(1);
        uint adc_x_raw = adc_read();

        printf("\nValor em X: %d Valor em Y: %d",adc_x_raw, adc_y_raw );
        sleep_ms(100);
    }
}