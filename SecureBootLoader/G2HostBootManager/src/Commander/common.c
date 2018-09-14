/*
 * common.c
 *
 *  Created on: Sep 14, 2018
 *      Author: abhishek.pandey
 */

#include "common.h"
#include "stdio.h"

static FILE *fp;


void welcome()
{
	printf("\n +===============================================+");
	printf("\n |  DOWNLOADER Serial XCP interface (Win32 API)  |");
	printf("\n +===============================================+\n");
}
static char const * GetLineTrailerByPercentage(uint8_t percentage)
{
  char const * result;
  /* Note that the following string was declared static to guarantee that the pointer
   * stays valid and can be used by the caller of this function.
   */
  static char trailerStrPct[32] = "";

  /* Construct the trailer. */
  sprintf(trailerStrPct, "[" OUTPUT_YELLOW "%3hhu%%" OUTPUT_RESET "]", percentage);
  /* Set the result. */
  result = &trailerStrPct[0];
  /* Give the result back to the caller. */
  return result;
} /*** end of GetLineTrailerByPercentage ***/
void show_menu()
{
	puts("1. Download New bin to system\n"
	     "2. Get Target system information\n"
		 "3. Press enter to inn to the target\n\r");
}

uint32_t command_handler(uint8_t command)
{
	switch(command)
	{
	case '1':
	{
		uint8_t file_location_str[300];
		uint32_t file_size;
		uint32_t err;
		fflush(stdin);
		/* We are in downloader */
		puts("Enter the location of bin file : ");gets(file_location_str);
		/* open the file */
		fp = fopen(file_location_str,"rb"); if(fp == NULL){puts("Error in file open \n");exit(0);}
		/* Get the size of file */
		fseek (fp , 0 , SEEK_END);  file_size = ftell (fp); rewind (fp);
		printf("File Size : %d\n",file_size);

		/* Send Downloader a command to enable */
		err = serial_write(DOWNLOAD_IMAGE, 1); if(err){puts("Error serial write \n"); exit(0);}

	}
	}
}
