#ifndef __SHAREDMEM_H_
#define __SHAREDMEM_H_

// Includes
#include <stdint.h>

// Place any defines, data structs and types that must be shared between cores below

// EQ controls structure
#define FILTER_COUNT 7
#define STARTUP_VOLUME 0.2f
#define STARTUP_GAIN 1.00f
#define STARTUP_FILTYPE 1
typedef struct {
    float volume;                   // Volume gain applied before sample is sent to codec
    float EQgain[FILTER_COUNT];     // All Equalizer filter gains in an array
    uint8_t filterType;             // Currently used filter type
} EQcontrols;





// Shared memory structure containing all shared data
typedef struct {
    EQcontrols eqControls; // EQ controls structure
    uint8_t state;
    // Add other shared data structures as needed
} SharedMem;

// Extern instance of shared memory
#define SHARED_MEM_BASE 0x100000
#define sharedMem ((volatile SharedMem*)SHARED_MEM_BASE)

#endif