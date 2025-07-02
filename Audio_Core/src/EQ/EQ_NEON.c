#include "EQ_NEON.h"

// Filtering functions for Direct form II using NEON

void init_biquads_neon(BiquadSectionNeon* filt,
                       const float (*num_arr1)[3], const float (*den_arr1)[3], const int *nl_arr1,
                       const float (*num_arr2)[3], const float (*den_arr2)[3], const int *nl_arr2,
                       int num_sect_1, int num_sect_2)
{
    int num_sect = MAX(num_sect_1, num_sect_2);

    for (int i = 0; i < num_sect; i++) {
        // Section order from NL arrays, default to 1 if not provided
        uint8_t ord1 = (i < num_sect_1) ? nl_arr1[i] : 1;
        uint8_t ord2 = (i < num_sect_2) ? nl_arr2[i] : 1;
        uint8_t ord = MAX(ord1, ord2);
        filt[i].order = ord;

        // Get coefficients or pad with zeros
        const float* b1 = (i < num_sect_1) ? num_arr1[i] : (float[3]){1.0f, 0.0f, 0.0f};
        const float* a1 = (i < num_sect_1) ? den_arr1[i] : (float[3]){0.0f, 0.0f, 0.0f};
        const float* b2 = (i < num_sect_2) ? num_arr2[i] : (float[3]){1.0f, 0.0f, 0.0f};
        const float* a2 = (i < num_sect_2) ? den_arr2[i] : (float[3]){0.0f, 0.0f, 0.0f};

        // Numerator coefficients
        filt[i].b[0][0] = filt[i].b[0][1] = b1[0];
        filt[i].b[0][2] = filt[i].b[0][3] = b2[0];
        filt[i].b[1][0] = filt[i].b[1][1] = (ord1 > 1) ? b1[1] : 0.0f;
        filt[i].b[1][2] = filt[i].b[1][3] = (ord2 > 1) ? b2[1] : 0.0f;
        filt[i].b[2][0] = filt[i].b[2][1] = (ord1 > 2) ? b1[2] : 0.0f;
        filt[i].b[2][2] = filt[i].b[2][3] = (ord2 > 2) ? b2[2] : 0.0f;

        // Denominator coefficients (a0 is always 1)
        filt[i].a[0][0] = filt[i].a[0][1] = (ord1 > 1) ? a1[1] : 0.0f;
        filt[i].a[0][2] = filt[i].a[0][3] = (ord2 > 1) ? a2[1] : 0.0f;
        filt[i].a[1][0] = filt[i].a[1][1] = (ord1 > 2) ? a1[2] : 0.0f;
        filt[i].a[1][2] = filt[i].a[1][3] = (ord2 > 2) ? a2[2] : 0.0f;

        // Initialize filter states to 0
        memset(&filt[i].w[0][0], 0, 2 * 4 * sizeof(float));
    }
}

