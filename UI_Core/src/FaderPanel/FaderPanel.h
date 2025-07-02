#ifndef __FADERPANEL_H_
#define __FADERPANEL_H_

#include <xiltimer.h>
#include <stdbool.h>
#include <xil_cache.h>
#include "tla2528.h"
#include "SharedMem.h"

#define FADERPANEL_I2C_ADDRESS 0x10 // 7-bit I2C address for the fader panel
#define FADERPANEL_UPDATE_RATE 325000 * 50 // Update rate in milliseconds
#define FADERPANEL_NUM_FADERS 8 // Number of faders on the panel
#define FADERPANEL_MAX_VALUE (1 << 12) - 1 // Maximum output value of the fader panel sliders
#define FADERPANEL_VOLUME_SLIDER // Uncomment to enable volume slider functionality (always the FILTER_COUNT + 1th fader, DO NOT USE IF FILTER_COUNT >= 8!)
//#define FADERPANEL_DEBUG // Uncomment to print gains upon update

typedef struct {
    XIicPs *iic; // Pointer to the I2C instance
    TLA2528 tla2528; // TLA2528 device instance
    volatile EQcontrols *eqControls; // Pointer to EQ controls
    long long unsigned int lastUpdateTime; // Last update time in clock cycles
    uint16_t faderValues[FADERPANEL_NUM_FADERS]; // Array to hold fader values
} FaderPanel;

// Function prototypes
void faderPanelInit(FaderPanel *panel, XIicPs *iic, volatile EQcontrols *eqControls);
bool faderPanelUpdate(FaderPanel* panel);

#endif