#ifndef __AUDIO_CODEC_H_
#define __AUDIO_CODEC_H_

#include <stdint.h>
#include <xil_io.h>
#include "xparameters.h"

/* Redefine audio controller base address from xparameters.h */
#define AUDIO_BASE				XPAR_AUDIO_AUDIO_CODEC_CTRL_0_BASEADDR

/* Audio controller registers */
enum i2s_regs {
	I2S_DATA_RX_L_REG	= 0x00 + AUDIO_BASE,
	I2S_DATA_RX_R_REG	= 0x04 + AUDIO_BASE,
	I2S_DATA_TX_L_REG   = 0x08 + AUDIO_BASE,
	I2S_DATA_TX_R_REG   = 0x0c + AUDIO_BASE,
	I2S_STATUS_REG      = 0x10 + AUDIO_BASE,
};

/* Prototype Functions */
void AudioWriteToCodec(int32_t leftSample, int32_t rightSample);
void AudioReadFromCodec(int32_t* leftSample, int32_t* rightSample);

#endif
