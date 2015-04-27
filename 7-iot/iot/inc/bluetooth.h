#ifndef BLUETOOTH_H
#define BLUETOOTH_H

/* Set up UART peripheral on USART1 for use by Bluetooth module */
void initUsart();

/* Send a byte of data over the Bluetooth serial connection           */
void btSend(uint8_t data);
/* To receive data over Bluetooth, use an interrupt handler (USART1_IRQHandler)  */

/* Set up GPIO pin connected to Bluetooth status line  */
void initBtState();

/* Gives current Bluetooth connection status           */
int btConnected();

#endif
