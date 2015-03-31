#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_exti.h"
#include "misc.h"
#include "stm32f4xx_syscfg.h"


void init();

void initLeds();
void initTimer4();
void initTimer2();
void initPwm();


#define HSE_VALUE 8000000

int main() {

    init();


    /* In semi-hosting mode, you can toggle comment to run these lines and print bus speeds 
    RCC_ClocksTypeDef RCC_ClocksStruct;
    RCC_GetClocksFreq(&RCC_ClocksStruct);
    printf("SYSCLK:   %d\n", RCC_ClocksStruct.SYSCLK_Frequency);
    printf("AHB CLK:  %d\n", RCC_ClocksStruct.HCLK_Frequency);
    printf("APB1 CLK: %d\n", RCC_ClocksStruct.PCLK1_Frequency);
    printf("APB2 CLK: %d\n", RCC_ClocksStruct.PCLK2_Frequency);
    */

    while (1);


}

void init() {
    initLeds();
    initTimer4();
    initTimer2();
    initPwm();
}




void initLeds() {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    GPIO_InitTypeDef gpio;
    GPIO_StructInit(&gpio);
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_Pin = GPIO_Pin_12;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOD, &gpio);

    GPIO_InitTypeDef gpio2;
    GPIO_StructInit(&gpio2);
    gpio2.GPIO_Mode = GPIO_Mode_OUT;
    gpio2.GPIO_Pin = GPIO_Pin_15;
    gpio2.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOD, &gpio2);

}

void initTimer4() {


    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    TIM_TimeBaseInitTypeDef timerInitStructure;
    // The APB1 bus operates at 42 MHz
    // (System clock of 168 MHz / 4 APB1 prescaler, configured in system_stm32f4xx.c)
    // Since the APB1 prescaler > 1, the timer operates at double this, 84 MHz
    // Using a prescaler of 21000 - 1, we set the timer frequency to 82 MHz/21 kHz = 4kHz
    timerInitStructure.TIM_Prescaler = 21000 - 1;
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    // Configure timer to trigger every 4000 cycles,
    // now we should get an interrupt every 1 second
    timerInitStructure.TIM_Period = 4000 - 1;
    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    timerInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4, &timerInitStructure);
    TIM_Cmd(TIM4, ENABLE);

}


void initTimer2()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseInitTypeDef timerInitStructure; 
    // The APB1 bus operates at 42 MHz
    // (System clock of 168 MHz / 4 APB1 prescaler, configured in system_stm32f4xx.c)
    // Since the APB1 prescaler > 1, the timer operates at double this, 84 MHz
    // Using a prescaler of 21000 - 1, we set the timer frequency to 82 MHz/21 kHz = 4kHz
    timerInitStructure.TIM_Prescaler = 21000 - 1;
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    // Configure timer to trigger every 4000 cycles,
    // now we should get an interrupt every 1 second
    timerInitStructure.TIM_Period = 4000 - 1;
    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    timerInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &timerInitStructure);
    TIM_Cmd(TIM2, ENABLE);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    NVIC_InitTypeDef nvicStructure;
    nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
    nvicStructure.NVIC_IRQChannelSubPriority = 1;
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicStructure);

}



void initPwm() {
    TIM_OCInitTypeDef outputChannelInit = {0,};
    outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
    outputChannelInit.TIM_Pulse = 3000;
    outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
    outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM4, &outputChannelInit);
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);

}

void TIM2_IRQHandler()
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
    }
}
