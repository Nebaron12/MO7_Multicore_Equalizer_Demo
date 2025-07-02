/*
 * audio.c
 *
 *  Contains all of the functions related to audio codec setup.
 */

#include "audio_codec.h"

// Writes the given samples to the audio codec through the AXI bus
void AudioWriteToCodec(int32_t leftSample, int32_t rightSample)
{
    // Write back as 24-bit left-aligned
    Xil_Out32(I2S_DATA_TX_L_REG, (uint32_t)(leftSample) & 0x00FFFFFF);
    Xil_Out32(I2S_DATA_TX_R_REG, (uint32_t)(rightSample) & 0x00FFFFFF);
}

// Writes the given samples to the audio codec through the AXI bus
void AudioReadFromCodec(int32_t* leftSample, int32_t* rightSample)
{
    // Read 24-bit signed input samples
    *leftSample = (int32_t)(Xil_In32(I2S_DATA_RX_L_REG) << 8) >> 8;
    *rightSample = (int32_t)(Xil_In32(I2S_DATA_RX_R_REG) << 8) >> 8;
}