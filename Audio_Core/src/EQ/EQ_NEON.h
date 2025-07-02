#ifndef __EQ_NEON_H_
#define __EQ_NEON_H_

#include <stdint.h>
#include <arm_neon.h>
#include <string.h>
#include <xil_printf.h>
#include <stdlib.h>

#include "SharedMem.h"
#include "Coeffs.h"

// Macros

#define MAX(a,b) (((a)>(b))?(a):(b))

// Structs

// Biquadsection for direct form II using NEON
typedef struct {
    float b[3][4];  // Coefficients for the numerator
    float a[2][4];  // Coefficients for the denominator (a0 is always 1, so it's omitted)
    float w[2][4];  // Delay buffer (stores intermediate state w[n-1], w[n-2])
    uint8_t order;  // Number indicating the section order (1, 2 or 3)
} BiquadSectionNeon;

// Struct for referencing all filters cleanly
typedef struct {
    uint8_t* maxSectionsArray; // Array of maxSectionCount variables for each filter
    BiquadSectionNeon** filterArrays; // Array of pointers to filter arrays
    size_t numFilterPairs; // Number of filter pairs within the equalizer
} EQFilterState;

// Function prototypes
void init_biquads_neon(BiquadSectionNeon* filt,
                       const float (*num_arr1)[3], const float (*den_arr1)[3], const int *nl_arr1,
                       const float (*num_arr2)[3], const float (*den_arr2)[3], const int *nl_arr2,
                       int num_sect_1, int num_sect_2);
float32x4_t process_biquad_neon(BiquadSectionNeon* section, float32x4_t input);
float32x4_t process_iir_neon(BiquadSectionNeon* filt, float32x4_t input, int num_sect);
void apply_equalizer_neon(EQFilterState* eqState, float in_left, float in_right, float* out_left, float* out_right, volatile EQcontrols* EQctrls);
EQFilterState* EQ_filters_init(void);
void EQ_filters_free(EQFilterState* state);

#endif