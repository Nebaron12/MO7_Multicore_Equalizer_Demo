// UI-core main
#include "SharedMemFunctions.h"
#include "FaderPanel.h"
#include "UART_Control.h"
#include "I2C.h"
#include "Graphics.h"
#include "audio_codec.h"

// UI components
XIicPs IicInstance; // I2C instance for communication (The codec has its own I2C channel)
XUartPs UartInstance; // UART instance for communication
UARTControl uartControl; // UART control instance
OLED oledDisplay; // OLED display instance
FaderPanel faderPanel; // Fader panel instance

int main()
{
    // Reset the shared memory
    resetSharedMem();

    // Initialize I2C and UART
    if (I2C_Initialize(&IicInstance, XPAR_XIICPS_0_BASEADDR) != XST_SUCCESS) {
        xil_printf("I2C Initialization failed\n");
        return XST_FAILURE;
    }
    if (UART_Init(&uartControl, &UartInstance, &sharedMem->eqControls) != XST_SUCCESS) {
        xil_printf("UART Initialization failed\n");
        return XST_FAILURE;
    }
    xil_printf("I2C and UART initialized\n");

    // Initialize the fader panel
    faderPanelInit(&faderPanel, &IicInstance, &sharedMem->eqControls);
    xil_printf("Fader panel initialized\n");
    msleep(20);

    // Initialize the OLED display
    initDisplay(&oledDisplay, &IicInstance);
    xil_printf("Display initialized\n");
    msleep(20);

    // Configure the Audio Codec
    // configure_audio_codec(&IicCodec);
    // msleep(20);

    // Initialize the EQControls instance
    EQcontrolsInit(&sharedMem->eqControls);

    // Enter the main loop
    while (1) {
        // Update the fader panel
        if (faderPanelUpdate(&faderPanel) || cmdReceive(&uartControl)) {
            // Flush the EQ controls cache so the audio core can read the updated data
            Xil_DCacheFlushRange((UINTPTR)sharedMem, sizeof(SharedMem));
            __asm volatile("dmb sy" ::: "memory");

            // Update the oled display with the new data
            oledRenderEqualizer(&oledDisplay, &sharedMem->eqControls);
        }
    }
}