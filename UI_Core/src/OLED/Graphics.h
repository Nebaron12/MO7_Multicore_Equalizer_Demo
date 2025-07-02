#ifndef __GRAPHICS_H_
#define __GRAPHICS_H_

#include <stdio.h>
#include "OLED.h"
#include "SharedMemFunctions.h"


/*--------------------------------------------------------------
* Application specific functions
* These functions are for displaying equalizer information
* No returns
---------------------------------------------------------------*/
void oledRenderEqualizer(OLED* screen, const volatile EQcontrols* eqCtrls);


#endif