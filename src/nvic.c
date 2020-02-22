#include "global.h"

void EXTI9_5_IRQHandler() {
    if (EXTI->PR & (1 << BUTTON_PIN)) {
        /* clear EXTI status flag */
        EXTI->PR |= (1 << BUTTON_PIN);
        /* toggle global 'state' variable */
        state++;
        if (state == 3)
            state = 0;
    }
}
