#include "tla2528.h"

void TLA2528_Init(TLA2528 *dev, XIicPs *Iic, uint16_t address) {
    dev->Iic = Iic;
    dev->Address = address;

    uint8_t status = 0;

    // Reset the device
    status |= TLA2528_WriteRegister(dev, SYSTEM_STATUS_ADDRESS, 0x01);
    usleep(1000); // Wait for reset to complete

    // 1. Set all pins to analog input
    status |= TLA2528_WriteRegister(dev, PIN_CFG_ADDRESS, PIN_CFG_DEFAULT);

    // 2. Set OSR to 128x (highest oversampling)
    status |= TLA2528_WriteRegister(dev, OSR_CFG_ADDRESS, OSR_128);

    // 3. Set OPMODE to high speed, no clock divider
    status |= TLA2528_WriteRegister(dev, OPMODE_CFG_ADDRESS, OPMODE_CFG_DEFAULT);

    // 4. Enable all 8 channels for auto-sequence
    status |= TLA2528_WriteRegister(dev, AUTO_SEQ_CH_SEL_ADDRESS, AUTO_SEQ_CH_SEL_MASK);

    // 5. Trigger calibration
    status |= TLA2528_WriteRegister(dev, GENERAL_CFG_ADDRESS, CAL_START);

    // 6. Wait for calibration to complete (poll CAL bit to clear)
    uint8_t gen_cfg = 0;
    int timeout = 1000; // avoid infinite loop
    do {
        TLA2528_ReadRegister(dev, GENERAL_CFG_ADDRESS, &gen_cfg);
        usleep(1000); // 1ms delay
    } while ((gen_cfg & CAL_MASK) && --timeout);

    // 7. Set sequencer to auto mode and start it
    status |= TLA2528_WriteRegister(dev, SEQUENCE_CFG_ADDRESS, SEQ_MODE_AUTO | SEQ_START_ASSEND);
    usleep(1000); // Give time for sequencer to start

    if (status != XST_SUCCESS) {
        xil_printf("Failed to write to TLA2528 registers\n");
    }
}

int TLA2528_ReadRegister(TLA2528 *dev, uint8_t reg_addr, uint8_t *data) {
    uint8_t tx[2] = {0x10, reg_addr};
    int status;

    status = XIicPs_MasterSendPolled(dev->Iic, tx, 2, dev->Address);
    if (status != XST_SUCCESS) return status;

    while (XIicPs_BusIsBusy(dev->Iic));
    status = XIicPs_MasterRecvPolled(dev->Iic, data, 1, dev->Address);
    return status;
}

int TLA2528_WriteRegister(TLA2528 *dev, uint8_t reg_addr, uint8_t data) {
    uint8_t tx[3] = {0x08, reg_addr, data};
    int status;

    status = XIicPs_MasterSendPolled(dev->Iic, tx, 3, dev->Address);
    if (status != XST_SUCCESS) return status;
    
    // Wait for I2C transaction to complete
    while (XIicPs_BusIsBusy(dev->Iic));
    
    return status;
}

// Reads all 8 channels in auto-sequence mode (only returns 12 bit values)
int TLA2528_ReadAllChannels(TLA2528 *dev, uint16_t* data) {
    // The TLA2528 outputs 2 bytes per channel, 8 channels = 16 bytes
    // The data register address for reading conversion results is typically 0x00
    uint8_t reg_addr = 0x00; // Replace with correct data register if needed
    uint8_t rx[16] = {0};
    int status;

    // Send register address to read from
    status = XIicPs_MasterSendPolled(dev->Iic, &reg_addr, 1, dev->Address);
    if (status != XST_SUCCESS) return status;

    while (XIicPs_BusIsBusy(dev->Iic));

    // Read 16 bytes (8 channels x 2 bytes)
    status = XIicPs_MasterRecvPolled(dev->Iic, rx, 16, dev->Address);
    if (status != XST_SUCCESS) return status;

    // Convert bytes to 16-bit values (MSB first)
    for (int i = 0; i < 8; ++i) {
        data[i] = ((uint16_t)rx[2*i] << 4) | rx[2*i+1] >> 4; // Right align to 12 bits
    }

    return XST_SUCCESS;
}

// Reads all 8 channels in auto-sequence mode (returns 16 bit values) (could not get this to work)
/*int TLA2528_ReadAllChannels(TLA2528 *dev, uint16_t* data, uint8_t baseAddress) {
    // The TLA2528 outputs 2 bytes per channel, 8 channels = 16 bytes
    // The data register address for reading conversion results is typically 0x00
    uint8_t tx[2] = {0x30, baseAddress}; // RECENT_CHx base address (0x20 for AIN0_LSB, 0x21 for AIN0_MSB, etc.)
    uint8_t rx[16] = {0};
    int status;

    // Send opcode and first register address to read from
    status = XIicPs_MasterSendPolled(dev->Iic, tx, 2, dev->Address);
    if (status != XST_SUCCESS) return status;

    // Then read out 16 registers (8 channels x 2 bytes)
    while (XIicPs_BusIsBusy(dev->Iic));
    status = XIicPs_MasterRecvPolled(dev->Iic, rx, 16, dev->Address);
    if (status != XST_SUCCESS) return status;

    // Convert bytes to 16-bit values (MSB first)
    for (int i = 0; i < 8; ++i) {
        data[i] = ((uint16_t)rx[2*i] << 8) | rx[2*i+1];
    }

    return XST_SUCCESS;
}*/