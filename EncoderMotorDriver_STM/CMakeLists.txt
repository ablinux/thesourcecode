cmake_minimum_required(VERSION 3.15)
project(UartTest C ASM)

SET(CMAKE_VERBOSE_MAKEFILE OFF)

# Set the path of Driver and CMSIS here for quick access
set (CMSIS_DIR_PATH ${CMAKE_SOURCE_DIR}/Drivers/CMSIS)
set (STM32F1xx_DRIVER_PATH ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver)

# include CMSIS dir's
include_directories(${CMSIS_DIR_PATH}/Device/ST/STM32F1xx/Include/
                    ${CMSIS_DIR_PATH}/Include/ )

# Include STMDriver dir's                    
include_directories(${STM32F1xx_DRIVER_PATH}/Inc/)

# include Users header file
include_directories(${CMAKE_SOURCE_DIR}/Inc/)

# Set compiler option 
add_compile_options(-DSTM32F103xB)
add_compile_options(-DUSE_HAL_DRIVER)
add_compile_options(-mthumb)
add_compile_options(-mcpu=cortex-m3)
add_compile_options(-Og)
add_compile_options(-Wall -fdata-sections -ffunction-sections -g -lc -lm -lnosys) 

set (CMAKE_EXE_LINKER_FLAGS "-T${CMAKE_SOURCE_DIR}/STM32F103C8Tx_FLASH.ld -mthumb -mcpu=cortex-m3")

#Add Stm32 diver source file here file here 
file(GLOB SRC_FILES ${STM32F1xx_DRIVER_PATH}/Src/*.c)

# Add user code source file 
file(GLOB USER_SRC ${CMAKE_SOURCE_DIR}/Src/*.c)

# Add asm file
set(STM32_STARTUP ${CMAKE_SOURCE_DIR}/startup_stm32f103xb.s)

# final
add_executable(${PROJECT_NAME}.elf ${SRC_FILES} ${USER_SRC} ${STM32_STARTUP})