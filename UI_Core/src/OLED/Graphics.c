#include "Graphics.h"

// Renders the equalizer band gains and overall volume on the OLED display
void oledRenderEqualizer(OLED* screen, const volatile EQcontrols* eqCtrls)
{
    // Constants
    const int numBars = FILTER_COUNT + 1; // 6 EQ + 1 volume
    const int barWidth = 10;
    const int barSpacing = 6;
    const int barBaseY = screen->DispHeight - 10; // Leave space for gain text
    const int barMaxHeight = 40;
    const int valueTextY = screen->DispHeight - 1;
    const int volumeBarExtra = 1; // Make volume bar wider

    // Clear display buffer
    u8g2_ClearBuffer(&screen->u8g2);

    // Draw filter type in top-left
    u8g2_SetFont(&screen->u8g2, STD_FONT);
    const char* filterNames[] = { "Direct", "7-Band EQ" };
    char filterStr[16];
    if (eqCtrls->filterType == 0)
        snprintf(filterStr, sizeof(filterStr), "Type:%s", filterNames[0]);
    else if (eqCtrls->filterType == 1)
        snprintf(filterStr, sizeof(filterStr), "Type:%s", filterNames[1]);
    else
        snprintf(filterStr, sizeof(filterStr), "Type:%d", eqCtrls->filterType);
    u8g2_DrawStr(&screen->u8g2, 0, 10, filterStr);
    u8g2_SetFont(&screen->u8g2, EQ_FONT);

    // Draw bars and values
    int x = 3;
    for (int i = 0; i < numBars; ++i) {
        float value = (i < FILTER_COUNT) ? eqCtrls->EQgain[i] : eqCtrls->volume;
        if (value < 0.0f) value = 0.0f;

        // Map value to bar height
        int barHeight = 0;
        switch (i) {
            case 0: // Bass slider
                barHeight = (int)((value / 8.0f) * barMaxHeight);
                break;

            case FILTER_COUNT: // Volume slider
                barHeight = (int)(value * barMaxHeight);
                break;

            default: // Normal gain sliders
                barHeight = (int)((value / 4.0f) * barMaxHeight);
                break;
        }

        // Draw bar
        int width = (i == FILTER_COUNT) ? barWidth + volumeBarExtra : barWidth;
        int barX = x;
        int barY = barBaseY - barHeight;
        if (i == FILTER_COUNT) {
            // Draw volume bar with border
            u8g2_DrawFrame(&screen->u8g2, barX - 1, barBaseY - barMaxHeight, width + 2, barMaxHeight);
            if (barHeight > 0) {
                u8g2_DrawBox(&screen->u8g2, barX, barY, width, barHeight);
            }
        } else {
            u8g2_DrawBox(&screen->u8g2, barX, barY, width, barHeight);
        }

        // Draw value as integer below bar
        char valStr[8];
        snprintf(valStr, sizeof(valStr), "%.1f", value);
        int strWidth = u8g2_GetStrWidth(&screen->u8g2, valStr);
        int textX = barX + (width - strWidth) / 2;
        u8g2_DrawStr(&screen->u8g2, textX, valueTextY, valStr);

        x += width + barSpacing;
    }

    // Optionally, add a label for the last bar
    u8g2_SetFont(&screen->u8g2, STD_FONT);
    u8g2_DrawStr(&screen->u8g2, screen->DispWidth - 20, 10, "Vol");

    // Send buffer to display
    u8g2_SendBuffer(&screen->u8g2);
}
