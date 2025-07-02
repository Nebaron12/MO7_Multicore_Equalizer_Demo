#include "FaderPanel.h"
#include <xiicps.h>

void faderPanelInit(FaderPanel *panel, XIicPs *iic, volatile EQcontrols* eqControls) {
    panel->iic = iic;
    TLA2528_Init(&panel->tla2528, iic, FADERPANEL_I2C_ADDRESS);
    panel->eqControls = eqControls;
    panel->lastUpdateTime = 0;
}

bool faderPanelUpdate(FaderPanel *panel) {
    long long unsigned int currentTime;
    XTime_GetTime(&currentTime);
    
    // Check if enough time has passed since the last update
    if ((currentTime - panel->lastUpdateTime >= FADERPANEL_UPDATE_RATE) && !XIicPs_BusIsBusy(panel->iic)) {
        
        // Read fader values from TLA2528
        if (TLA2528_ReadAllChannels(&panel->tla2528, panel->faderValues) == XST_SUCCESS) {
            // Allow the bass gain to go up to 8
            panel->eqControls->EQgain[0] = (float)(FADERPANEL_MAX_VALUE - panel->faderValues[0]) / 511.875f; // Normalize 12 bit value to [0, 8] float range

            // Set the rest of the gains
            for (uint8_t i = 1; i < FILTER_COUNT; i++) {
                // Update EQ controls with fader values
                panel->eqControls->EQgain[i] = (float)(FADERPANEL_MAX_VALUE - panel->faderValues[i]) / 1023.75f; // Normalize 12 bit value to [0, 4] float range
                #ifdef FADERPANEL_DEBUG
                xil_printf("Filter %u: %u   ", i, (uint16_t)(panel->eqControls->EQgain[i] * 100));
                #endif
            }
            
            #ifdef FADERPANEL_VOLUME_SLIDER
            // Update volume slider if enabled
            panel->eqControls->volume = (float)(FADERPANEL_MAX_VALUE - panel->faderValues[FILTER_COUNT]) / 4095.0f; // Normalize 12 bit value to [0, 1] float range
            #endif

            #ifdef FADERPANEL_DEBUG
            xil_printf("\n");
            #endif
        } else {
            xil_printf("Failed to read fader values from TLA2528\n");
        }
        
        panel->lastUpdateTime = currentTime;

        return TRUE;
    }

    return FALSE;
}