float32x4_t process_biquad_neon(BiquadSectionNeon* section, float32x4_t input) {
    float32x4_t w_new, output;

    if (section->order == 1) {
        // First-order: y[n] = b0 * x[n]
        float32x4_t b0 = vld1q_f32(section->b[0]);
        output = vmulq_f32(b0, input);
    } else if (section->order == 2) {
        // Second-order: y[n] = b0*w[n] + b1*w[n-1]
        float32x4_t w0 = vld1q_f32(section->w[0]);
        float32x4_t a1 = vld1q_f32(section->a[0]);
        float32x4_t b0 = vld1q_f32(section->b[0]);
        float32x4_t b1 = vld1q_f32(section->b[1]);

        w_new = vsubq_f32(input, vmulq_f32(a1, w0));
        float32x4_t b0w = vmulq_f32(b0, w_new);
        float32x4_t b1w = vmulq_f32(b1, w0);
        output = vaddq_f32(b0w, b1w);

        vst1q_f32(section->w[0], w_new);
    } else {
        // Third-order (or higher): original implementation
        float32x4_t w0 = vld1q_f32(section->w[0]);
        float32x4_t w1 = vld1q_f32(section->w[1]);
        float32x4_t a1 = vld1q_f32(section->a[0]);
        float32x4_t a2 = vld1q_f32(section->a[1]);
        float32x4_t b0 = vld1q_f32(section->b[0]);
        float32x4_t b1 = vld1q_f32(section->b[1]);
        float32x4_t b2 = vld1q_f32(section->b[2]);

        float32x4_t a1w0 = vmulq_f32(a1, w0);
        float32x4_t a2w1 = vmulq_f32(a2, w1);
        float32x4_t a1w0_plus_a2w1 = vaddq_f32(a1w0, a2w1);
        w_new = vsubq_f32(input, a1w0_plus_a2w1);

        float32x4_t b0w = vmulq_f32(b0, w_new);
        float32x4_t b1w = vmulq_f32(b1, w0);
        float32x4_t b2w = vmulq_f32(b2, w1);
        output = vaddq_f32(vaddq_f32(b0w, b1w), b2w);

        vst1q_f32(section->w[1], w0);
        vst1q_f32(section->w[0], w_new);
    }

    return output;
}

float32x4_t process_iir_neon(BiquadSectionNeon* filt, float32x4_t input, int num_sect) {
    float32x4_t x = input;

    for (int i = 0; i < num_sect; ++i) {
        x = process_biquad_neon(&filt[i], x);
    }

    return x;
}

// For the 4 band MO7 SID equalizer
// Make sure to change this function to match your number of filters
// void apply_equalizer_neon(EQFilterState* eqState, float in_left, float in_right, float* out_left, float* out_right, EQcontrols* EQctrls)
// {
//     // Interleaved input: [L, R, L, R]
//     float input_arr[4] = {in_left, in_right, in_left, in_right};
//     float32x4_t input = vld1q_f32(input_arr);

//     float32x4_t AB = process_iir_neon(eqState->filterArrays[0], input, eqState->maxSectionsArray[0]);
//     float32x4_t CD = process_iir_neon(eqState->filterArrays[1], input, eqState->maxSectionsArray[1]);

//     // Extract each float from the NEON vectors
//     float ab0 = vgetq_lane_f32(AB, 0);
//     float ab1 = vgetq_lane_f32(AB, 1);
//     float ab2 = vgetq_lane_f32(AB, 2);
//     float ab3 = vgetq_lane_f32(AB, 3);

//     float cd0 = vgetq_lane_f32(CD, 0);
//     float cd1 = vgetq_lane_f32(CD, 1);
//     float cd2 = vgetq_lane_f32(CD, 2);
//     float cd3 = vgetq_lane_f32(CD, 3);

//     // Compute left and right outputs
//     *out_left =    (ab0 * EQctrls->EQgain[0] +
//                     ab2 * EQctrls->EQgain[1] +
//                     cd0 * EQctrls->EQgain[2] +
//                     cd2 * EQctrls->EQgain[3]);

//     *out_right =   (ab1 * EQctrls->EQgain[0] +
//                     ab3 * EQctrls->EQgain[1] +
//                     cd1 * EQctrls->EQgain[2] +
//                     cd3 * EQctrls->EQgain[3]);
// }



// EQFilterState* EQ_filters_init(void) {
//     EQFilterState* state = (EQFilterState*)malloc(sizeof(EQFilterState));
//     if (!state) return NULL;

//     state->numFilterPairs = (FILTER_COUNT + 1) / 2;
//     state->maxSectionsArray = (uint8_t*)malloc(state->numFilterPairs * sizeof(uint8_t));
//     if (!state->maxSectionsArray) { free(state); return NULL; }

//     // Calculate max sections for each filter pair
//     state->maxSectionsArray[0] = MAX(MWSPT_NSEC_A, MWSPT_NSEC_B);
//     state->maxSectionsArray[1] = MAX(MWSPT_NSEC_C, MWSPT_NSEC_D);

