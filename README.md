# blinky

A bare-metal (no HAL/LL drivers) example of blinking an LED on an STM32 microcontroller using an external interrupt.

This code was written for an STM32F334R8, but it will likely work for any STM32F3 (with adjusted device header files). It will simply blink the on-board LED at a certain speed based on the number of times an external button (see button pin '#define' in 'global.h') has been pressed.

Much of this code was adapted from Vivonomicon's STM32 bare metal tutorials: https://vivonomicon.com/category/stm32_baremetal_examples/
