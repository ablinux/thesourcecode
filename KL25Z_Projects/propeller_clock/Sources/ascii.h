/*
 * ascii.h
 *
 *  Created on: Sep 7, 2017
 *      Author: abhishek.pandey
 */

#ifndef ASCII_H_
#define ASCII_H_

#define LETTERS 26+9 //26 English and 9 number
#define CHAR_WIDTH 3 // Currently 3 but i will increase that TODO : inc the char array

char ASCII_ARRY[LETTERS][4]=
{
		{0x7f,0x18,0x18,0x7f}, //<----- H
		{0x45,0x45,0x42,0xaa},
		{0x45,0x45,0x42,0xaa},
		{0x45,0x45,0x42,0xaa},
		//<-- up to z and than from 0 to 9
		// TODO update the table from the xl sheet 
};


#endif /* ASCII_H_ */