//     state->filterArrays = (BiquadSectionNeon**)malloc(state->numFilterPairs * sizeof(BiquadSectionNeon*));
//     if (!state->filterArrays) { free(state->maxSectionsArray); free(state); return NULL; }

//     // Allocate and initialize each filter array
//     state->filterArrays[0] = (BiquadSectionNeon*)malloc(state->maxSectionsArray[0] * sizeof(BiquadSectionNeon));
//     state->filterArrays[1] = (BiquadSectionNeon*)malloc(state->maxSectionsArray[1] * sizeof(BiquadSectionNeon));

//     if (!state->filterArrays[0] || !state->filterArrays[1]) {
//         for (size_t i = 0; i < state->numFilterPairs; ++i)
//             if (state->filterArrays[i]) free(state->filterArrays[i]);
//         free(state->filterArrays);
//         free(state->maxSectionsArray);
//         free(state);
//         xil_printf("Failed to allocate memory for filter arrays.\n\r");
//         return NULL;
//     }

//     init_biquads_neon(state->filterArrays[0],
//          NUM_A, DEN_A, NL_A, 
//          NUM_B, DEN_B, NL_B, 
//          MWSPT_NSEC_A, MWSPT_NSEC_B);
//     init_biquads_neon(state->filterArrays[1],
//          NUM_C, DEN_C, NL_C, 
//          NUM_D, DEN_D, NL_D, 
//          MWSPT_NSEC_C, MWSPT_NSEC_D);

//     return state;
// }


// For the 7 band equalizer
void apply_equalizer_neon(EQFilterState* eqState, float in_left, float in_right, float* out_left, float* out_right, volatile EQcontrols* EQctrls)
{
    // Interleaved input: [L, R, L, R]
    float input_arr[4] = {in_left, in_right, in_left, in_right};
    float32x4_t input = vld1q_f32(input_arr);

    float32x4_t AB = process_iir_neon(eqState->filterArrays[0], input, eqState->maxSectionsArray[0]);
    float32x4_t CD = process_iir_neon(eqState->filterArrays[1], input, eqState->maxSectionsArray[1]);
    float32x4_t EF = process_iir_neon(eqState->filterArrays[2], input, eqState->maxSectionsArray[2]);
    float32x4_t G = process_iir_neon(eqState->filterArrays[3], input, eqState->maxSectionsArray[3]);

    // Extract each float from the NEON vectors
    float ab0 = vgetq_lane_f32(AB, 0);
    float ab1 = vgetq_lane_f32(AB, 1);
    float ab2 = vgetq_lane_f32(AB, 2);
    float ab3 = vgetq_lane_f32(AB, 3);

    float cd0 = vgetq_lane_f32(CD, 0);
    float cd1 = vgetq_lane_f32(CD, 1);
    float cd2 = vgetq_lane_f32(CD, 2);
    float cd3 = vgetq_lane_f32(CD, 3);

    float ef0 = vgetq_lane_f32(EF, 0);
    float ef1 = vgetq_lane_f32(EF, 1);
    float ef2 = vgetq_lane_f32(EF, 2);
    float ef3 = vgetq_lane_f32(EF, 3);

    float g0 = vgetq_lane_f32(G, 0);
    float g1 = vgetq_lane_f32(G, 1);
    float g2 = vgetq_lane_f32(G, 2);
    float g3 = vgetq_lane_f32(G, 3);

    // Compute left and right outputs
    *out_left =    (ab0 * EQctrls->EQgain[0] +
                    ab2 * EQctrls->EQgain[1] +
                    cd0 * EQctrls->EQgain[2] +
                    cd2 * EQctrls->EQgain[3] +
                    ef0 * EQctrls->EQgain[4] +
                    ef2 * EQctrls->EQgain[5] +
                    g0 * EQctrls->EQgain[6]);

    *out_right =   (ab1 * EQctrls->EQgain[0] +
                    ab3 * EQctrls->EQgain[1] +
                    cd1 * EQctrls->EQgain[2] +
                    cd3 * EQctrls->EQgain[3] +
                    ef1 * EQctrls->EQgain[4] +
                    ef3 * EQctrls->EQgain[5] +
                    g1 * EQctrls->EQgain[6]);
}

