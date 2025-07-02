#ifndef __COEFFS_H_
#define __COEFFS_H_

// For the 4 band MO7 SID equalizer
// Low pass frequency filter coefficients
// #define MWSPT_NSEC_A 5
// extern const int NL_A[MWSPT_NSEC_A];
// extern const float NUM_A[MWSPT_NSEC_A][3];
// extern const int DL_A[MWSPT_NSEC_A];
// extern const float DEN_A[MWSPT_NSEC_A][3];

// // Low-high frequency filter coefficients
// #define MWSPT_NSEC_B 7
// extern const int NL_B[MWSPT_NSEC_B];
// extern const float NUM_B[MWSPT_NSEC_B][3];
// extern const int DL_B[MWSPT_NSEC_B];
// extern const float DEN_B[MWSPT_NSEC_B][3];

// // Mid-high frequency filter coefficients
// #define MWSPT_NSEC_C 9
// extern const int NL_C[MWSPT_NSEC_C];
// extern const float NUM_C[MWSPT_NSEC_C][3];
// extern const int DL_C[MWSPT_NSEC_C];
// extern const float DEN_C[MWSPT_NSEC_C][3];

// // High pass filter coefficients
// #define MWSPT_NSEC_D 7
// extern const int NL_D[MWSPT_NSEC_D];
// extern const float NUM_D[MWSPT_NSEC_D][3];
// extern const int DL_D[MWSPT_NSEC_D];
// extern const float DEN_D[MWSPT_NSEC_D][3];


// For the 7 band equalizer
// A
#define MWSPT_NSEC_A 7
extern const int NL_A[MWSPT_NSEC_A];
extern const float NUM_A[MWSPT_NSEC_A][3];
extern const int DL_A[MWSPT_NSEC_A];
extern const float DEN_A[MWSPT_NSEC_A][3];

// B
#define MWSPT_NSEC_B 13
extern const int NL_B[MWSPT_NSEC_B];
extern const float NUM_B[MWSPT_NSEC_B][3];
extern const int DL_B[MWSPT_NSEC_B];
extern const float DEN_B[MWSPT_NSEC_B][3];

// C
#define MWSPT_NSEC_C 13
extern const int NL_C[MWSPT_NSEC_C];
extern const float NUM_C[MWSPT_NSEC_C][3];
extern const int DL_C[MWSPT_NSEC_C];
extern const float DEN_C[MWSPT_NSEC_C][3];

// D
#define MWSPT_NSEC_D 13
extern const int NL_D[MWSPT_NSEC_D];
extern const float NUM_D[MWSPT_NSEC_D][3];
extern const int DL_D[MWSPT_NSEC_D];
extern const float DEN_D[MWSPT_NSEC_D][3];

// E
#define MWSPT_NSEC_E 13
extern const int NL_E[MWSPT_NSEC_E];
extern const float NUM_E[MWSPT_NSEC_E][3];
extern const int DL_E[MWSPT_NSEC_E];
extern const float DEN_E[MWSPT_NSEC_E][3];

// F
#define MWSPT_NSEC_F 13
extern const int NL_F[MWSPT_NSEC_F];
extern const float NUM_F[MWSPT_NSEC_F][3];
extern const int DL_F[MWSPT_NSEC_F];
extern const float DEN_F[MWSPT_NSEC_F][3];

// G
#define MWSPT_NSEC_G 13
extern const int NL_G[MWSPT_NSEC_G];
extern const float NUM_G[MWSPT_NSEC_G][3];
extern const int DL_G[MWSPT_NSEC_G];
extern const float DEN_G[MWSPT_NSEC_G][3];

#endif