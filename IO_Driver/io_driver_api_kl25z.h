/*
 * io_driver_api_kl25z.h
 *
 *  Created on: Apr 21, 2017
 *      Author: abhishek.pandey
 */

#ifndef IO_DRIVER_API_KL25Z_H_
#define IO_DRIVER_API_KL25Z_H_

#include <stdint.h>
#include <MKL25Z4.h>
/* structures */
typedef enum port
{
	PORT_A = 0x200,
	PORT_B = 0x400,
	PORT_C = 0x800,
	PORT_D = 0x1000,
	PORT_E = 0x2000,
}enable_port_t;

/* Driver API's */
uint8_t port_enable(enable_port_t); //> init the specific port to save power
uint8_t port_write();        //> Write the value on the port
uint8_t port_set_direction();
uint8_t port_read();         //> Read the value on the port
uint8_t pin_write();         //> Write the value on the pin
uint8_t pin_read();          //> Read the value on the port

#endif /* IO_DRIVER_API_KL25Z_H_ */
