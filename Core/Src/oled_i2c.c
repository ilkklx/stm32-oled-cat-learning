/**
  ******************************************************************************
  * @file    oled_i2c.c
  * @brief   基于 STM32 HAL I2C 的 SSD1306 128x64 OLED 驱动
  * @note    位图数据按照 SSD1306 页寻址顺序发送
  ******************************************************************************
  */
#include "oled_i2c.h"

#include "i2c.h"

#define OLED_CONTROL_COMMAND  0x00U
#define OLED_CONTROL_DATA     0x40U

/**
  * @brief  向 SSD1306 控制器发送一个或多个命令字节
  * @param  commands      命令字节数组指针
  * @param  command_count 命令字节数量，范围为 1~31
  * @retval HAL_OK      命令发送成功
  * @retval HAL_ERROR   参数无效或 I2C 发送失败
  * @retval HAL_BUSY    I2C 外设正忙
  * @retval HAL_TIMEOUT I2C 通信超时
  */
static HAL_StatusTypeDef OLED_WriteCommands(const uint8_t *commands,
                                             uint8_t command_count)
{
  uint8_t tx_buffer[32];

  if ((commands == NULL) || (command_count == 0U) ||
      (command_count > (sizeof(tx_buffer) - 1U)))
  {
    return HAL_ERROR;
  }

  tx_buffer[0] = OLED_CONTROL_COMMAND;
  for (uint8_t i = 0U; i < command_count; ++i)
  {
    tx_buffer[i + 1U] = commands[i];
  }

  return HAL_I2C_Master_Transmit(&hi2c1,
                                 OLED_I2C_ADDRESS,
                                 tx_buffer,
                                 (uint16_t)command_count + 1U,
                                 OLED_I2C_TIMEOUT_MS);
}

/**
  * @brief  设置页地址，并向该页连续写入一行数据
  * @param  x     OLED 起始列坐标，范围为 0~127
  * @param  page  目标页，范围为 0~7，每页高度为 8 个像素
  * @param  data  待发送的页数据指针
  * @param  width 要写入的列数
  * @retval HAL_OK      页数据发送成功
  * @retval HAL_ERROR   参数无效或 I2C 发送失败
  * @retval HAL_BUSY    I2C 外设正忙
  * @retval HAL_TIMEOUT I2C 通信超时
  */
static HAL_StatusTypeDef OLED_WritePage(uint8_t x,
                                        uint8_t page,
                                        const uint8_t *data,
                                        uint8_t width)
{
  uint8_t commands[3];
  uint8_t tx_buffer[OLED_WIDTH + 1U];
  uint8_t column;
  HAL_StatusTypeDef status;

  if ((data == NULL) || (width == 0U) || (page >= OLED_PAGE_COUNT) ||
      (((uint16_t)x + width) > OLED_WIDTH))
  {
    return HAL_ERROR;
  }

  column = (uint8_t)(x + OLED_COLUMN_OFFSET);
  commands[0] = (uint8_t)(0xB0U | page);
  commands[1] = (uint8_t)(column & 0x0FU);
  commands[2] = (uint8_t)(0x10U | (column >> 4U));

  status = OLED_WriteCommands(commands, sizeof(commands));
  if (status != HAL_OK)
  {
    return status;
  }

  tx_buffer[0] = OLED_CONTROL_DATA;
  for (uint8_t i = 0U; i < width; ++i)
  {
    tx_buffer[i + 1U] = data[i];
  }

  return HAL_I2C_Master_Transmit(&hi2c1,
                                 OLED_I2C_ADDRESS,
                                 tx_buffer,
                                 (uint16_t)width + 1U,
                                 OLED_I2C_TIMEOUT_MS);
}

/**
  * @brief  检测并初始化 SSD1306 OLED，随后清空显存
  * @note   调用本函数前必须先执行 MX_I2C1_Init()
  * @retval HAL_OK      OLED 初始化成功
  * @retval HAL_ERROR   OLED 无响应或 I2C 操作失败
  * @retval HAL_BUSY    I2C 外设正忙
  * @retval HAL_TIMEOUT I2C 通信超时
  */
