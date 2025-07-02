#include "OLED.h"


void initDisplay(OLED* screen, XIicPs* Iic)
{
  // Set the IIC instance (Should be already initialized in main)
  screen->Iic = Iic;

	// Setup function for the specific screen
	// Gets all the required data and links the CB function
  u8g2_Setup_ssd1309_i2c_128x64_noname0_f(&screen->u8g2, U8G2_R0, cb_HW_I2C_send, cb_gpio);
  screen->u8g2.u8x8.user_ptr = (void*)screen; // Set the user pointer to the OLED instance
  u8g2_InitDisplay(&screen->u8g2);		// Send init sequence to the display, display is in sleep mode after this,
  u8g2_SetPowerSave(&screen->u8g2, 0);	// Wake up display

  // Clear the buffer and the screen
  u8g2_ClearBuffer(&screen->u8g2);
  u8g2_ClearDisplay(&screen->u8g2);

  u8g2_SetFont(&screen->u8g2, STD_FONT);	// Update the font

  // Get variable data
  screen->DispHeight = u8g2_GetDisplayHeight(&screen->u8g2);
  screen->CentreHeight = screen->DispHeight/2;
  screen->DispWidth = u8g2_GetDisplayWidth(&screen->u8g2);
  screen->CentreWidth = screen->DispWidth/2;
  screen->MaxStrHeight = u8g2_GetMaxCharHeight(&screen->u8g2);
}

void sendBuffer(OLED* screen)
{
  u8g2_SendBuffer(&screen->u8g2);				// Send buffer to the screen
}

void cleanBuffer(OLED* screen)
{
  u8g2_ClearBuffer(&screen->u8g2);            // Clear the buffer for new printing
}

// Internal functions

uint8_t cb_gpio(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  switch(msg)
  {
      case U8X8_MSG_DELAY_NANO:			// delay arg_int * 1 nano second
        break;
      case U8X8_MSG_DELAY_100NANO:		// delay arg_int * 100 nano seconds
        break;
      case U8X8_MSG_DELAY_10MICRO:		// delay arg_int * 10 micro seconds
        break;
      case U8X8_MSG_DELAY_MILLI:			// delay arg_int * 1 milli second
        break;
      case U8X8_MSG_GPIO_D0:				// D0 or SPI clock pin: Output level in arg_int
      //case U8X8_MSG_GPIO_SPI_CLOCK:
        break;
      case U8X8_MSG_GPIO_D1:				// D1 or SPI data pin: Output level in arg_int
      //case U8X8_MSG_GPIO_SPI_DATA:
        break;
      case U8X8_MSG_GPIO_D2:				// D2 pin: Output level in arg_int
        break;
      case U8X8_MSG_GPIO_D3:				// D3 pin: Output level in arg_int
        break;
      case U8X8_MSG_GPIO_D4:				// D4 pin: Output level in arg_int
        break;
      case U8X8_MSG_GPIO_D5:				// D5 pin: Output level in arg_int
        break;
      case U8X8_MSG_GPIO_D6:				// D6 pin: Output level in arg_int
        break;
      case U8X8_MSG_GPIO_D7:				// D7 pin: Output level in arg_int
        break;
      case U8X8_MSG_GPIO_E:				// E/WR pin: Output level in arg_int
        break;
      case U8X8_MSG_GPIO_CS:				// CS (chip select) pin: Output level in arg_int
        break;
      case U8X8_MSG_GPIO_DC:				// DC (data/cmd, A0, register select) pin: Output level in arg_int
        break;
      case U8X8_MSG_GPIO_RESET:			// Reset pin: Output level in arg_int
        break;
      case U8X8_MSG_GPIO_CS1:				// CS1 (chip select) pin: Output level in arg_int
        break;
      case U8X8_MSG_GPIO_CS2:				// CS2 (chip select) pin: Output level in arg_int
        break;
      case U8X8_MSG_GPIO_MENU_SELECT:
        u8x8_SetGPIOResult(u8x8, /* get menu select pin state */ 0);
        break;
      case U8X8_MSG_GPIO_MENU_NEXT:
        u8x8_SetGPIOResult(u8x8, /* get menu next pin state */ 0);
        break;
      case U8X8_MSG_GPIO_MENU_PREV:
        u8x8_SetGPIOResult(u8x8, /* get menu prev pin state */ 0);
        break;
      case U8X8_MSG_GPIO_MENU_HOME:
        u8x8_SetGPIOResult(u8x8, /* get menu home pin state */ 0);
        break;
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
      break;
    case U8X8_MSG_DELAY_I2C:
      break;
    case U8X8_MSG_GPIO_I2C_CLOCK:
      break;
    case U8X8_MSG_GPIO_I2C_DATA:
      break;
    default:
    	u8x8_SetGPIOResult(u8x8, 1);			// default return value
      break;
  }
  return 1;
}

uint8_t cb_HW_I2C_send(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
	  /* u8g2/u8x8 will never send more than 32 bytes between START_TRANSFER and END_TRANSFER */
	  static uint8_t buffer[32];	// buffer that will be send
	  static uint8_t buf_idx;		// index of the buffer above
	  uint8_t *data;				// pointer to the data that needs to be send
    OLED* screen = (OLED*)u8x8->user_ptr; // Get the OLED screen instance from u8x8 struct
    XIicPs* Iic = screen->Iic; // Get the IIC instance from the OLED screen
    //xil_printf("cb_HW_I2C_send: Iic pointer = %p\n", Iic);
	  //XIicPs_Config *Config;		// config of the I2C
	  int status;

	  switch(msg)
	  {
	  	  // Only called once to set up the IIC hardware in the right way
	    case U8X8_MSG_BYTE_INIT:
        // I2C is already initialized in main, so no need to reinitialize
        //xil_printf("U8X8_MSG_BYTE_INIT called, but I2C is already initialized.\n");
	      break;

	      // Collects all the data in a buffer
	    case U8X8_MSG_BYTE_SEND:
	      data = (uint8_t *)arg_ptr;	// load data
	      while( arg_int > 0 )			// continue while there is data
	      {
          buffer[buf_idx++] = *data;
          data++;
          arg_int--;
	      }
	      break;

	    case U8X8_MSG_BYTE_SET_DC:
	      /* ignored for i2c */
	      break;
	    case U8X8_MSG_BYTE_START_TRANSFER:
	      buf_idx = 0;
	      break;

	      // Sends the data over the IIC bus
	    case U8X8_MSG_BYTE_END_TRANSFER:
	    	// Send the data and check if it is received properly
        //xil_printf("U8X8_MSG_BYTE_END_TRANSFER: buf_idx = %d\n", buf_idx);
	    	status = XIicPs_MasterSendPolled(Iic, buffer, buf_idx, SlaveAddress);
        //xil_printf("XIicPs_MasterSendPolled returned %d\n", status);
	    	if (status != XST_SUCCESS)
	    	{
	    		xil_printf("XIicPs_MasterSendPolled failure Status = %d\r\n",status);
	    		return 0;  // Return failure
	    	}
	    	
	        // CRITICAL FIX: Wait for I2C bus to complete transaction
	        // This prevents bus conflicts with other I2C devices (like audio codec)
	        while (XIicPs_BusIsBusy(Iic)) {
	            // Small delay to prevent busy-waiting from consuming too much CPU
	            usleep(1);
	        }
	        
	      break;
	    default:
	      return 0;
	  }
	  return 1;
}