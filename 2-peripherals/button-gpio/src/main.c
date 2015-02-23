#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

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

    if (1) {
        // Use BSRRL, set bits of GPIOD pins 12-15

    }
    else {
        // Use BSRRH, set bits of GPIOD pins 12-15

    }
}

void initLeds() {
    // Enable GPIOD Clock

    // Set GPIOD pins 12-15 mode to output

}

void initButton() {
    // Enable GPIOA Clock

    // Set GPIOA pin 0 mode to input

}

