#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"


/**
 * @brief Sets up three PWM channels for controlling RGB LED
 *
 * This function sets up three PWM channels on Timer 3, which 
 * may then be used to produce colors of various intensity 
 * on three channels (red, green, and blue).
 * 
 * The output channels will be on PC6, PC8, and PC9
 * 
 * Refer to `timing-pwm` project in lab 5 for more details.
 */
void initPwm(void) {

	GPIO_InitTypeDef gpio_C; 
	TIM_TimeBaseInitTypeDef TIM_TimeBase; 

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

	gpio_C.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_9 | GPIO_Pin_8;  
	gpio_C.GPIO_Mode = GPIO_Mode_AF; 
	gpio_C.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_C.GPIO_OType = GPIO_OType_PP; 
	gpio_C.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOC, &gpio_C);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3); 
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3); 
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3); 

	/* Sets frequency to 1 MHz   */
	TIM_TimeBase.TIM_Prescaler = 84 - 1; 
	TIM_TimeBase.TIM_CounterMode = TIM_CounterMode_Up; 
	/* Set period to 255, so that we control duty cycle by writing 
	   a uint8_t value to the CCR registers                         */
	TIM_TimeBase.TIM_Period = 255 - 1;
	TIM_TimeBase.TIM_ClockDivision = 0;
	TIM_TimeBase.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBase); 


	TIM_Cmd(TIM3, ENABLE); 

	/* Configure output channel 1   */
	TIM_OCInitTypeDef TIM3_OC1; 
	TIM3_OC1.TIM_OCMode = TIM_OCMode_PWM1;
	TIM3_OC1.TIM_OutputState = TIM_OutputState_Enable; 
	TIM3_OC1.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM3_OC1.TIM_Pulse = 10; 
	TIM_OC1Init(TIM3, &TIM3_OC1); 
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable); 


	/* Configure output channel 4   */
	TIM_OCInitTypeDef TIM3_OC4; 
	TIM3_OC4.TIM_OCMode = TIM_OCMode_PWM1;
	TIM3_OC4.TIM_OutputState = TIM_OutputState_Enable; 
	TIM3_OC4.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM3_OC4.TIM_Pulse = 10; 
	TIM_OC4Init(TIM3, &TIM3_OC4); 
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Disable); 


	/* Configure output channel 3   */
	TIM_OCInitTypeDef TIM3_OC3; 
	TIM3_OC3.TIM_OCMode = TIM_OCMode_PWM1;
	TIM3_OC3.TIM_OutputState = TIM_OutputState_Enable; 
	TIM3_OC3.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM3_OC3.TIM_Pulse = 10; 
	TIM_OC3Init(TIM3, &TIM3_OC3);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Disable); 

}



/**
 * @brief Set color of RGB LED by changing duty cycle of PWM outputs
 *
 * This function sets the color of the RGB LED by changing the duty 
 * cycle of the three PWM outputs (red, green, blue), thereby changing
 * the color mixture.
 */
void setColor(uint8_t red, uint8_t green, uint8_t blue) {

  TIM3->CCR1 = red;
  TIM3->CCR4 = green;
  TIM3->CCR3 = blue;

}
