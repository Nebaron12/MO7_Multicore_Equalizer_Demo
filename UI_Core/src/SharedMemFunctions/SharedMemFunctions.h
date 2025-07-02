#include "SharedMem.h"

#include <xil_cache.h>
#include <string.h>


// Functions
void resetSharedMem();
void EQcontrolsInit(volatile EQcontrols *EQctrls);