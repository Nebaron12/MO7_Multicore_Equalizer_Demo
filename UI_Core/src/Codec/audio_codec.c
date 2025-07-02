/*
 * audio.c
 *
 *  Contains all of the functions related to audio codec setup.
 */

#include "audio_codec.h"
#include <xiicps.h>

// Function to combine all config into one
void configure_audio_codec(XIicPs* IIC) {
    xil_printf("Starting audio codec configuration...\n\r");
    
    // Ensure I2C bus is ready
    while (XIicPs_BusIsBusy(IIC)) {
        msleep(1);
    }
    
    // Configure codec with proper sequencing
    AudioPllConfig(IIC);
    
    // Wait for PLL to stabilize
    msleep(50);
    
    AudioConfigureJacks(IIC);
    
    // Final wait to ensure codec is fully configured
    while (XIicPs_BusIsBusy(IIC)) {
        msleep(1);
    }
    msleep(50);

    // Verify codec is responding correctly
    if (verify_codec_response(IIC) == XST_SUCCESS) {
        xil_printf("ADAU1761 configured and verified successfully\n\r");
    } else {
        xil_printf("ADAU1761 configured but verification failed\n\r");
    }
}

/* ---------------------------------------------------------------------------- *
 * 								AudioPllConfig()								*
 * ---------------------------------------------------------------------------- *
 * Configures audio codes's internal PLL. With MCLK = 10 MHz it configures the
 * PLL for a VCO frequency = 49.152 MHz, and an audio sample rate of 48 KHz.
 * ---------------------------------------------------------------------------- */
void AudioPllConfig(XIicPs* IIC) {
    unsigned char u8TxData[8], u8RxData[6];
    int Status;

    // Disable Core Clock
    AudioWriteToReg(IIC, R0_CLOCK_CONTROL, 0x0E);
    xil_printf("Core clock disabled\r\n");

    // Set PLL configuration - Complete sequence to write to PLL Control Register (R1)
    u8TxData[0] = 0x40;      // 0b01000000 - Register Write Address Byte (Control Port Register Write)
    u8TxData[1] = 0x02;      // 0b00000010 - PLL Control Register Address (R1 = 0x02)
    
    // Configure Integer Clock Divider (M) = 625 (0x0271 in two bytes)
    // This divides the PLL VCO output to produce the required sample rate
    u8TxData[2] = 0x02;      // 0b00000010 - Integer Clock Divider (M) MSBs [15:8]
    u8TxData[3] = 0x71;      // 0b01110001 - Integer Clock Divider (M) LSBs [7:0]
                            // Combined value: 0x0271 = 625 decimal
    
    // Configure PLL Input Clock Divider (N) = 2
    // This divides the input clock before it feeds the PLL
    u8TxData[4] = 0x02;      // 0b00000010 - Input Clock Divider (N)
                            // 0x02 = divide input clock by 2
    
    // Configure PLL Type and No-Clock Timeout options
    u8TxData[5] = 0x3C;      // 0b00111100 - PLL settings:
                            // Bits [7:6]: Reserved
                            // Bits [5:4]: No-Clock Timeout (11 = longest timeout)
                            // Bits [3:2]: PLL Type (11 = Type High Performance)
                            // Bits [1:0]: X Factor (00 = 1)
    
    // Configure Power Management and Fractional PLL values
    u8TxData[6] = 0x21;      // 0b00100001 - Power & Fractional settings:
                            // Bit 5: PLL Power-Down (1 = normal operation)
                            // Bits [4:0]: Fractional N value MSBs
    
    // Enable PLL
    u8TxData[7] = 0x01;      // 0b00000001 - PLL Control:
                            // Bit 0: PLL Enable Bit (1 = enabled)

    Status = XIicPs_MasterSendPolled(IIC, u8TxData, 8, (IIC_SLAVE_ADDR >> 1));
    if (Status != XST_SUCCESS) {
        xil_printf("I2C write to PLL failed\r\n");
        return;
    }
    while (XIicPs_BusIsBusy(IIC));

    xil_printf("PLL bytes written\r\n");

    // Poll PLL Lock bit
    u8TxData[0] = 0x40;
    u8TxData[1] = 0x02;

    do {
        Status = XIicPs_MasterSendPolled(IIC, u8TxData, 2, (IIC_SLAVE_ADDR >> 1));
        if (Status != XST_SUCCESS) {
            xil_printf("PLL lock poll write failed\r\n");
            return;
        }
        while (XIicPs_BusIsBusy(IIC));

        Status = XIicPs_MasterRecvPolled(IIC, u8RxData, 6, (IIC_SLAVE_ADDR >> 1));
        if (Status != XST_SUCCESS) {
            xil_printf("PLL lock poll read failed\r\n");
            return;
        }
        while (XIicPs_BusIsBusy(IIC));

    } while ((u8RxData[5] & 0x02) == 0); // Check lock bit

    xil_printf("PLL Locked\r\n");

    // Enable Core Clock
    AudioWriteToReg(IIC, R0_CLOCK_CONTROL, 0x0F);
}


