#ifndef BUFFER_H
#define BUFFER_H


#define BUFF_SIZE 8
#define BUFF_SIZE_MASK (BUFF_SIZE-1)
/* Keep buffer size as a power of 2 */


/* Struct for circular buffer: has buffer, read index, and write index */
typedef struct buffer{
	uint8_t buff[BUFF_SIZE];
	int8_t readIndex;
	int8_t writeIndex;
}buffer;

/* Initialize buffer, set write and read index to zero */
void initBuffer(buffer* buffer);

/* Write to circular buffer, update write index */
void write(buffer* buffer, uint8_t value);

/* Read oldest byte in circular buffer, update read index */
volatile uint8_t read(buffer* buffer);

/* Return size of unread data in circular buffer */
volatile uint8_t size(buffer* buffer);

#endif
