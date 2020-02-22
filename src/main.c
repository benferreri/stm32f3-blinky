#include "global.h"
#include "main.h"

/* globals initialization */
volatile unsigned char state = 0;

/* rudimentary sleep function, not 100% accurate */
void sleep(int msecs) {
    #define STEPS_PER_MSEC 720
    unsigned int i,s;
    for (s=0; s < msecs; s++) {
        for (i = 0; i < STEPS_PER_MSEC; i++) {
            // skip CPU cycle
            asm("nop");
        }
    }
}

/* turn on led, wait, turn off led */
void flash_led(int msecs) {
        GPIOA->ODR |= (1 << LED_PIN);
        sleep(msecs);
        GPIOA->ODR &= ~(1 << LED_PIN);
}
    
int main(void) {
    /* enable GPIOA/B peripherals in 'RCC_AHBENR' */
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    
    /* enable SYSCFG peripheral */
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    /* set SYSCFG to connect button EXTI line to GPIOB (0x1 -> B) */
    SYSCFG->EXTICR[(BUTTON_PIN/4)] &= ~(0xF << ((BUTTON_PIN % 4) * 4));
    SYSCFG->EXTICR[(BUTTON_PIN/4)] |=  (0x1 << ((BUTTON_PIN % 4) * 4));
    /* setup button's EXTI line as an interrupt */
    EXTI->IMR  |=  (1 << BUTTON_PIN);
    /* disable rising edge interrupt trigger (button release) */
    EXTI->RTSR &= ~(1 << BUTTON_PIN);
    /* enable falling edge interrupt trigger (button press) */
    EXTI->FTSR |=  (1 << BUTTON_PIN);

    /* enable NVIC interrupt for EXTI5 - EXTI9 (button pin in range [5,9]) */
    NVIC_SetPriority(EXTI9_5_IRQn, 0xF);        // lowest priority
    NVIC_EnableIRQ(EXTI9_5_IRQn);
    
    /* initialize GPIO pins */
    /* button */
    GPIOB->MODER &= ~(0x3 << (BUTTON_PIN*2));
    GPIOB->PUPDR &= ~(0x3 << (BUTTON_PIN*2));
    GPIOB->PUPDR |=  (0x1 << (BUTTON_PIN*2));
    /* LED */
    GPIOA->MODER  &= ~(0x3 << (LED_PIN*2));
    GPIOA->MODER  |=  (0x1 << (LED_PIN*2));
    GPIOA->OTYPER &= ~(1 << LED_PIN);
    
    while (1) {
        flash_led(100 * state);
        sleep(100 * state);
    }
}
