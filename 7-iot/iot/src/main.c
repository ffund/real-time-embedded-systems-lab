#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_adc.h"
#include "misc.h"
#include "stm32f4xx_syscfg.h"

#include "rgbled.h"
#include "lightsensor.h"
#include "bluetooth.h"
#include "leds.h"
#include "power.h"
#include "buffer.h"
#include "audioutils.h"

#include <stdio.h>

/* Use flags to keep track of system state. Set flags in ISRs, then check their 
   values and act accordingly in the main while() loop.                        */
int flags = 0x0;

/* Circular buffer used for incoming Bluetooth data */
buffer rxBuffer;

/* Set up other global variables here              */


/**
 * @brief main() implements main system logic
 */
int main() {
    
    initBuffer(&rxBuffer);
    /* Initialize everything else that needs initializing here  */

    while (1) {

    }  

}



/**
 * @brief Interrupt handler for user button (used for wakeup from stop mode)
 * 
 * If you have previously called initWakeupPin(), this handler will be
 * triggered when the User button is pressed (rising edge) or other external 
 * interrupt. 
 */
void EXTI0_IRQHandler(void) {
    if(EXTI_GetITStatus(EXTI_Line0) != RESET){
        EXTI_ClearITPendingBit(EXTI_Line0);
	// do something
     }
} 


/**
 * @brief Interrupt handler for Bluetooth status changes
 * 
 * If you have previously called initBtState(), this handler will be
 * triggered when a Bluetooth connection is made or broken.
 */
void EXTI1_IRQHandler(void) {
    if(EXTI_GetITStatus(EXTI_Line1) != RESET){
      EXTI_ClearITPendingBit(EXTI_Line1);
      // Do something on Bluetooth connection status change
    }
} 


/**
 * @brief Interrupt handler for USART1 (Bluetooth) interrupts
 *
 * If you have previously called initUsart(), this handler will receive 
 * data from the RX port and write it to the circular buffer rxBuffer.
 */
void USART1_IRQHandler(void){
  
  if( USART_GetITStatus(USART1, USART_IT_RXNE) ){
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	write(&rxBuffer, (uint8_t)USART1->DR);
  }
}


/**
 * @brief Interrupt handler for ADC1 interrupts
 *
 * If you have previously called initAdc(), this handler will be called
 * when the light reading is higher than LIGHT_THRESHOLD_HIGH or lower 
 * than LIGHT_THRESHOLD_LOW. 
 */
void ADC_IRQHandler(void) {
    if(ADC_GetITStatus(ADC1, ADC_IT_AWD) != RESET) {
     ADC_ClearITPendingBit(ADC1, ADC_IT_AWD);
     // do something
    }
}
