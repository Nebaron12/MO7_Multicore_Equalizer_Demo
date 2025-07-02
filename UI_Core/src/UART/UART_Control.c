#include "UART_Control.h"

#define TIMEOUT_CYCLES (325 * CMD_TIMEOUT_US) // Convert to clock cycles

static inline int uartWaitChar(XUartPs* uart, u8* c, u64 timeoutCycles) {
    u64 start, now;
    XTime_GetTime(&start);

    do {
        if (XUartPs_IsReceiveData(uart->Config.BaseAddress)) {
            if (XUartPs_Recv(uart, c, 1) > 0)
                return 1;
        }
        XTime_GetTime(&now);
    } while ((now - start) < timeoutCycles);

    return 0; // Timeout
}

// UART Initialization
int UART_Init(UARTControl* uartControl, XUartPs* uartPS, volatile EQcontrols* eqControls) {
    XUartPs_Config *Config;
    int Status;

    // Look up the configuration
    Config = XUartPs_LookupConfig(UART_BASEADDRESS);
    if (!Config) {
        xil_printf("UART lookup failed\n");
        return XST_FAILURE;
    }

    // Initialize the UART driver
    Status = XUartPs_CfgInitialize(uartPS, Config, Config->BaseAddress);
    if (Status != XST_SUCCESS) {
        xil_printf("UART initialization failed\n");
        return XST_FAILURE;
    }

    // Run a self-test
    Status = XUartPs_SelfTest(uartPS);
    if (Status != XST_SUCCESS) {
        xil_printf("UART self-test failed\n");
        return XST_FAILURE;
    }

    // Set UART baud rate (optional)
    XUartPs_SetBaudRate(uartPS, UART_BAUD_RATE);

    // Set the UART instance and EQ controls in the UARTControl structure
    uartControl->uartInstance = uartPS;
    uartControl->eqControls = eqControls;
    
    return XST_SUCCESS;
}

// Command processing function
uint8_t cmdReceive(UARTControl* uartCtrl) {
    u8 receivedChar;

    while (XUartPs_IsReceiveData(uartCtrl->uartInstance->Config.BaseAddress)) {
        if (XUartPs_Recv(uartCtrl->uartInstance, &receivedChar, 1) <= 0)
            return 0;

        if (receivedChar != CMD_START)
            continue;

        // Wait for next byte (command type)
        if (!uartWaitChar(uartCtrl->uartInstance, &receivedChar, TIMEOUT_CYCLES))
            continue;

        switch (receivedChar) {
            case CMD_RESET:
                // Reset all EQ controls to default values
                EQcontrolsInit(uartCtrl->eqControls);

                #ifdef CMD_ECHO_CMD
                xil_printf(">> CMD_RESET: All values reset to defaults\n\r");
                #endif
                return 1;

            case CMD_ECHO: {
                // Echo current EQ settings
                printf(">> CMD_ECHO: Current EQ settings:\n\r");
                printf("Volume: %.2f\n\r", uartCtrl->eqControls->volume);
                printf("Filter Type: %u\n\r", uartCtrl->eqControls->filterType);
                for (int i = 0; i < FILTER_COUNT; i++) {
                    printf("Filter[%d] Gain: %.2f\n\r", i, uartCtrl->eqControls->EQgain[i]);
                }
                return 0;
            }

            case CMD_FILTYPE: {
                u8 type = STARTUP_FILTYPE;
                if (uartWaitChar(uartCtrl->uartInstance, &type, TIMEOUT_CYCLES))
                {
                    #ifdef CMD_NUMBER_FORMAT
                    type -= '0'; // Convert ASCII to integer if CMD_NUMBER_FORMAT is defined
                    #endif
                    uartCtrl->eqControls->filterType = type;
                }

                #ifdef CMD_ECHO_CMD
                xil_printf(">> CMD_FILTYPE: Filter type set to %u\n\r", uartCtrl->eqControls->filterType);
                #endif
                return 1;
            }

            case CMD_VOLUME: {
                u8 vol = STARTUP_VOLUME * 100;
                if (uartWaitChar(uartCtrl->uartInstance, &vol, TIMEOUT_CYCLES))
                    uartCtrl->eqControls->volume = (float)vol / 100.0f;
                else
                    uartCtrl->eqControls->volume = STARTUP_VOLUME;

                #ifdef CMD_ECHO_CMD
                xil_printf(">> CMD_VOLUME: Volume set to %u\n\r", vol);
                #endif
                return 1;
            }

            case CMD_GAIN: {
                u8 filterIdx;
                u8 gainVal = STARTUP_GAIN * 50;
                if (uartWaitChar(uartCtrl->uartInstance, &filterIdx, TIMEOUT_CYCLES)) {
                    #ifdef CMD_NUMBER_FORMAT
                    filterIdx -= '0'; // Convert ASCII to integer if CMD_NUMBER_FORMAT is defined
                    #endif

                    // Check if the filter index is valid
                    if (filterIdx >= FILTER_COUNT) {
                        #ifdef CMD_ECHO_CMD
                        xil_printf(">> CMD_GAIN: Invalid filter index %u, must be < %d\n\r", 
                                   filterIdx, FILTER_COUNT);
                        #endif
                        return 1; // Skip invalid filter index
                    }

                    // Wait for the gain value, reset to default if not received
                    if (uartWaitChar(uartCtrl->uartInstance, &gainVal, TIMEOUT_CYCLES)) {
                        uartCtrl->eqControls->EQgain[filterIdx] = (float)gainVal * 0.02f;
                    } else {
                        uartCtrl->eqControls->EQgain[filterIdx] = STARTUP_GAIN;
                    }

                    #ifdef CMD_ECHO_CMD
                    xil_printf(">> CMD_GAIN: Filter[%u] gain set to %u\n\r", 
                               filterIdx, gainVal * 2);
                    #endif

                    return 1;
                }
                return 0;
            }

            case CMD_ISOLATE: {
                u8 filterIdx;
                if (uartWaitChar(uartCtrl->uartInstance, &filterIdx, TIMEOUT_CYCLES)) {
                    #ifdef CMD_NUMBER_FORMAT
                    filterIdx -= '0'; // Convert ASCII to integer if CMD_NUMBER_FORMAT is defined
                    #endif

                    // Check if the filter index is valid
                    if (filterIdx >= FILTER_COUNT) {
                        #ifdef CMD_ECHO_CMD
                        xil_printf(">> CMD_ISOLATE: Invalid filter index %u, must be < %d\n\r", 
                                   filterIdx, FILTER_COUNT);
                        #endif
                        return 0; // Skip invalid filter index
                    }

                    // Isolate the specified filter
                    uartCtrl->eqControls->EQgain[filterIdx] = 1.0f; // Set isolated filter gain to 1.0f
                    for (int i = 0; i < FILTER_COUNT; i++) {
                        if (i != filterIdx) {
                            uartCtrl->eqControls->EQgain[i] = 0.0f; // Set other filters to 0.0f
                        }
                    }

                    #ifdef CMD_ECHO_CMD
                    xil_printf(">> CMD_ISOLATE: Filter[%u] isolated\n\r", filterIdx);
                    #endif

                    return 1;
                }
                return 0;
            }

            default:
                #ifdef CMD_ECHO_CMD
                xil_printf(">> Unknown command: '%c'\n\r", receivedChar);
                #endif
                return 0;
        }
    }
    return 0;
}