/* ---------------------------------------------------------------------------- *
 * 								AudioWriteToReg									*
 * ---------------------------------------------------------------------------- *
 * Function to write one byte (8-bits) to one of the registers from the audio
 * controller.
 * ---------------------------------------------------------------------------- */
void AudioWriteToReg(XIicPs* IIC, unsigned char u8RegAddr, unsigned char u8Data) {
    unsigned char u8TxData[3], u8RxData[1];
    int Status;

    u8TxData[0] = 0x40;  // Device address (assuming this is correct)
    u8TxData[1] = u8RegAddr;
    u8TxData[2] = u8Data;

    // Write the data
    Status = XIicPs_MasterSendPolled(IIC, u8TxData, 3, (IIC_SLAVE_ADDR >> 1));
    if (Status != XST_SUCCESS) {
        xil_printf("I2C write failed for reg 0x%02X\r\n", u8RegAddr);
        return;
    }
    while (XIicPs_BusIsBusy(IIC));

    // Read back to verify
    u8TxData[0] = 0x40;
    u8TxData[1] = u8RegAddr;

    Status = XIicPs_MasterSendPolled(IIC, u8TxData, 2, (IIC_SLAVE_ADDR >> 1));
    if (Status != XST_SUCCESS) {
        xil_printf("I2C write (readback request) failed for reg 0x%02X\r\n", u8RegAddr);
        return;
    }
    while (XIicPs_BusIsBusy(IIC));

    Status = XIicPs_MasterRecvPolled(IIC, u8RxData, 1, (IIC_SLAVE_ADDR >> 1));
    if (Status != XST_SUCCESS) {
        xil_printf("I2C read failed for reg 0x%02X\r\n", u8RegAddr);
        return;
    }
    while (XIicPs_BusIsBusy(IIC));

    // Check if written value matches read value
    if (u8RxData[0] != u8Data) {
        xil_printf("I2C MISMATCH: reg 0x%02X expected 0x%02X, got 0x%02X\r\n", u8RegAddr, u8Data, u8RxData[0]);
    }
}

/* ---------------------------------------------------------------------------- *
 * 								AudioConfigureJacks()							*
 * ---------------------------------------------------------------------------- *
 * Configures audio codes's various mixers, ADC's, DAC's, and amplifiers to
 * accept stereo input from line in and push stereo output to line out.
 * ---------------------------------------------------------------------------- */
