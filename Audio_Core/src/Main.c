#include <stdio.h>
#include <xil_cache.h>
#include <sleep.h>
#include "SharedMem.h"
#include "audio_codec.h"
#include "EQ_NEON.h"
#include "I2C.h"

// Codec
XIicPs IicCodec; // I2C instance for communication (The codec has its own I2C channel)
uint32_t status_reg; // Uncomment if using the I2S status register instead of timer polling

// Filtering
EQFilterState* eqState;
int32_t in_left, in_right;
float out_left, out_right;


int main(void)
{
    // Wait for the UI core to have everything set up
    // do {
    //     msleep(20);
    //     Xil_DCacheInvalidateRange((UINTPTR)sharedMem, sizeof(SharedMem));
    //     __asm volatile("dmb sy" ::: "memory");
    // } while (sharedMem->state == 0);

    // Initialize EQ filter state
    eqState = EQ_filters_init();
    if (!eqState) {
        xil_printf("Failed to initialize EQ filters!\n");
        return -1;
    }

    // Wait for the UI core to set everything up and start updating the shared memory
    msleep(200);

    // Initialize I2C
    if (I2C_Initialize(&IicCodec, XPAR_XIICPS_1_BASEADDR) != XST_SUCCESS) {
        xil_printf("I2C Initialization failed\n");
        return XST_FAILURE;
    }

    // Configure the Audio Codec
    configure_audio_codec(&IicCodec);
    msleep(20);

    // Configure the Audio Codec
    configure_audio_codec(&IicCodec);
    msleep(20);

    // Enter the main loop
    while (1)
    {
        // Get the data ready bit from the codec status register
        status_reg = Xil_In32(I2S_STATUS_REG);
        if ((status_reg >> 21) & 1) {
            // Clear the status register
            Xil_Out32(I2S_STATUS_REG, status_reg & (1 << 21));

            // Invalidate the EQ controls cache before using
            Xil_DCacheInvalidateRange((UINTPTR)sharedMem, sizeof(SharedMem));
            __asm volatile("dmb sy" ::: "memory");

            // Read 24-bit signed input samples
            AudioReadFromCodec(&in_left, &in_right);

            // Filter the audio based on the filter type
            switch (sharedMem->eqControls.filterType) {
                case 0: // Throughput
                    // Directly write the input to output without filtering
                    out_left = in_left;
                    out_right = in_right;
                    break;

                case 1: // Equalizer
                    // Process the input through the equalizer filters using NEON
                    apply_equalizer_neon(eqState, (float)in_left, (float)in_right, &out_left, &out_right, &sharedMem->eqControls);
                    break;

                default:
                    // Default case, don't do anything
                    break;
            }

            // Apply volume and write output to codec
            AudioWriteToCodec((int32_t)(out_left * sharedMem->eqControls.volume), (int32_t)(out_right * sharedMem->eqControls.volume));
        }
    }

    // Cleanup (never reached in this loop, but good practice)
    EQ_filters_free(eqState);
}