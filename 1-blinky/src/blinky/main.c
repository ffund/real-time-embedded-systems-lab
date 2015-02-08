#ifndef USE_STDPERIPH_DRIVER
#define USE_STDPERIPH_DRIVER
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_fsmc.h"
#endif

#include <stdio.h>

static void initSystick();
static void delayMillis(uint32_t timedelay);
static volatile uint32_t millisecondCounter;


void initSystick() {
  millisecondCounter=0;
  SysTick_Config(SystemCoreClock/1000);
}
 
void delayMillis(uint32_t millis) {
  uint32_t target;
 
  target=millisecondCounter+millis;
  while(millisecondCounter<target)
    ;
}
 
void SysTick_Handler(void) {
    millisecondCounter++;
 }


int main(void)
{
    initSystick();
    unsigned int pin = 13;		  // pin 13 is the orange LED
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;  // enable the clock to GPIOD
    GPIOD->MODER = (1 << 2*pin);          // set pin 13 to be general purpose output

    while (1) {

	       delayMillis(1000);

	       GPIOD->ODR ^= (1 << pin);           // Toggle pin 
	       setbuf(stdout, NULL);
	       printf("Toggling LED state\r\n");

    }
}