HAL_StatusTypeDef OLED_Init(void)
{
  static const uint8_t init_commands[] = {
    0xAEU,             /* 关闭显示 */
    0xD5U, 0x80U,      /* 设置显示时钟分频 */
    0xA8U, 0x3FU,      /* 设置复用率为 64 行 */
    0xD3U, 0x00U,      /* 设置显示偏移为 0 */
    0x40U,             /* 设置显示起始行为 0 */
    0x8DU, 0x14U,      /* 开启内部电荷泵 */
    0x20U, 0x02U,      /* 使用页寻址模式 */
    0xA1U,             /* 设置列地址映射方向 */
    0xC8U,             /* 设置 COM 扫描方向 */
    0xDAU, 0x12U,      /* 设置 COM 引脚配置 */
    0x81U, 0x7FU,      /* 设置对比度 */
    0xD9U, 0xF1U,      /* 设置预充电周期 */
    0xDBU, 0x40U,      /* 设置 VCOMH 电压 */
    0xA4U,             /* 显示内容跟随显存 */
    0xA6U,             /* 正常显示，不反色 */
    0x2EU,             /* 停止滚动 */
    0xAFU              /* 开启显示 */
  };
  HAL_StatusTypeDef status;

  HAL_Delay(100U);

  status = HAL_I2C_IsDeviceReady(&hi2c1,
                                 OLED_I2C_ADDRESS,
                                 3U,
                                 OLED_I2C_TIMEOUT_MS);
  if (status != HAL_OK)
  {
    return status;
  }

  status = OLED_WriteCommands(init_commands, sizeof(init_commands));
  if (status != HAL_OK)
  {
    return status;
  }

  return OLED_Clear();
}

/**
  * @brief  清空全部 8 页显存并熄灭所有 OLED 像素
  * @retval HAL_OK      显存清空成功
  * @retval HAL_ERROR   I2C 发送失败
  * @retval HAL_BUSY    I2C 外设正忙
  * @retval HAL_TIMEOUT I2C 通信超时
  */
HAL_StatusTypeDef OLED_Clear(void)
{
  static const uint8_t empty_page[OLED_WIDTH] = {0U};

  for (uint8_t page = 0U; page < OLED_PAGE_COUNT; ++page)
  {
    HAL_StatusTypeDef status = OLED_WritePage(0U,
                                               page,
                                               empty_page,
                                               OLED_WIDTH);
    if (status != HAL_OK)
    {
      return status;
    }
  }

  return HAL_OK;
}

/**
  * @brief  将页优先位图数据绘制到 OLED 的指定矩形区域
  * @param  x          起始列坐标，范围为 0~127
  * @param  start_page 起始页，范围为 0~7
  * @param  width      显示区域宽度，单位为像素
  * @param  page_count 要写入的页数，每页高度为 8 个像素
  * @param  data       SSD1306 页优先格式的源数据指针
  * @retval HAL_OK      完整区域写入成功
  * @retval HAL_ERROR   参数无效或 I2C 发送失败
  * @retval HAL_BUSY    I2C 外设正忙
  * @retval HAL_TIMEOUT I2C 通信超时
  */
HAL_StatusTypeDef OLED_ShowRegion(uint8_t x,
                                  uint8_t start_page,
                                  uint8_t width,
                                  uint8_t page_count,
                                  const uint8_t *data)
{
  if ((data == NULL) || (width == 0U) || (page_count == 0U) ||
      (((uint16_t)x + width) > OLED_WIDTH) ||
      (((uint16_t)start_page + page_count) > OLED_PAGE_COUNT))
  {
    return HAL_ERROR;
  }

  for (uint8_t page = 0U; page < page_count; ++page)
  {
    HAL_StatusTypeDef status = OLED_WritePage(x,
                                               (uint8_t)(start_page + page),
                                               &data[(uint16_t)page * width],
                                               width);
    if (status != HAL_OK)
    {
      return status;
    }
  }

  return HAL_OK;
}

/**
  * @brief  绘制矩形边框和 X 形图案，用于屏幕诊断
  * @note   本函数用于首次接线、屏幕方向和显示范围测试
  * @retval HAL_OK      测试图案写入成功
  * @retval HAL_ERROR   I2C 发送失败
  * @retval HAL_BUSY    I2C 外设正忙
  * @retval HAL_TIMEOUT I2C 通信超时
  */
HAL_StatusTypeDef OLED_ShowTestPattern(void)
{
  uint8_t page_data[OLED_WIDTH];

  for (uint8_t page = 0U; page < OLED_PAGE_COUNT; ++page)
  {
    for (uint8_t x = 0U; x < OLED_WIDTH; ++x)
    {
      uint8_t value = 0U;
      uint8_t diagonal_y = (uint8_t)(x >> 1U);
      uint8_t reverse_y = (uint8_t)((OLED_HEIGHT - 1U) - diagonal_y);

      if ((x == 0U) || (x == (OLED_WIDTH - 1U)))
      {
        value = 0xFFU;
      }
      if (page == 0U)
      {
        value |= 0x01U;
      }
      if (page == (OLED_PAGE_COUNT - 1U))
      {
        value |= 0x80U;
      }
      if ((diagonal_y >> 3U) == page)
      {
        value |= (uint8_t)(1U << (diagonal_y & 0x07U));
      }
      if ((reverse_y >> 3U) == page)
      {
        value |= (uint8_t)(1U << (reverse_y & 0x07U));
      }

      page_data[x] = value;
    }

    {
      HAL_StatusTypeDef status = OLED_WritePage(0U,
                                                 page,
                                                 page_data,
                                                 OLED_WIDTH);
      if (status != HAL_OK)
      {
        return status;
      }
    }
  }

  return HAL_OK;
}
