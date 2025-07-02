#ifndef OLED_H
#define OLED_H

#include "xiicps.h"

#include "I2Csrc/u8g2.h"

// The defines for the SSD1309 screen
#define SlaveAddress 0x3c //0x3c default// Address to which the data is sent
#define IIC_SCLK_RATE 400000			// the clock rate of the screen and IIC
#define STD_FONT u8g2_font_t0_11_me		// The font used to print normal text
#define WRAP_FONT u8g2_font_6x10_tf     // The font used for wrapped printing
#define EQ_FONT u8g2_font_5x7_mf        // The font used for printing the EQ information

// OLED screen struct
typedef struct {
    XIicPs* Iic;
    u8g2_t u8g2;
    u8g2_uint_t DispHeight, DispWidth, CentreHeight, CentreWidth;
    int8_t MaxStrHeight;
} OLED;

/*--------------------------------------------------------------
 * Function for initializing and configuring the display
 * Saves the screen data and links the different CB functions
 * No returns
 ---------------------------------------------------------------*/
void initDisplay(OLED* screen, XIicPs* iic);

void sendBuffer(OLED* screen);

void cleanBuffer(OLED* screen);


/*-----------------------------------------------------------
 * Callback function that the U8G2 library uses
 * Takes care of the delays and initializing the gpio
 * Because HW IIC is used this function has no real meaning
 * input:
 * 		*u8x8		pointer to the structure with the data
 * 		msg			the function that the callback function has to do
 * 		arg_int		length in char of the data that needs to be send
 * 		*arg_ptr	pointer to the data that needs to be send
 -----------------------------------------------------------*/
uint8_t cb_gpio(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);


/*--------------------------------------------------------------
 * Callback function that the U8G2 library uses
 * This function is the communication between the U8G2 library
 * and the HW IIC library of the PYNQ-Z2
 * It initializes the IIC, formats the data so it is transferable
 * and gives the command to the IIC library to send the data
 * input:
 * 		*u8x8		pointer to the structure with the data
 * 		msg			the function that the callback function has to do
 * 		arg_int		length in char of the data that needs to be send
 * 		*arg_ptr	pointer to the data that needs to be send
 * return:
 * 		succes or failure of the function
 */
uint8_t cb_HW_I2C_send(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);


#endif