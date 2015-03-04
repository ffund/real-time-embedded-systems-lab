#include <stdio.h>

#include "stm32f4xx.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_rcc.h"


void initTempSensor(void);
float readTemp(void);

volatile uint32_t msTicks = 0;                      

// SysTick Handler (ISR for the System Tick interrupt)
void SysTick_Handler(void){
  msTicks++;
}

// Initialize the system tick 
void InitSystick(void){
	SystemCoreClockUpdate();                      
	if (SysTick_Config(SystemCoreClock / 1000)) { 
		while (1);                             
	}
}

int main(void){

	SystemInit();
	InitSystick();
	initTempSensor();

	while(1)                             
	{
		readTemp();
	}

	return 0;
}

void initTempSensor(){

	// In this function, we'll set up the ADC and use 
	// it to read values from an on-board temperature sensor.

        // Refer to the ADC library header and source files
        // (stm32f4xx_adc.h and stm32f4xx_adc.c) for more details on this code.

	// Declare adc as a ADC_InitTypeDef, adc_common as a ADC_CommonInitTypeDef
	ADC_InitTypeDef adc;
	ADC_CommonInitTypeDef adc_common;
	
	// Enable clock signal to ADC peripheral
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	
	// Common ADC Initialization
	ADC_CommonStructInit(&adc_common); // initialize struct to default values
        // Change a few values
	adc_common.ADC_Prescaler = ADC_Prescaler_Div8; // sets ADC clock prescaler
	ADC_CommonInit(&adc_common);

        // ADC initialization
	ADC_StructInit(&adc);  	// initialize struct to default values

	// TODO: explicitly set some values before initialization
	// Set the ADC resolution to 12 bits
	// and enable continuous conversion mode





	ADC_Init(ADC1, &adc);
	
	// ADC Channel 1 configuration
	ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 1, ADC_SampleTime_144Cycles);
	
	// Enable temperature smensor
	ADC_TempSensorVrefintCmd(ENABLE);

	// Enable ADC
	ADC_Cmd(ADC1, ENABLE);
}


float readTemp(){
	float temperature;
	
	ADC_SoftwareStartConv(ADC1);				// Start the conversion
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); // Wait for conversion to finish
	temperature = (float) ADC_GetConversionValue(ADC1); 			// Get ADC reading

	// Print ADC reading
	setbuf(stdout, NULL);
        printf("%f, " , temperature);
	
	// TODO: Convert ADC (digital) reading back to voltage value
        // Use the formula on page 20 of the lecture slides
        // Here, v_min = 0, v_max = 3.3, and n depends on the resolution
        // of the ADC (refer to the adc intialization in initTempSensor() function)
	// Assign the voltage value back to the temperature variable





	setbuf(stdout, NULL);
        printf("%f, " , temperature);
	
	// TODO: Convert the digital value to a temperature and assign back
	// to the temperature value.
	// The formula for this conversion is given in the Technical Reference Manual
        // (v_sense is the voltage value we calculated in the previous step 
	// and assigned back to temp)
	// Temperature (in °C) = {(V_SENSE - V_25) / Avg_Slope} + 25



	setbuf(stdout, NULL);
        printf("%f\n" , temperature);
	
	return temperature;
}
