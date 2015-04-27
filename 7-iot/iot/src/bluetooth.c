#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_usart.h"
#include "misc.h"
#include "stm32f4xx_syscfg.h"


#define BT_BAUD 9600

/**
 * @brief Set up UART peripheral on USART1 for use by Bluetooth module
 *
 * This function configures the USART1 peripheral. It sets up the GPIO 
 * pins (PA9 and PA10) in alternate function mode and enables a
 * receive interrupt with high priority.
 * 
 * You must define an interrupt handler (USART1_IRQHandler) to handle
 * received data.
 */
void initUsart() {

  GPIO_InitTypeDef GPIO_InitStruct; 
  USART_InitTypeDef USART_InitStruct;
  NVIC_InitTypeDef NVIC_InitStructure; 

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; 
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; 			
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;			
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;			
  GPIO_Init(GPIOA, &GPIO_InitStruct);					
  

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1); 
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
  

  USART_InitStruct.USART_BaudRate = BT_BAUD;				
  USART_InitStruct.USART_WordLength = USART_WordLength_8b;
  USART_InitStruct.USART_StopBits = USART_StopBits_1;
  USART_InitStruct.USART_Parity = USART_Parity_No;
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART_Init(USART1, &USART_InitStruct);	

  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);		
  
  USART_Cmd(USART1, ENABLE);
}




/**
 * @brief Send a byte of data over the Bluetooth serial connection
 *
 * This function blocks until the USART1 transmit buffer is empty,
 * then places a byte of data inside the transmit buffer to be sent.
 */
void btSend(uint8_t data)
{
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
  USART_SendData(USART1, data);

}


/**
 * @brief Set up GPIO pin connected to Bluetooth status line
 *
 * This function sets up the GPIO pin (PD1) as a digital input
 * with an interrupt enabled on any logic change. This pin will 
 * be connected to the status output of the HC-05 Bluetooth module.
 * By reading this input, we can detect whether there is a 
 * Bluetooth  connection (logical 1 on this input) or not (logical 0 on 
 * this input).
 */
void initBtState() {
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;		
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource1);
    
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;             
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;    
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; 
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;            
    EXTI_Init(&EXTI_InitStructure);                         
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;                
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;   
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;         
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               
    NVIC_Init(&NVIC_InitStructure);                                 
}



/**
 * @brief Gives current Bluetooth connection status
 *
 * This function returns 1 if there is currently an active 
 * Bluetooth connection (as indicated by a logical 1 on the 
 * input pin connected to the HC-05 status line). Otherwise, 
 * it returns 0.
 */
int btConnected() {
    return GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1);
}


