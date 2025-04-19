#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "inc/ssd1306.h"

#define I2C_SDA_PIN     14
#define I2C_SCL_PIN     15
#define BT_A_PIN     5
#define BT_B_PIN     6

// Contador e flags 
volatile int  contador_regressivo  = 0;    
volatile int  cliques_b = 0;    
volatile bool contagem = false;
volatile bool atualizar_tela = false;

struct render_area area_renderizacao;
uint8_t ssd[ssd1306_buffer_length];
static repeating_timer_t temporizador;  

// Timer callback
bool callback_temporizador(repeating_timer_t *rt) {
    if (contagem && contador_regressivo > 0) {
        contador_regressivo--;
        atualizar_tela = true;
        if (contador_regressivo == 0) {
            contagem = false;
            // desabilita interrupções do botão B fora do modo ativo
            gpio_set_irq_enabled(BT_B_PIN, GPIO_IRQ_EDGE_FALL, false);
        }
    }
    return true; // continuar chamando
}

// GPIO callback para A e B
void callback_gpio(uint gpio, uint32_t events) {
    if ((events & GPIO_IRQ_EDGE_FALL) == 0) return;
    printf("GPIO %u @ %u us\n", gpio, time_us_32());

    if (gpio == BT_A_PIN) {
        // reinicia a contagem
        contador_regressivo = 9;
        cliques_b    = 0;
        contagem     = true;
        atualizar_tela     = true;
        // garante que B volte a gerar interrupções
        gpio_set_irq_enabled(BT_B_PIN, GPIO_IRQ_EDGE_FALL, true);
    }
    else if (gpio == BT_B_PIN) {
        if (contagem) {
            cliques_b++;
            atualizar_tela = true;
        }
    }
}

int main(void) {
    stdio_init_all();

    // Inicialização do I2C1 para SSD1306
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    // Inicializa display
    ssd1306_init();
    area_renderizacao.start_column = 0;
    area_renderizacao.end_column   = ssd1306_width - 1;
    area_renderizacao.start_page   = 0;
    area_renderizacao.end_page     = ssd1306_n_pages - 1;
    calculate_render_area_buffer_length(&area_renderizacao);
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &area_renderizacao);

    // Configura botões com pull‑up
    gpio_init(BT_A_PIN);
    gpio_set_dir(BT_A_PIN, GPIO_IN);
    gpio_pull_up(BT_A_PIN);

    gpio_init(BT_B_PIN);
    gpio_set_dir(BT_B_PIN, GPIO_IN);
    gpio_pull_up(BT_B_PIN);

    // Registra interrupções
    gpio_set_irq_enabled_with_callback(BT_A_PIN, GPIO_IRQ_EDGE_FALL, true, &callback_gpio);
    // B usará o mesmo callback
    gpio_set_irq_enabled(BT_B_PIN, GPIO_IRQ_EDGE_FALL, true);

    // Inicia timer repetidor em 1s
    add_repeating_timer_ms(1000, callback_temporizador, NULL, &temporizador);

    // Loop principal
    char linha[32];
    while (true) {
        if (atualizar_tela) {
            atualizar_tela = false;
            memset(ssd, 0, ssd1306_buffer_length);

            sprintf(linha, "Contador: %d", contador_regressivo);
            ssd1306_draw_string(ssd, 5, 0, linha);

            sprintf(linha, "Cliques: %d", cliques_b);
            ssd1306_draw_string(ssd, 5, 8, linha);

            render_on_display(ssd, &area_renderizacao);
        }
        sleep_ms(50);
    }

    return 0;
}