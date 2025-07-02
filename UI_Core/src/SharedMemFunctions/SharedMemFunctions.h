#include "SharedMem.h"

#include <xil_cache.h>
#include <string.h>
#include <xil_printf.h>


// Functions
void resetSharedMem();
void EQcontrolsInit(volatile EQcontrols *EQctrls);