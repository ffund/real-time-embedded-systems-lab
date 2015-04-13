/*
* EL6483 Lab 6
*
* Simple program to light LEDs on STM32F4 Discovery board
* when the User button is pressed.
*
*/

#include "ch.h"
#include "hal.h"

 
void Blink(void) 
{
	palSetPad(GPIOD, GPIOD_LED3);      
	palSetPad(GPIOD, GPIOD_LED4);      
	palSetPad(GPIOD, GPIOD_LED5);      
	palSetPad(GPIOD, GPIOD_LED6);      

}

void Clear(void) 
{
      palClearPad(GPIOD, GPIOD_LED3);      
      palClearPad(GPIOD, GPIOD_LED4);      
      palClearPad(GPIOD, GPIOD_LED5);     
      palClearPad(GPIOD, GPIOD_LED6);     

}
 
int main(void) 
{
 
  /*
   * System initializations.
   * - HAL initialization: this initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization: the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /*
   * Set LED GPIO pins to output mode using the ChibiOS HAL
   * Wondering where these numbers - LED3, LED4, LED5, LED6 - came from?
   * Look closely at your Discovery board and you'll notice that each 
   * of the four colored LEDs are labeled (LD3, LD4, LD5, LD6)
   */
  palSetPadMode(GPIOD, GPIOD_LED3, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOD, GPIOD_LED4, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOD, GPIOD_LED5, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOD, GPIOD_LED6, PAL_MODE_OUTPUT_PUSHPULL);


  /*
   * Normal main() thread activity: poll User button every 10 ms and 
   * call Blink() or Clear() functions based on button status
   */
  while (TRUE) {
    if (palReadPad(GPIOA, GPIOA_BUTTON))
      Blink();
    else
      Clear();
    chThdSleepMilliseconds(10);
  }
}

