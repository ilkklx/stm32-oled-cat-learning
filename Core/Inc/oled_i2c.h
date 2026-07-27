/**
  ******************************************************************************
  * @file    oled_i2c.h
  * @brief   SSD1306 128x64 OLED 硬件 I2C 驱动接口
  * @note    使用 I2C1 通信，图像数据采用 SSD1306 页寻址格式
  ******************************************************************************
  */
#ifndef OLED_I2C_H
#define OLED_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

#define OLED_WIDTH              128U
#define OLED_HEIGHT              64U
#define OLED_PAGE_COUNT          (OLED_HEIGHT / 8U)

/* HAL 接口要求将 7 位 I2C 地址左移一位后传入。 */
#define OLED_I2C_ADDRESS         (0x3CU << 1)
#define OLED_I2C_TIMEOUT_MS      100U

/* SSD1306 使用 0；常见的 128x64 SH1106 模块通常使用 2。 */
#define OLED_COLUMN_OFFSET       0U

/**
  * @brief  检测并初始化 SSD1306 OLED，随后清空显存
  * @retval HAL_OK      OLED 响应正常且初始化成功
  * @retval HAL_ERROR   操作无效或 OLED 初始化失败
  * @retval HAL_BUSY    I2C 外设正忙
  * @retval HAL_TIMEOUT I2C 通信超时
  */
HAL_StatusTypeDef OLED_Init(void);

/**
  * @brief  清空 OLED 显存并熄灭全部像素
  * @retval HAL_OK      显存清空成功
  * @retval HAL_ERROR   I2C 发送失败
  * @retval HAL_BUSY    I2C 外设正忙
  * @retval HAL_TIMEOUT I2C 通信超时
  */
HAL_StatusTypeDef OLED_Clear(void);

/**
  * @brief  将页格式位图写入 OLED 的指定矩形区域
  * @param  x          起始列坐标，范围为 0~127
  * @param  start_page 起始页，范围为 0~7，每页高度为 8 个像素
  * @param  width      显示区域宽度，单位为像素
  * @param  page_count 要写入的页数
  * @param  data       页优先位图数据，每字节表示竖直方向的 8 个像素
  * @retval HAL_OK      区域写入成功
  * @retval HAL_ERROR   参数无效或 I2C 发送失败
  * @retval HAL_BUSY    I2C 外设正忙
  * @retval HAL_TIMEOUT I2C 通信超时
  */
HAL_StatusTypeDef OLED_ShowRegion(uint8_t x,
                                  uint8_t start_page,
                                  uint8_t width,
                                  uint8_t page_count,
                                  const uint8_t *data);

/**
  * @brief  显示矩形边框和对角线，用于检查屏幕接线与方向
  * @retval HAL_OK      测试图案写入成功
  * @retval HAL_ERROR   I2C 发送失败
  * @retval HAL_BUSY    I2C 外设正忙
  * @retval HAL_TIMEOUT I2C 通信超时
  */
HAL_StatusTypeDef OLED_ShowTestPattern(void);

#ifdef __cplusplus
}
#endif

#endif /* OLED_I2C_H */
