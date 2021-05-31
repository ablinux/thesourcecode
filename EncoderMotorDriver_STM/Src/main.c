/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

#define RIGHT 0x02
#define LEFT 0x4
#define FWD_BACK 0x08
#define RIGHT_MOTOR_FINISH 0x16
#define LEFT_MOTOR_FINISH 0x32

/*
1 complete revolution = 695 ticks
wheel dia = 44mm
wheel circumference = 2*pi*22mm = 138.23mm or 13.823 cm

so 1tick = 138.23mm/695 ticks = 0.198892 mm/ticks

so 0.198892mm = 1 tick hence 138.23mm = 138.23mm/0.19892mm/tics = x ticks
*/

#define TURN_RADIUS                                   (84U) //mm
#define WHEEL_DIA                                     (44U) //mm
#define ONE_REVOLUTION_TOTAL_TICK                     (695U) //Tick
#define WHEEL_CIRCUMFERENCE                           (2*3.14*WHEEL_DIA)
#define MM_PAR_TICK                                   (WHEEL_CIRCUMFERENCE/ONE_REVOLUTION_TOTAL_TICK)
#define MM_TO_TICKS(x)                                (x/MM_PAR_TICK)
/*for Rotation :
turning radus = 87mm 
total circumference = 2*3.14*87mm = 546.36mm
*/
#define TURNING_circumference                         (2*3.14*TURN_RADIUS)
#define ANGLE_TO_MM_TRAVEL(x)                         (TURNING_circumference*(x/360)) 

