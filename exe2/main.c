#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int BTN_PIN_G = 26;
const int LED_PIN_R = 4;
const int LED_PIN_G = 6;

volatile int flag_r = 0;
volatile int flag_g = 0;

void btn_callback(uint gpio, uint32_t eventos) {
    if (eventos == 0x4) {
        if (gpio == BTN_PIN_R)
            flag_r = 1;
        else if (gpio == BTN_PIN_G)
            flag_g = 1;
    }
}

int main() {
    stdio_init_all();
    
    gpio_init(LED_PIN_R);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);
    gpio_put(LED_PIN_R, 0);
    
    gpio_init(LED_PIN_G);
    gpio_set_dir(LED_PIN_G, GPIO_OUT);
    gpio_put(LED_PIN_G, 0);
    
    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);
    
    gpio_init(BTN_PIN_G);
    gpio_set_dir(BTN_PIN_G, GPIO_IN);
    gpio_pull_up(BTN_PIN_G);
    
    gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true, btn_callback);
    gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_FALL, true);
    
    bool timer_r = false;
    bool timer_g = false;
    bool led_r = false;
    bool led_g = false;
    int contador_r = 0;
    int contador_g = 0;
    
    while (true) {
        if (flag_r) {
            flag_r = 0;
            if (!timer_r) {
                timer_r = true;
                led_r = false;
                gpio_put(LED_PIN_R, led_r);
                contador_r = 0;
            } else {
                timer_r = false;
                led_r = false;
                gpio_put(LED_PIN_R, led_r);
                contador_r = 0;
            }
        }
        if (flag_g) {
            flag_g = 0;
            if (!timer_g) {
                timer_g = true;
                led_g = false;
                gpio_put(LED_PIN_G, led_g);
                contador_g = 0;
            } else {
                timer_g = false;
                led_g = false;
                gpio_put(LED_PIN_G, led_g);
                contador_g = 0;
            }
        }
        if (timer_r) {
            contador_r++;
            if (contador_r >= 500) {
                led_r = !led_r;
                gpio_put(LED_PIN_R, led_r);
                contador_r = 0;
            }
        }
        if (timer_g) {
            contador_g++;
            if (contador_g >= 250) {
                led_g = !led_g;
                gpio_put(LED_PIN_G, led_g);
                contador_g = 0;
            }
        }
        sleep_ms(1);
    }
    
    return 0;
}
