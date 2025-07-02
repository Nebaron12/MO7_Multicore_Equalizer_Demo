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
    // Initialize shared memory with proper synchronization
    // First, ensure the state is 0 to signal audio core to wait
    sharedMem->state = 0;
    Xil_DCacheFlushRange((UINTPTR)&sharedMem->state, sizeof(sharedMem->state));
    __asm volatile("dsb sy" ::: "memory");
    
    // Now initialize the rest of shared memory
    memset((void*)&sharedMem->eqControls, 0, sizeof(EQcontrols));
    
    // Flush the entire shared memory region
    Xil_DCacheFlushRange((UINTPTR)sharedMem, sizeof(SharedMem));
    __asm volatile("dsb sy" ::: "memory");
    
    xil_printf("Shared memory reset complete\n");
}