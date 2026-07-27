# STM32F103 OLED 月薪喵动画

基于 STM32F103C8T6 和 0.96 英寸 I2C OLED 的 14 帧单色动画播放器。工程由 STM32CubeMX 生成，使用 STM32 HAL 库和 Keil MDK-ARM 构建。

![动画预览](docs/cat_oled_preview.gif)

## 硬件

| 器件 | 规格 |
| --- | --- |
| 主控 | STM32F103C8T6，Cortex-M3，官方 64KB Flash、20KB SRAM |
| 显示屏 | 0.96 英寸 OLED，128×64，SSD1306，I2C 接口 |
| 系统时钟 | 外部 8MHz 晶振，PLL 倍频至 72MHz |
| I2C | I2C1，400kHz，7 位地址 `0x3C` |

## 接线

OLED 接口从左到右为 `GND`、`VCC`、`SCL`、`SDA`：

| OLED | STM32F103C8T6 | 说明 |
| --- | --- | --- |
| GND | GND | 必须与开发板共地 |
| VCC | 3.3V | 不建议接 5V |
| SCL | PB6 | I2C1_SCL |
| SDA | PB7 | I2C1_SDA |

## 软件环境

- STM32CubeMX 6.10.0
- STM32Cube FW_F1 V1.8.7
- Keil MDK-ARM 5
- ARM Compiler 5.06

## 编译和下载

1. 使用 Keil 打开 `MDK-ARM/Stm32_Cat_hal.uvprojx`。
2. 点击 Build，工程应以 0 Error、0 Warning 完成编译。
3. 使用 ST-Link 连接开发板，通过 Keil Download 下载固件。
4. 复位后 OLED 将循环播放 14 帧动画。

当前构建占用约为：

```text
Code=4816  RO-data=6176  RW-data=20  ZI-data=1108
```

## 目录结构

```text
Stm32_Cat_hal/
├─ Core/
│  ├─ Inc/
│  │  ├─ cat_anim.h          动画尺寸、帧数和数据声明
│  │  └─ oled_i2c.h          OLED 驱动接口
│  └─ Src/
│     ├─ cat_anim_data.c     14 帧单色动画数据
│     ├─ main.c              初始化和动画播放主循环
│     └─ oled_i2c.c          SSD1306 I2C 驱动
├─ Drivers/                  STM32 HAL 和 CMSIS
├─ MDK-ARM/                  Keil 工程
└─ Stm32_Cat_hal.ioc         STM32CubeMX 配置
```

## 显示参数

- 动画尺寸：51×64
- 动画帧数：14 帧
- 帧等待时间：120ms
- 显示位置：水平居中
- 数据格式：SSD1306 页优先 1-bit 位图
- 发送方式：HAL I2C 轮询发送

修改播放速度：

```c
#define CAT_FRAME_DELAY_MS 120U
```

修改 OLED 地址：

```c
#define OLED_I2C_ADDRESS (0x3CU << 1)
```

HAL I2C 接口要求传入左移一位后的地址，因此 `0x3C` 在发送时表示为 `0x78`。

## 常见问题

### OLED 不亮

检查 3.3V、GND、PB6 和 PB7 接线，并确认 OLED 的 7 位地址是 `0x3C`。部分模块使用 `0x3D`。

### 图像水平偏移

本工程默认使用 SSD1306，`OLED_COLUMN_OFFSET` 为 0。部分 SH1106 模块需要将该值设为 2，但 SH1106 并非本工程的默认目标控制器。

### 重新生成 CubeMX 代码

自定义代码位于 CubeMX 的 `USER CODE BEGIN/END` 区域或独立的 `oled_i2c`、`cat_anim` 文件中。重新生成后仍建议检查 Keil 工程是否保留了 `oled_i2c.c` 和 `cat_anim_data.c`。
