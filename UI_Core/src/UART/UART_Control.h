#ifndef __UART_CONTROL_H_
#define __UART_CONTROL_H_

#include <xuartps.h>
#include <xiltimer.h>
#include <xil_printf.h>
#include <stdio.h>
#include "SharedMemFunctions.h"


// UART EQ communication protocol documentation
// No seperators between characters or values, ' - ' is for readability only

// Start of command character
// #-CMD
#define CMD_START '#'

// Reset to default settings (See: "EQ_Controls.h") -----------------------------------------------------
// #R
#define CMD_RESET 'R'

// Echo all current EQ settings to console --------------------------------------------------------------
// #E
#define CMD_ECHO 'E'

// Set throughput volume --------------------------------------------------------------------------------
// #V 					- Resets volume to default value
// #V-0xXX				- Sets volume to the value following #V command, 
//				  	    value is in uint8_t format ranging 0-255 to 0.00-2.55f
#define CMD_VOLUME 'V'

// Set EQ filter gain -----------------------------------------------------------------------------------
// #G-0xXX			    - Resets filter number 0xXX EQ-gain to default value,
//				   	    filter number is passed as uint8_t (Max 256 filters)
// #G-0xXX-0xYY		    - Sets filter number 0xXX to EQ-gain value 0xYY
//					    filter number is passed as uint8_t (Max 256 filters)
//					    EQ-gain is passed as uint8_t format ranging 0-255 to 0.00-5.10f (steps of 0.02f)
#define CMD_GAIN 'G'

// Change filter type -----------------------------------------------------------------------------------
// #T-0xXX				- Sets filter type currently used, 0xXX is in uint8_t format
//					    (0 = Throughput, 1 = Equalizer, etc)
#define CMD_FILTYPE 'T'

// Isolate a single filter ------------------------------------------------------------------------------
// #I-0xXX				- Isolates filter number 0xXX,
//					    filter number is passed as uint8_t (Max 256 filters)
#define CMD_ISOLATE 'I'

// User defined -----------------------------------------------------------------------------------------
#define UART_BASEADDRESS XPAR_XUARTPS_0_BASEADDR    // Base address of the UART peripheral
#define UART_BAUD_RATE 256000                       // Baud rate for UART communication
#define CMD_ECHO_CMD                                // Uncomment to enable command echoing in the console
#define CMD_NUMBER_FORMAT                           // Uncomment to allow indexing and filter type setting using decimal numbers (0-9) in commands
#define CMD_TIMEOUT_US 100                          // Timeout for waiting for a character in microseconds

// UART control structure
typedef struct {
    XUartPs* uartInstance; // Pointer to the UART instance
    volatile EQcontrols* eqControls; // Pointer to the EQ controls structure
} UARTControl;

// Functions
int UART_Init(UARTControl* uartControl, XUartPs* uartPS, volatile EQcontrols* EQctrls);
uint8_t cmdReceive(UARTControl* uartControl);



#endif