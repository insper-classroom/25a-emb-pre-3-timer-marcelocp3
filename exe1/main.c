#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

const int BOTAO_PINO = 28;
const int LED_PINO =  4;

volatile int flag_f_r = 0;
volatile bool timer_init = false;

void botao_callback(uint gpio, uint32_t eventos) {
    if (eventos == 0x4) {
        flag_f_r = 1;
    } else if (eventos == 0x8) {
    }
}

bool timer_callback(repeating_timer_t *rt) {
    timer_init = true;
    return true;
}

int main(void) {
    bool timer_ativo = false;
    bool led_ligado = false;
    repeating_timer_t meu_timer;

    stdio_init_all();
    gpio_init(LED_PINO);
    gpio_set_dir(LED_PINO, GPIO_OUT);
    gpio_put(LED_PINO, 0);
    gpio_init(BOTAO_PINO);
    gpio_set_dir(BOTAO_PINO, GPIO_IN);
    gpio_pull_up(BOTAO_PINO);
    gpio_set_irq_enabled_with_callback(
        BOTAO_PINO,
        GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE,
        true,
        botao_callback
    );

    while (true) {
        if (flag_f_r) {
            flag_f_r = 0;
            if (!timer_ativo) {
                add_repeating_timer_ms(500, timer_callback, NULL, &meu_timer);
                timer_ativo = true;
            } else {
                cancel_repeating_timer(&meu_timer);
                timer_ativo = false;
                led_ligado = false;
                gpio_put(LED_PINO, led_ligado);
            }
        }
        if (timer_init) {
            timer_init = false;
            led_ligado = !led_ligado;
            gpio_put(LED_PINO, led_ligado);
        }
        sleep_ms(1);
    }
    return 0;
}