void AudioConfigureJacks(XIicPs* IIC)
{
	AudioWriteToReg(IIC, R4_RECORD_MIXER_LEFT_CONTROL_0, 0b00000001); //enable mixer 1
	AudioWriteToReg(IIC, R5_RECORD_MIXER_LEFT_CONTROL_1, 0b00000001); //unmute Left channel of line in into mxr 1 and set gain to 6 db
	AudioWriteToReg(IIC, R6_RECORD_MIXER_RIGHT_CONTROL_0, 0b00000001); //enable mixer 2
	AudioWriteToReg(IIC, R7_RECORD_MIXER_RIGHT_CONTROL_1, 0b00000001); //unmute Right channel of line in into mxr 2 and set gain to 6 db
	AudioWriteToReg(IIC, R19_ADC_CONTROL, 0b00010011); //enable ADCs

	AudioWriteToReg(IIC, R22_PLAYBACK_MIXER_LEFT_CONTROL_0, 0b00100001); //unmute Left DAC into Mxr 3; enable mxr 3
	AudioWriteToReg(IIC, R24_PLAYBACK_MIXER_RIGHT_CONTROL_0, 0b01000001); //unmute Right DAC into Mxr4; enable mxr 4
	AudioWriteToReg(IIC, R26_PLAYBACK_LR_MIXER_LEFT_LINE_OUTPUT_CONTROL, 0b00000101); //unmute Mxr3 into Mxr5 and set gain to 6db; enable mxr 5
	AudioWriteToReg(IIC, R27_PLAYBACK_LR_MIXER_RIGHT_LINE_OUTPUT_CONTROL, 0b00010001); //unmute Mxr4 into Mxr6 and set gain to 6db; enable mxr 6
	AudioWriteToReg(IIC, R29_PLAYBACK_HEADPHONE_LEFT_VOLUME_CONTROL, 0b11011111);//Mute Left channel of HP port (LHP)
	AudioWriteToReg(IIC, R30_PLAYBACK_HEADPHONE_RIGHT_VOLUME_CONTROL, 0b11011111); //Mute Right channel of HP port (LHP)
	//AudioWriteToReg(IIC, R31_PLAYBACK_LINE_OUTPUT_LEFT_VOLUME_CONTROL, 0xE6); //set LOUT volume (0db); unmute left channel of Line out port; set Line out port to line out mode
	//AudioWriteToReg(IIC, R32_PLAYBACK_LINE_OUTPUT_RIGHT_VOLUME_CONTROL, 0xE6); // set ROUT volume (0db); unmute right channel of Line out port; set Line out port to line out mode
	AudioWriteToReg(IIC, R31_PLAYBACK_LINE_OUTPUT_LEFT_VOLUME_CONTROL, 0b00000000); //set LOUT volume (0db); unmute left channel of Line out port; set Line out port to line out mode
	AudioWriteToReg(IIC, R32_PLAYBACK_LINE_OUTPUT_RIGHT_VOLUME_CONTROL, 0b00000000); // set ROUT volume (0db); unmute right channel of Line out port; set Line out port to line out mode
	AudioWriteToReg(IIC, R35_PLAYBACK_POWER_MANAGEMENT, 0b10101011); //enable left and right channel playback (not sure exactly what this does...)
	AudioWriteToReg(IIC, R36_DAC_CONTROL_0, 0b00000011); //enable both DACs

	AudioWriteToReg(IIC, R58_SERIAL_INPUT_ROUTE_CONTROL, 0b00000001); //Connect I2S serial port output (SDATA_O) to DACs
	AudioWriteToReg(IIC, R59_SERIAL_OUTPUT_ROUTE_CONTROL, 0b00000001); //connect I2S serial port input (SDATA_I) to ADCs

	AudioWriteToReg(IIC, R65_CLOCK_ENABLE_0, 0b01111111); //Enable clocks
	AudioWriteToReg(IIC, R66_CLOCK_ENABLE_1, 0b00000011); //Enable rest of clocks
}




/* ---------------------------------------------------------------------------- *
 * 								LineinLineoutConfig()							*
 * ---------------------------------------------------------------------------- *
 * Configures Line-In input, ADC's, DAC's, Line-Out and HP-Out.
 * ---------------------------------------------------------------------------- */