static unsigned int ticks_left = 0;
static unsigned int ticks_right = 0;
static unsigned int notify = 1;
uint64_t total_ticks_to_run = 0;
float mm_distance = 0;
uint16_t right_Pin = fwd_right_Pin;
uint16_t left_Pin = fwd_left_Pin;
uint16_t angle = 0;
uint8_t rot_dir = RIGHT;
char *ack_str = "DONE#";
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
/*   
1 complete revolution = 695 ticks
wheel dia = 44mm
wheel circumference = 2*pi*22mm = 138.23mm or 13.823 cm

so 1tick = 138.23mm/695 ticks = 0.198892 mm/ticks
*/
  uint8_t rBuff [17] = {0};
  uint8_t temp[5];
  uint8_t err = 0;
  


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    /* Read Serial instructs */
    /* Clear all buffers */
    err = 0;
    memset(rBuff,0x00,sizeof(rBuff));
    memset(temp,0x00,sizeof(temp));


    /* Wait forever for next command from CN1*/
    HAL_UART_Receive(&huart1,rBuff,16,0xff);

    /* Parse the packet */
    if(rBuff[0] == 'S')
    {
      /* Total ticks */
      memcpy(temp,&rBuff[2],4);
      /* Reset everything*/
      ticks_left = 0;
      ticks_right = 0;
      notify = 0;

      if (rBuff[1] == 'D')
      {
        /* Convert MM to Ticks */
        mm_distance = atoi((char*)temp);
        total_ticks_to_run = (uint64_t)(mm_distance/0.199F);
      }
      else if (rBuff[1] == 'T')
      {
        total_ticks_to_run = atoi((char*)temp);
      }
      /* Rotate the car */
      else if (rBuff[1]=='A')
      {
        /* in this condition we will skip the tick/mm length */
        /* 1st copy the angle of rotation */
        memset(temp,0x00,sizeof(temp));
        memcpy(temp,&rBuff[12],3);
        angle = atoi((char*)temp);
        /* Calculate the total rotation of wheel to achieve it */
        mm_distance = (TURNING_circumference * (angle/360.0F));
        total_ticks_to_run = (uint64_t)(mm_distance/0.199F);
        /* Now set the pins based on dir of rotation */
        if(rBuff[11] == 'R')
        {
          /* Right Rotation  */
          left_Pin = fwd_left_Pin;
          right_Pin = 0;
          HAL_GPIO_WritePin(GPIOB,fwd_left_Pin,1);
          HAL_GPIO_WritePin(GPIOB,fwd_right_Pin,0);
          rot_dir = RIGHT;
          HAL_NVIC_EnableIRQ(EXTI1_IRQn);
          HAL_NVIC_EnableIRQ(EXTI2_IRQn);

        }
        else if ( rBuff[11]== 'L')
        {
          /* Left Rotation */
          left_Pin = 0;
          right_Pin = fwd_right_Pin;
          HAL_GPIO_WritePin(GPIOB,fwd_left_Pin,0);
          HAL_GPIO_WritePin(GPIOB,fwd_right_Pin,1);
          HAL_NVIC_EnableIRQ(EXTI1_IRQn);
          HAL_NVIC_EnableIRQ(EXTI2_IRQn);
          rot_dir = LEFT;
        }
         /* Rotation from the center of robot */
        else if(rBuff[11] == 'r')
        {
          /* in this case each wheel has to turn only half of the total ticks */
          total_ticks_to_run = total_ticks_to_run/2;
          /* Right Rotation  */
          left_Pin = fwd_left_Pin;
          right_Pin = back_right_Pin;
          HAL_GPIO_WritePin(GPIOB,fwd_left_Pin,1);
          HAL_GPIO_WritePin(GPIOB,fwd_right_Pin,0);
          rot_dir = FWD_BACK;
          HAL_NVIC_EnableIRQ(EXTI1_IRQn);
          HAL_NVIC_EnableIRQ(EXTI2_IRQn);

        }
        else if ( rBuff[11]== 'l')
        {
          /* in this case each wheel has to turn only half of the total ticks */
          total_ticks_to_run = total_ticks_to_run/2;
          /* Left Rotation */
          left_Pin = back_left_Pin;
          right_Pin = fwd_right_Pin;
          HAL_GPIO_WritePin(GPIOB,fwd_left_Pin,0);
          HAL_GPIO_WritePin(GPIOB,fwd_right_Pin,1);
          HAL_NVIC_EnableIRQ(EXTI1_IRQn);
          HAL_NVIC_EnableIRQ(EXTI2_IRQn);
          rot_dir = FWD_BACK;
        }
        continue;
      }
      else
      {
        /* error abort */
        err = 1U;
      }
      if(err == 0)
      {
        /* To achieve fwd and back function */
        if(rBuff[7] == '0' && rBuff[8] == '0')
        {
          left_Pin = back_left_Pin;
          right_Pin = back_right_Pin;
        }
        else if(rBuff[7] == '1' && rBuff[8] == '1')
        {
          left_Pin = fwd_left_Pin;
          right_Pin = fwd_right_Pin;
        }
        /* right rotation and left rotation to angle*/
        HAL_GPIO_WritePin(GPIOB,fwd_left_Pin,rBuff[7]-'0');
        HAL_GPIO_WritePin(GPIOB,fwd_right_Pin,rBuff[8]-'0');
        HAL_GPIO_WritePin(GPIOB,back_left_Pin,rBuff[9]-'0');
        HAL_GPIO_WritePin(GPIOB,back_right_Pin,rBuff[10]-'0');
        HAL_NVIC_EnableIRQ(EXTI1_IRQn);
        HAL_NVIC_EnableIRQ(EXTI2_IRQn);
        rot_dir = FWD_BACK;
      }

    }
    if(err > 0)
    {
      HAL_UART_Abort(&huart1);
    }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, fwd_right_Pin|back_right_Pin|fwd_left_Pin|back_left_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : right_encoder_Pin left_encoder_Pin */
  GPIO_InitStruct.Pin = right_encoder_Pin|left_encoder_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : fwd_right_Pin back_right_Pin fwd_left_Pin back_left_Pin */
  GPIO_InitStruct.Pin = fwd_right_Pin|back_right_Pin|fwd_left_Pin|back_left_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == right_encoder_Pin)
  {
    ticks_right ++;
    // notify = 1;
  }
  if(GPIO_Pin == left_encoder_Pin)
  {
    ticks_left++;
    // notify = 1;
  }

  /* For FWD only */
  if(rot_dir == FWD_BACK)
  {
    if(notify != (LEFT_MOTOR_FINISH|RIGHT_MOTOR_FINISH))
    {
      /* both motor has to run in syn */
      /* if left motor has turned more stop the left motor and start the right motor */
      if(ticks_left > ticks_right && ticks_left < total_ticks_to_run)
      {
        HAL_GPIO_WritePin(GPIOB,left_Pin,0);
        HAL_GPIO_WritePin(GPIOB,right_Pin,1);
      }
      /* if right motor has turned more stop right and start left */
      else if(ticks_left < ticks_right && ticks_right < total_ticks_to_run)
      {
        HAL_GPIO_WritePin(GPIOB,left_Pin,1);
        HAL_GPIO_WritePin(GPIOB,right_Pin,0);
      }
      /*Both motors running equally keep them running*/
      else
      {
        HAL_GPIO_WritePin(GPIOB,left_Pin,1);
        HAL_GPIO_WritePin(GPIOB,right_Pin,1);
      }
    }

    /* Stop the motor left motor if it has reached to desired ticks
       also good advoice to disable the interrupt for this motor*/
    if( (ticks_left >= total_ticks_to_run) )
    {
      HAL_GPIO_WritePin(GPIOB,left_Pin,0);
      notify |= LEFT_MOTOR_FINISH;
    }
    /* Stop the motor right motor if it has reached to desired ticks
       also good advoice to disable the interrupt for this motor*/
    if( (ticks_right >= total_ticks_to_run) )
    {
      HAL_GPIO_WritePin(GPIOB,right_Pin,0);
     notify |= RIGHT_MOTOR_FINISH;
    }
  }
  /* Right roatation */
  else if (rot_dir == RIGHT)
  {
    /* Stop the motor */
    if( (ticks_left >= total_ticks_to_run) )
    {
      HAL_GPIO_WritePin(GPIOB,left_Pin,0);
      notify |= LEFT;
    }

  }
  /* Left rotation */
  else if (rot_dir == LEFT)
  {
    /* Stop the motor */
    if( (ticks_right >= total_ticks_to_run) )
    {
      HAL_GPIO_WritePin(GPIOB,right_Pin,0);
      notify |= RIGHT;
     
    }
  }
  if( (notify == (LEFT_MOTOR_FINISH|RIGHT_MOTOR_FINISH)) || /*1st condition for FW and bw movement*/
      (notify == LEFT) || (notify == RIGHT) )
  {
    HAL_UART_Transmit(&huart1,(uint8_t*)ack_str,sizeof(ack_str),2);
    HAL_NVIC_DisableIRQ(EXTI2_IRQn); /* Left Motor */
    HAL_NVIC_DisableIRQ(EXTI1_IRQn); /* Right Motor */
  }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
