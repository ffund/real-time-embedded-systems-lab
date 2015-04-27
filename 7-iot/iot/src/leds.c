#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#define LEDS  GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15


/**
 * @brief Set up on-board LEDs as GPIO outputs
 *
 * This function initializes the GPIO output pins connected 
 * to the four on-board LEDs.
 */
void initLeds() {

  // Copy this function in from lab 2

}
