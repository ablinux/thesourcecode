/*
 * io_driver_kl25z.c
 *
 *  Created on: Apr 21, 2017
 *      Author: abhishek.pandey
 */
#include "io_driver_api_kl25z.h"
#include "HW_IO_PORT.h"
/**********************************************************************************************************************
 * Function:port_enable();
 * 
 * 
 **********************************************************************************************************************/
uint8_t port_enable(port_t PORT)
{
	uint8_t ret_val = 0;
	/** Enable the clock to the specific PORTx */
	HW_IOPORT_ENABLE(PORT);
	
	return ret_val;
}
/**********************************************************************************************************************
 * Function:port_set_direction();
 * 
 * 
 **********************************************************************************************************************/
uint8_t port_set_direction()
{
	uint8_t ret_val = 0;
	HW_PORT_DIR_SET();
}