EQFilterState* EQ_filters_init(void) {
    EQFilterState* state = (EQFilterState*)malloc(sizeof(EQFilterState));
    if (!state) return NULL;

    state->numFilterPairs = (FILTER_COUNT + 1) / 2;
    state->maxSectionsArray = (uint8_t*)malloc(state->numFilterPairs * sizeof(uint8_t));
    if (!state->maxSectionsArray) { free(state); return NULL; }

    // Calculate max sections for each filter pair
    state->maxSectionsArray[0] = MAX(MWSPT_NSEC_A, MWSPT_NSEC_B);  // A-B pair
    state->maxSectionsArray[1] = MAX(MWSPT_NSEC_C, MWSPT_NSEC_D);  // C-D pair
    state->maxSectionsArray[2] = MAX(MWSPT_NSEC_E, MWSPT_NSEC_F);  // E-F pair
    state->maxSectionsArray[3] = MWSPT_NSEC_G;                      // G alone

    state->filterArrays = (BiquadSectionNeon**)malloc(state->numFilterPairs * sizeof(BiquadSectionNeon*));
    if (!state->filterArrays) { free(state->maxSectionsArray); free(state); return NULL; }

    // Allocate and initialize each filter array
    state->filterArrays[0] = (BiquadSectionNeon*)malloc(state->maxSectionsArray[0] * sizeof(BiquadSectionNeon));
    state->filterArrays[1] = (BiquadSectionNeon*)malloc(state->maxSectionsArray[1] * sizeof(BiquadSectionNeon));
    state->filterArrays[2] = (BiquadSectionNeon*)malloc(state->maxSectionsArray[2] * sizeof(BiquadSectionNeon));
    state->filterArrays[3] = (BiquadSectionNeon*)malloc(state->maxSectionsArray[3] * sizeof(BiquadSectionNeon));

    if (!state->filterArrays[0] || !state->filterArrays[1] || !state->filterArrays[2] || !state->filterArrays[3]) {
        for (size_t i = 0; i < state->numFilterPairs; ++i)
            if (state->filterArrays[i]) free(state->filterArrays[i]);
        free(state->filterArrays);
        free(state->maxSectionsArray);
        free(state);
        xil_printf("Failed to allocate memory for filter arrays.\n\r");
        return NULL;
    }

    // Initialize filter pairs
    init_biquads_neon(state->filterArrays[0],
         NUM_A, DEN_A, NL_A, 
         NUM_B, DEN_B, NL_B, 
         MWSPT_NSEC_A, MWSPT_NSEC_B);
    init_biquads_neon(state->filterArrays[1],
         NUM_C, DEN_C, NL_C, 
         NUM_D, DEN_D, NL_D, 
         MWSPT_NSEC_C, MWSPT_NSEC_D);
    init_biquads_neon(state->filterArrays[2],
         NUM_E, DEN_E, NL_E, 
         NUM_F, DEN_F, NL_F, 
         MWSPT_NSEC_E, MWSPT_NSEC_F);
    
    // For the single filter G, we use the same filter for both channels
    init_biquads_neon(state->filterArrays[3],
         NUM_G, DEN_G, NL_G, 
         NUM_G, DEN_G, NL_G, 
         MWSPT_NSEC_G, MWSPT_NSEC_G);

    return state;
}

void EQ_filters_free(EQFilterState* state) {
    if (!state) return;
    for (size_t i = 0; i < state->numFilterPairs; ++i)
        free(state->filterArrays[i]);
    free(state->filterArrays);
    free(state->maxSectionsArray);
    free(state);
}