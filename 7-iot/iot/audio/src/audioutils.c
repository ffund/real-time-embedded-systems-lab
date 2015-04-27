#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "stm32f4xx_conf.h"
#include "utils.h"
#include "Audio.h"
#include "mp3dec.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"


// Private variables
volatile uint32_t time_var1, time_var2;
MP3FrameInfo mp3FrameInfo;
HMP3Decoder hMP3Decoder;

// Private function prototypes
static void AudioCallback(void *context,int buffer);
void initMusic();

// External variables
extern const char mp3_data[];

// Some macros
#define MP3_SIZE	820845
#define BUTTON		(GPIOA->IDR & GPIO_Pin_0)


/**
 * @brief Play the MP3 music file
 *
 */
void playMusic() {

	hMP3Decoder = MP3InitDecoder();
	InitializeAudio(Audio44100HzSettings);
	SetAudioVolume(0xCF);
	PlayAudioWithCallback(AudioCallback, 0);
}


/**
 * @brief Stop the music playback
 *
 */
void stopMusic() {
	StopAudio();
}


/**
 * @brief Adjust the music volume
 *
 */
void setMusicVolume(uint8_t volume) {
	SetAudioVolume(volume);
}


/*
 * Called by the audio driver when it is time to provide data to
 * one of the audio buffers (while the other buffer is sent to the
 * CODEC using DMA). One mp3 frame is decoded at a time and
 * provided to the audio driver.
 */
static void AudioCallback(void *context, int buffer) {
	static int16_t audio_buffer0[4096];
	static int16_t audio_buffer1[4096];

	int offset, err;
	int outOfData = 0;
	static const char *read_ptr = mp3_data;
	static int bytes_left = MP3_SIZE;

	int16_t *samples;

	if (buffer) {
		samples = audio_buffer0;
	} else {
		samples = audio_buffer1;
	}

	offset = MP3FindSyncWord((unsigned char*)read_ptr, bytes_left);
	bytes_left -= offset;

	if (bytes_left <= 10000) {
		read_ptr = mp3_data;
		bytes_left = MP3_SIZE;
		offset = MP3FindSyncWord((unsigned char*)read_ptr, bytes_left);
	}

	read_ptr += offset;
	err = MP3Decode(hMP3Decoder, (unsigned char**)&read_ptr, &bytes_left, samples, 0);

	if (err) {
		/* error occurred */
		switch (err) {
		case ERR_MP3_INDATA_UNDERFLOW:
			outOfData = 1;
			break;
		case ERR_MP3_MAINDATA_UNDERFLOW:
			/* do nothing - next call to decode will provide more mainData */
			break;
		case ERR_MP3_FREE_BITRATE_SYNC:
		default:
			outOfData = 1;
			break;
		}
	} else {
		/* no error */
		MP3GetLastFrameInfo(hMP3Decoder, &mp3FrameInfo);
	}

	if (!outOfData) {
		ProvideAudioBuffer(samples, mp3FrameInfo.outputSamps);
	}
}


