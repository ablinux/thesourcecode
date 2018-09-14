/*
 * common.h
 *
 *  Created on: Sep 14, 2018
 *      Author: abhishek.pandey
 */

#ifndef COMMANDER_COMMON_H_
#define COMMANDER_COMMON_H_

#if defined (PLATFORM_LINUX)
#define OUTPUT_RESET                        "\033[0m"
#define OUTPUT_RED                          "\033[31m"
#define OUTPUT_GREEN                        "\033[32m"
#define OUTPUT_YELLOW                       "\033[33m"
#else
#define OUTPUT_RESET                        ""
#define OUTPUT_RED                          ""
#define OUTPUT_GREEN                        ""
#define OUTPUT_YELLOW                       ""
#endif

#include <stdint.h>
#include "stdio.h"
#include <stdlib.h>

#define   NEW_APP               'A'
#define   GET_APP_INFO          'I'
#define   GET_LOADER_INFO       'G'
#define   DOWNLOADER_EN         'D'
#define   EXIT_BOOT             'E'
#define   GET_LAST_BOOT_STATUS  '\r'
#define   DOWNLOAD_IMAGE        'N'

#define HOST_SUCESS 0;

void welcome();
void show_menu();
static char const * GetLineTrailerByPercentage(uint8_t percentage);
uint32_t command_handler(uint8_t command);

#endif /* COMMANDER_COMMON_H_ */
