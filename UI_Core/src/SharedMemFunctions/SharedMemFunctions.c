#include "SharedMemFunctions.h"

// Functions for managing data within the shared memory

void EQcontrolsInit(volatile EQcontrols *EQctrls)
{
    EQctrls->filterType = STARTUP_FILTYPE;
    for (int i = 0; i < FILTER_COUNT; i++)
        EQctrls->EQgain[i] = STARTUP_GAIN;
    EQctrls->volume = STARTUP_VOLUME;
}

void resetSharedMem()
{
    memset((void*)sharedMem, 0, sizeof(SharedMem));
    Xil_DCacheFlushRange((UINTPTR)sharedMem, sizeof(SharedMem));
    __asm volatile("dsb sy" ::: "memory");
}