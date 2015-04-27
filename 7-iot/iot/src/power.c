#include "stm32f4xx.h"
#include "stm32f4xx_pwr.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_usart.h"
#include "misc.h"
#include "stm32f4xx_syscfg.h"

#define USER_BUTTON GPIO_Pin_0

/**
 * @brief Sets up User button to wake system from STOP mode
 *
 * This function configures the User button (PA0) as a digital 
 * input that triggers an interrupt, which may be used to wake the 
 * system from STOP mode.
 * 
 * You must define an interrupt handler (EXTI0_IRQHandler) to handle
 * this interrupt.
 */
void initWakeupPin() {

    /* Copy in your initButton() function from the `interrupt-button' 
       project in lab 5.                                             */

    PWR_WakeUpPinCmd(ENABLE);                         
}


/**
 * @brief Put system in sleep mode.
 *
 * When the system is in sleep mode, only the Cortex-M4 core is turned off. 
 * Any interrupt that is acknowledged by the NVIC can wake the system from sleep.
 */
void sleep() {

	__WFI();  

}

/**
 * @brief Put system in stop mode.
 *
 * In stop mode, the Cortex-M4 and peripheral clocks are switched off, but volatile
 * memory is preserved.
 *
 * When the system is in stop mode, it may be woken by a rising edge on the wakeup 
 * pin (PA0, connected to User button). 
 * 
 * After waking from stop mode, the HSE is disabled and you may need to call 
 * SystemInit() again to reconfigure your clocks. 
 * 
 * Also, after stop mode, you may need to reset your board before you can flash 
 * code onto it again. In some cases, you may need to reset the board by shorting
 * BOOT0 and VDD and erasing the flash on the board.
 *
 */
void stop() {

	PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);

}
