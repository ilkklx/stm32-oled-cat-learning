/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : 月薪喵 OLED 动画主程序
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* 包含文件 ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "gpio.h"

/* 私有包含文件 --------------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "cat_anim.h"
#include "oled_i2c.h"

/* USER CODE END Includes */

/* 私有类型定义 --------------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* 私有常量定义 --------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* 私有宏定义 ----------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* 私有变量 ------------------------------------------------------------------*/

/* USER CODE BEGIN PV */
static uint8_t s_frame_index = 0U;

/* USER CODE END PV */

/* 私有函数声明 --------------------------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
static HAL_StatusTypeDef APP_PlayNextFrame(void);

/* USER CODE END PFP */

/* 私有用户代码 --------------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
  * @brief  在 OLED 上显示下一帧动画，并更新帧索引
  * @retval HAL_OK      当前帧显示成功
  * @retval HAL_ERROR   参数无效或 I2C 发送失败
  * @retval HAL_BUSY    I2C 外设正忙
  * @retval HAL_TIMEOUT I2C 通信超时
  */
static HAL_StatusTypeDef APP_PlayNextFrame(void)
{
  HAL_StatusTypeDef status;

  status = OLED_ShowRegion(CAT_FRAME_X,
                           0U,
                           CAT_FRAME_WIDTH,
                           CAT_FRAME_PAGE_COUNT,
                           cat_anim_frames[s_frame_index]);
  if (status != HAL_OK)
  {
    return status;
  }

  s_frame_index++;
  if (s_frame_index >= CAT_FRAME_COUNT)
  {
    s_frame_index = 0U;
  }

  return HAL_OK;
}

/* USER CODE END 0 */

/**
  * @brief  程序入口
  * @retval int 程序不会正常退出
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU 配置 ----------------------------------------------------------------*/

  /* 复位全部外设，并初始化 Flash 接口和系统滴答定时器。 */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* 配置系统时钟 */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* 初始化全部已配置外设 */
  MX_GPIO_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  if (OLED_Init() != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE END 2 */

  /* 无限循环 */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    if (APP_PlayNextFrame() != HAL_OK)
    {
      Error_Handler();
    }

    HAL_Delay(CAT_FRAME_DELAY_MS);
  }
  /* USER CODE END 3 */
}

/**
  * @brief  配置系统时钟：外部 8MHz 晶振经 PLL 倍频至 72MHz
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** 按照 RCC_OscInitTypeDef 结构体中的参数初始化振荡器。 */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** 初始化 CPU、AHB 和 APB 总线时钟。 */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  错误处理函数：关闭中断并停留在死循环中
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* 发生不可恢复错误时停止程序，便于连接调试器定位。 */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  报告发生参数断言错误的源文件和行号
  * @param  file 源文件名称指针
  * @param  line 发生错误的源代码行号
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* 可在此处输出发生参数错误的文件名称和源代码行号。 */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
