/*
 * common.h
 *
 *  Created on: Sep 7, 2017
 *      Author: abhishek.pandey
 */

#ifndef COMMON_H_
#define COMMON_H_

/* init LED's */
void init_led();

/* Init Sensor */
void init_sensor();

/* Sensor output */
uint8_t is_sensor_hit();

/* Time delay function  */
void delay(void);

/* Returns the motor RPM */
uint32_t motor_RPM();

/* Return the the time for 1 D/sec-us-ms this time delay is going to use for turning on
   LED at particular location to make a char */ 
uint32_t time_delay();

/* This function prints the letter with the time delay provided from the time_delay function */
void print_char(char letter,uint32_t delay);

/* Print starting function to print a name on propeller display*/
void print_str(char *str,uint32_t delay);

/* Get hex code to write on a IO line to make a letter */
void get_hex_commands(char letter,char cmds[]);

#endif /* COMMON_H_ */