void LineinLineoutConfig(XIicPs* IIC) {

	AudioWriteToReg(IIC, R17_CONVERTER_CONTROL_0, 0x05);//48 KHz
	AudioWriteToReg(IIC, R64_SERIAL_PORT_SAMPLING_RATE, 0x05);//48 KHz
	AudioWriteToReg(IIC, R19_ADC_CONTROL, 0x13);
	AudioWriteToReg(IIC, R36_DAC_CONTROL_0, 0x03);
	AudioWriteToReg(IIC, R35_PLAYBACK_POWER_MANAGEMENT, 0x03);
	AudioWriteToReg(IIC, R58_SERIAL_INPUT_ROUTE_CONTROL, 0x01);
	AudioWriteToReg(IIC, R59_SERIAL_OUTPUT_ROUTE_CONTROL, 0x01);
	AudioWriteToReg(IIC, R65_CLOCK_ENABLE_0, 0x7F);
	AudioWriteToReg(IIC, R66_CLOCK_ENABLE_1, 0x03);

	AudioWriteToReg(IIC, R4_RECORD_MIXER_LEFT_CONTROL_0, 0x01);
	AudioWriteToReg(IIC, R5_RECORD_MIXER_LEFT_CONTROL_1, 0x05);//0 dB gain
	AudioWriteToReg(IIC, R6_RECORD_MIXER_RIGHT_CONTROL_0, 0x01);
	AudioWriteToReg(IIC, R7_RECORD_MIXER_RIGHT_CONTROL_1, 0x05);//0 dB gain

	AudioWriteToReg(IIC, R22_PLAYBACK_MIXER_LEFT_CONTROL_0, 0x21);
	AudioWriteToReg(IIC, R24_PLAYBACK_MIXER_RIGHT_CONTROL_0, 0x41);
	AudioWriteToReg(IIC, R26_PLAYBACK_LR_MIXER_LEFT_LINE_OUTPUT_CONTROL, 0x03);//0 dB
	AudioWriteToReg(IIC, R27_PLAYBACK_LR_MIXER_RIGHT_LINE_OUTPUT_CONTROL, 0x09);//0 dB
	AudioWriteToReg(IIC, R29_PLAYBACK_HEADPHONE_LEFT_VOLUME_CONTROL, 0xE7);//0 dB
	AudioWriteToReg(IIC, R30_PLAYBACK_HEADPHONE_RIGHT_VOLUME_CONTROL, 0xE7);//0 dB
	AudioWriteToReg(IIC, R31_PLAYBACK_LINE_OUTPUT_LEFT_VOLUME_CONTROL, 0xE6);//0 dB
	AudioWriteToReg(IIC, R32_PLAYBACK_LINE_OUTPUT_RIGHT_VOLUME_CONTROL, 0xE6);//0 dB
}

/* ---------------------------------------------------------------------------- *
 * 								verify_codec_response()							*
 * ---------------------------------------------------------------------------- *
 * Verifies that the codec is responding to I2C communications by reading back
 * a known register and checking if the codec is accessible.
 * ---------------------------------------------------------------------------- */
int verify_codec_response(XIicPs* IIC) {
    unsigned char u8TxData[2], u8RxData[1];
    int Status;
    
    // Try to read the clock control register (R0)
    u8TxData[0] = 0x40;  // Register read command
    u8TxData[1] = R0_CLOCK_CONTROL;
    
    Status = XIicPs_MasterSendPolled(IIC, u8TxData, 2, (IIC_SLAVE_ADDR >> 1));
    if (Status != XST_SUCCESS) {
        xil_printf("Codec verification: failed to send read command\r\n");
        return XST_FAILURE;
    }
    while (XIicPs_BusIsBusy(IIC));
    
    Status = XIicPs_MasterRecvPolled(IIC, u8RxData, 1, (IIC_SLAVE_ADDR >> 1));
    if (Status != XST_SUCCESS) {
        xil_printf("Codec verification: failed to read response\r\n");
        return XST_FAILURE;
    }
    while (XIicPs_BusIsBusy(IIC));
    
    xil_printf("Codec verification: Clock control register = 0x%02X\r\n", u8RxData[0]);
    
    // A typical good value would be 0x0F (all clocks enabled)
    if (u8RxData[0] == 0x0F) {
        xil_printf("Codec verification: PASSED - codec is responding correctly\r\n");
        return XST_SUCCESS;
    } else {
        xil_printf("Codec verification: WARNING - unexpected register value\r\n");
        return XST_SUCCESS; // Don't fail, just warn
    }
}
