#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_adc.h"
#include "misc.h"
#include "stm32f4xx_syscfg.h"

#include "lightsensor.h"


/**
 * @brief Set up analog input and ADC to read values from light sensor
 *
 * This function configures GPIO PC0 as an analog input connected to an 
 * ADC. It also enables an analog watchdog interrupt that fires each time the 
 * digital value is less then LIGHT_THRESHOLD_LOW or higher than LIGHT_THRESHOLD_HIGH.
 * 
 * (LIGHT_THRESHOLD_LOW and LIGHT_THRESHOLD_HIGH are defined in lightsensor.h)
 * 
 * You must define an interrupt handler (ADC_IRQHandler) to handle
 * these interrupts.
 */
void initAdc() {
	GPIO_InitTypeDef GPIO_initStructre; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);

	RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOCEN,ENABLE); 

	GPIO_initStructre.GPIO_Pin = GPIO_Pin_0;
	GPIO_initStructre.GPIO_Mode = GPIO_Mode_AN;
	GPIO_initStructre.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC,&GPIO_initStructre);
	
	/* Common ADC Initialization           */
	ADC_CommonInitTypeDef adc_common;
	ADC_CommonStructInit(&adc_common);
	adc_common.ADC_Prescaler = ADC_Prescaler_Div8; 
	ADC_CommonInit(&adc_common);

	/* ADC Initialization           */
	ADC_InitTypeDef adc;
	ADC_StructInit(&adc);  
	adc.ADC_Resolution = ADC_Resolution_12b;
        adc.ADC_ContinuousConvMode = ENABLE; 

	ADC_Init(ADC1, &adc);

	ADC_Cmd(ADC1,ENABLE);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_10,1,ADC_SampleTime_480Cycles);

	/* Use an analog watchdog to trigger interrupt on given thresholds      */
        ADC_AnalogWatchdogThresholdsConfig(ADC1, LIGHT_THRESHOLD_HIGH, LIGHT_THRESHOLD_LOW);
        ADC_AnalogWatchdogSingleChannelConfig(ADC1, ADC_Channel_10);
        ADC_ClearFlag(ADC1, ADC_FLAG_AWD);
        ADC_AnalogWatchdogCmd(ADC1, ADC_AnalogWatchdog_SingleRegEnable);
	ADC_ITConfig(ADC1, ADC_IT_AWD, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure and enable ADC interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	ADC_SoftwareStartConv(ADC1);
}

/**
 * @brief Change thresholds for light sensor readings
 *
 * This function reconfigures the analog watchdog thresholds.
 */
void setAdcThresholds(uint32_t high, uint32_t low){

        ADC_AnalogWatchdogThresholdsConfig(ADC1, high, low);
        ADC_AnalogWatchdogSingleChannelConfig(ADC1, ADC_Channel_10);
        ADC_ClearFlag(ADC1, ADC_FLAG_AWD);
        ADC_AnalogWatchdogCmd(ADC1, ADC_AnalogWatchdog_SingleRegEnable);
	ADC_ITConfig(ADC1, ADC_IT_AWD, ENABLE);

}



/**
 * @brief Get current light reading (digital value) from light sensor
 *
 * This function polls the ADC to get the current light sensor reading
 * as a digital value.
 */
int getLightReading(){
	ADC_SoftwareStartConv(ADC1);
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
	return ADC_GetConversionValue(ADC1); 
}
