/*
 * HW_IO_PORT.h
 *
 *  Created on: Apr 21, 2017
 *      Author: abhishek.pandey
 */

#ifndef HW_IO_PORT_H_
#define HW_IO_PORT_H_
#include "io_driver_api_kl25z.h"

static SIM_MemMapPtr SIM = SIM_BASE_PTR;
static PORT_MemMapPtr PORT;
static inline void HW_IOPORT_ENABLE(enable_port_t port)
{
	SIM->SCGC5 = port;
}

static inline void HW_PORT_DIR_SET()
{
	
}

#endif /* HW_IO_PORT_H_ */
