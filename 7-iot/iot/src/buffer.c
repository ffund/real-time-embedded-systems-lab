#include <stdint.h>
#include "buffer.h"


/**
 * @brief Initialize buffer, set write and read index to zero
 * 
 */
void initBuffer(buffer* buffer){
	int i;
	buffer->writeIndex = 0;
	buffer->readIndex = 0;
	for(i=0;i<BUFF_SIZE;i++){
		buffer->buff[i]=0;
	}
}


/**
 * @brief Write to circular buffer, update write index.
 * 
 * This function does *not* check for overflow
 * 
 */
void write(buffer* buffer, uint8_t value){
	buffer->buff[buffer->writeIndex]=value;
	buffer->writeIndex++;
	if(buffer->writeIndex==BUFF_SIZE) {
		buffer->writeIndex=0;
	}
}


/**
 * @brief Read oldest byte in circular buffer, update read index. 
 * 
 * This function does *not* check for overflow/underflow
 * 
 */
volatile uint8_t read(buffer* buffer){
	uint8_t temp;
	temp=buffer->buff[buffer->readIndex];
	buffer->readIndex++;
	if(buffer->readIndex==BUFF_SIZE){
		buffer->readIndex=0;
	}
	return temp;
}


/**
 * @brief Return size of unread data in circular buffer. 
 * 
 * This function does *not* check for overflow/underflow
 * 
 */
volatile uint8_t size(buffer* buffer){
	return ((uint8_t)(buffer->writeIndex - buffer->readIndex) % BUFF_SIZE);
}

