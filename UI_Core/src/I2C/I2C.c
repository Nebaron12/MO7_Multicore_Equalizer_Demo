#include "I2C.h"
#include <xstatus.h>

int I2C_Initialize(XIicPs *Iic, uint32_t baseAddress) {
    // Look up the config of the IIC
    XIicPs_Config* Config;
    uint8_t status;
    Config = XIicPs_LookupConfig(baseAddress);
    if (NULL == Config)
    {
        xil_printf("XIicPs_LookupConfig failure\r\n");
        return XST_FAILURE;
    }
    // Initialize the IIC using the config
    status = XIicPs_CfgInitialize(Iic, Config, Config->BaseAddress);
    if (status != XST_SUCCESS)
    {
        xil_printf("XIicPs_CfgInitialize failure\r\n");
        return XST_FAILURE;
    }
    // Do a selftest on the IIC struct to ensure it is working
    status = XIicPs_SelfTest(Iic);
    if (status != XST_SUCCESS)
    {
        xil_printf("IIC selftest FAILED \r\n");
        return XST_FAILURE;
    }
    // Set the clock speed of the IIC bus
    status = XIicPs_SetSClk(Iic, IIC_SCLK_RATE);
    if (status != XST_SUCCESS)
    {
        xil_printf("IIC setClock FAILED \r\n");
        return XST_FAILURE;
    }
    xil_printf("IIC Passed\n");
    return XST_SUCCESS;
}