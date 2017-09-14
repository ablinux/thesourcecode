/* ###################################################################
 **     Filename    : main.c
 **     Project     : propeller_clock
 **     Processor   : MKL25Z128VLK4
 **     Version     : Driver 01.01
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2017-08-29, 13:03, # CodeGen: 0
 **     Abstract    :
 **         Main module.
 **         This module contains user's application code.
 **     Settings    :
 **     Contents    :
 **         No public methods
 **
 ** ###################################################################*/

/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "WAIT1.h"
#include "MCUC1.h"
#include "Byte1.h"
#include "BitsIoLdd1.h"
#include "Bit1.h"
#include "BitIoLdd1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "common.h"
#include "ascii.h"


#define M_WAIT_TIME (2)

int main(void)
{
	PE_low_level_init();

	volatile uint32_t rpm_val = 0,delay = 0;
	
	init_led();// LED init

	init_sensor();// Sensor ON
	
	/* By this time motor is running, so batter to calculate a its RPM now */
	rpm_val = motor_RPM();

	delay = rpm_val/360;

	while(1)
	{
		/* By this time Motor rotation might have changed so again calculate the delay value 
  		to make it robust */
		Byte1_PutVal(0xff);
		rpm_val = motor_RPM();
		delay = rpm_val/360;
		print_char('a',delay);

	}/* end of while 1 */
} 
/****************************************************************************************************
 * Functions : Print_char
 *
 *
 ***************************************************************************************************/
void print_char(char letter, uint32_t delay)
{
	/** char hex_val_set[3] this will hold the value of requested ASCII char
	 * here the resolution of char is 7x3 where 7 is the vertical led's and 3 is the widht 
	 * of the char
	 * it looks some thing like this 
	 * --------
	 *           |--4--|
	 *       -   #	   #	
	 *       |   #	   #
	 *       |   #     #
	 *       7   # # # #
	 *       |   #     #
	 *       |   #     #
	 *       -   #     #
	 *           -------- This is a letter H that is going to print on the propeller
	 * Each element in the array can hold 8bit of data hence the 8 or 7 LED, and we have to
	 * print this as per the resolution of the char here is choose 3 hence the char array is of 3
	 */ 
	uint8_t index = 0;
//	index = letter - 'A'; 
	uint8_t i = 0;
	/** A is the first in ASCII_ARRY to get the other char index we need
	 * substrac from the A to get the index for ex to get the index value of C
	 * index = C - A;
	 * index will have 2;
	 */
	/* Search for the hex value to write on the io lines */
	while(i<4)
	{
		WAIT1_Waitms(delay);
		Byte1_PutVal(~(ASCII_ARRY[index][i]));
		WAIT1_Waitms(delay);
		i++;
	}
}

/****************************************************************************************************
 * Functions : Print_str
 *
 *
 ***************************************************************************************************/
void print_str(char *str, uint32_t delay)
{
	//TODO :- add logic here
}

/****************************************************************************************************
 * Functions : motor_RPM
 *
 *
 ***************************************************************************************************/
uint32_t motor_RPM()
{
	uint32_t rpm_val = 0;
	uint8_t hit=0;
	/** inc the delay count and wait for the sensot hit  */
	while(1)
	{
		hit = is_sensor_hit()+ hit;
		if(hit==1)
		{
			WAIT1_Waitms(M_WAIT_TIME);
			rpm_val++;
		}
		else if(hit ==2)
		{
			break;
		}
	}
	/* As the sensor hits means that one revolution is complete
	 * Return the time in micro sec to the calleee */ 
	return rpm_val;
}

/****************************************************************************************************
 * Functions : init_led
 *
 *
 ***************************************************************************************************/
void init_led()
{
	/** Set port PTC as a output */
	Byte1_SetDir(1);
}

/****************************************************************************************************
 * Functions : init_sensor
 *
 *
 ***************************************************************************************************/
void init_sensor()
{
	/* Set PTE0 as a input */
	Bit1_SetDir(0);
}
/****************************************************************************************************
 * Functions : is_sensor_hit
 *
 *
 ***************************************************************************************************/
uint8_t is_sensor_hit()
{
	/** returns 1 if sensor hit else return 0 */
	return Bit1_GetVal();
}
