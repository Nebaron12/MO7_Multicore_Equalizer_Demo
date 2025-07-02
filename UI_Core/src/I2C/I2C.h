#ifndef __I2C_H_
#define __I2C_H_

#include <stdint.h>
#include <xparameters.h>
#include <xiicps.h>

// I2C peripheral configuration
#define IIC_SCLK_RATE   400000			            // The IIC clock speed in Hz

// Function prototypes
int I2C_Initialize(XIicPs *Iic, uint32_t baseAddress);

#endif