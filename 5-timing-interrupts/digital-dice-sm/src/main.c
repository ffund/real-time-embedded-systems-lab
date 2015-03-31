#include <stdio.h>

#include "main.h"
#include "stm32f4xx.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_rng.h"
#include "misc.h"
#include "stm32f4xx_syscfg.h"


#define ABS(x) ((x)<0 ? -(x) : (x))

#define BOUNCE_DELAY 0
#define THRESHOLD    0

#define STARTING 00
#define WAITING  01
#define SHAKING  10
#define DISPLAY  11

uint8_t state = STARTING;

/* The states are:
STARTING – during initialization
WAITING – waiting for first shake
SHAKING – while shaking the board
DISPLAYING – the result is being displayed on the LEDs
*/

const uint16_t LEDS = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;

uint32_t bounceCounter = 0;

static void initSystick();
void initAccelerometer();
void initLeds();
void initRng();

void readAxes();
uint8_t readSPI(uint8_t);
void writeSPI(uint8_t, uint8_t);

int8_t isShaking();

void showRandomNumber();

typedef struct {
	int16_t X;
	int16_t Y;
	int16_t Z;
} AccelerometerDataStruct;

AccelerometerDataStruct dat;
AccelerometerDataStruct datOld;

void initSystick() {
	// You can change the frequency of the SysTick interrupt if you want
	SysTick_Config(SystemCoreClock/100);
}
 
void SysTick_Handler(void) { 

    switch(state) {
    case STARTING:
        // Fill in any necessary actions here
        break;
    case WAITING: 
        // Fill in any necessary actions here
        break;
    case DISPLAY:
        // Fill in any necessary actions here
        break;
    case SHAKING:
        // Fill in any necessary actions here
        break;
    default:
	state = WAITING;
	break;
    }

}
 
int main() {
	SystemInit();

	initSystick();
	initRng();
	initAccelerometer();
	initLeds();

	state = WAITING;

	while(1);
}


void initRng() {
	
   RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);
   RNG_Cmd(ENABLE);

}

void initAccelerometer() {

	// Initialize SPI1 clock and data lines for slave (accelerometer)

	// Enable clock signal to SPI1 and GPIOA peripherals
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);


	// Configure GPIO pins for SPI1 clock and data lines
 	GPIO_InitTypeDef gpio_spi1;
	gpio_spi1.GPIO_OType = GPIO_OType_PP;
	gpio_spi1.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio_spi1.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_spi1.GPIO_Mode = GPIO_Mode_AF;
	gpio_spi1.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOA, &gpio_spi1);

	// Configure GPIO pins to use alternate function for SPI1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

	// Configure SPI peripheral (master)
	SPI_InitTypeDef spi;

	SPI_StructInit(&spi);
	spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	spi.SPI_DataSize = SPI_DataSize_8b;
	spi.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	spi.SPI_FirstBit = SPI_FirstBit_MSB;
	spi.SPI_Mode = SPI_Mode_Master;
	spi.SPI_CPOL = SPI_CPOL_High;
	spi.SPI_CPHA = SPI_CPHA_2Edge;
	spi.SPI_NSS = SPI_NSS_Soft;

	SPI_Init(SPI1, &spi);
	SPI_Cmd(SPI1, ENABLE);

	// Set up SPI chip select/slave select line
	GPIO_InitTypeDef gpio_cs;
	
	// Enable clock for CS GPIO port
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 
	
	// Configure CS pin
	gpio_cs.GPIO_Pin = GPIO_Pin_3;
	gpio_cs.GPIO_Mode = GPIO_Mode_OUT;
	gpio_cs.GPIO_OType = GPIO_OType_PP;
	gpio_cs.GPIO_PuPd = GPIO_PuPd_UP;
	gpio_cs.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOE, &gpio_cs);

	// Set chip select line (high output)
	GPIO_SetBits(GPIOE, GPIO_Pin_3);
	
	uint16_t temp;
	uint8_t tmpreg;

	// Set up configuration register on MEMS accelerometer chip
	temp  = (uint16_t) (LIS3DSH_DATARATE_100);
	temp |= (uint16_t) (LIS3DSH_XYZ_ENABLE);
	temp |= (uint16_t) (LIS3DSH_SELFTEST_NORMAL);
	temp |= (uint16_t) (LIS3DSH_SERIALINTERFACE_4WIRE);
	temp |= (uint16_t) (LIS3DSH_FULLSCALE_2);
	temp |= (uint16_t) (LIS3DSH_FILTER_BW_50 << 8);
	
	tmpreg = (uint8_t) (temp);

	// Write configuration value (lower byte) to MEMS CTRL_REG4 register
	writeSPI(LIS3DSH_CTRL_REG4_ADDR, tmpreg);

	tmpreg = (uint8_t) (temp >> 8);

	// Write configuration value (upper byte) to MEMS CTRL_REG5 register
	writeSPI(LIS3DSH_CTRL_REG5_ADDR, tmpreg);
}


void writeSPI(uint8_t address, uint8_t data){
 
	// Copy your code from lab 4 here

}


 
uint8_t readSPI(uint8_t address){

	// Copy your code from lab 4 here
}
 

void readAxes(AccelerometerDataStruct *dat) {

	// Copy your code from lab 4 here

}



void initLeds() {

    // Copy in your initLeds() function from lab2 

}



void showRandomNumber() {
	GPIO_ResetBits(GPIOD, LEDS);

        /* Wait for the data to appear.*/
        while(RESET == RNG_GetFlagStatus(RNG_FLAG_DRDY)){}
	// Don't let the number be zero
	int n = 0;
 	while (!n) {
		// Use 4 bits of random number to set LEDs
		n = RNG_GetRandomNumber() & 0xf << 12;
	}
	GPIO_SetBits(GPIOD, n);

}

int8_t isShaking() {


}
