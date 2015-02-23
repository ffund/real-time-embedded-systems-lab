#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

const uint16_t LEDS = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
const uint16_t USER_BUTTON = GPIO_Pin_0;

void init();
void loop();

void initLeds();
void initButton();

int main() {
    init();

    do {
        loop();
    } while (1);
}

void init() {
    initLeds();
    initButton();
}

void loop() {

    // Replace the '1' in the next line with the current User button 
    // input status; use the GPIO_ReadInputDataBit function
    if (1) {

        // Use the GPIO_SetBits function to turn on the LEDs


    }
    else {
        // Use the GPIO_ResetBits function to turn on the LEDs


    }
}

void initLeds() {
    // Use RCC_AHB1PeriphClockCmd to enable the clock on GPIOD


    // Declare a gpio as a GPIO_InitTypeDef:
    GPIO_InitTypeDef gpio;

    // Call GPIO_StructInit, passing a pointer to gpio
    // This resets the GPIO port to its default values


    // Before calling GPIO_Init, 
    // Use gpio.GPIO_Pin to set the pins you are interested in
    // (you can use the LEDS constant defined above)

    // Use gpio.GPIO_Mode to set these pins to output mode 

    // Now call GPIO_Init with the correct arguments


}

void initButton() {
    // Use the RCC_AHB1PeriphClockCmd to enable the clock on GPIOA

    // Declare a gpio as a GPIO_InitTypeDef:
    GPIO_InitTypeDef gpio;

    // Call GPIO_StructInit, passing a pointer to gpio
    // This resets the GPIO port to its default values

    // Before calling GPIO_Init, 
    // Use gpio.GPIO_Pin to set the pins you are interested in
    // (you can use the USER_BUTTON constant defined above)

    // Use gpio.GPIO_Mode to set this pins to input mode 

    // Now call GPIO_Init with the correct arguments

}

