# STM32F103 OLED 月薪喵动画

这个小项目，是我跟着 B 站江协科技（江科大）的 STM32 入门课程学习后，尝试把课程知识用到实际作品中的一次实践。起点就是课程配套资料和手边的学习套装：从 GPIO、OLED 显示到工程编译与程序下载，把原本分散在各个实验中的知识串起来，让屏幕上的字符和图形变成一只会跳舞的小猫。

实现过程中，我结合课程资料、网上搜集的 OLED 动画与开源工程资料，并借助 AI 辅助理解代码、分析报错和梳理操作步骤，逐步完成了现有开源工程在自己开发环境中的适配与硬件验证。这次实践使用 STM32F103C8T6，通过 I2C 驱动 OLED，循环播放 14 帧单色动画；同时解决了 AC5 工程在本机 AC6 环境下的编译设置、ST-Link 下载器选择等问题，最终在开发板上呈现出与演示一致的小猫跳舞效果。

对已经有同类 STM32 学习套装的同学来说，这个项目的门槛比较低：只需要套装中的核心板、四针 I2C OLED、ST-Link 下载器和杜邦线，再配合供电线即可，不需要为了播放这段动画额外购买传感器、舵机、外部存储模块或其他复杂器材。确认主控和屏幕规格匹配后，就可以利用现有材料动手尝试。它很适合作为入门课程之后的第一个趣味项目，在可见的成果中练习接线、读代码、编译和排错，也希望能给同样刚入门的同学提供一份可学习、可借鉴的实践参考。

本项目基于 [jechanoo/stm32f103-oled-cat-animation](https://github.com/jechanoo/stm32f103-oled-cat-animation) 复现与整理，保留原工程的驱动和动画实现；我的实践主要是环境适配、硬件验证与学习记录。原工程代码及动画素材并非本人原创，相关权利归各自权利人所有。感谢课程作者、原项目作者及相关资料分享者。

学习参考：[江协科技《STM32入门教程》](https://www.bilibili.com/video/BV1th411z7sn/)。课程提供了入门知识与实验基础，本文是个人学习实践记录，不代表课程作者参与或认可本项目。

基于 STM32F103C8T6 和 0.96 英寸 I2C OLED 的 14 帧单色动画播放器。工程由 STM32CubeMX 生成，使用 STM32 HAL 库和 Keil MDK-ARM 构建。

![动画预览](docs/cat_oled_preview.gif)

## 我的实物效果

使用现有学习套装完成接线、编译与烧录后，OLED 已成功循环播放小猫动画。以下是本次实践的实拍照片和视频。

![OLED 小猫动画实拍一](docs/oled-cat-photo-01.png)

![OLED 小猫动画实拍二](docs/oled-cat-photo-02.png)

[查看实拍演示视频](video/5f114181739bccc0294df3c6eaca5015.mp4)

照片在 README 中直接显示；MP4 通过链接打开，具体在线播放能力取决于浏览平台，也可以下载后观看。上方“动画预览”来自原工程，以上实拍材料由本次学习实践提供。

## 来源与使用说明

- 原工程作者：[jechanoo](https://github.com/jechanoo)，原项目：[STM32F103 OLED 月薪喵动画](https://github.com/jechanoo/stm32f103-oled-cat-animation)。
- 我的工作：AC6 编译环境适配、ST-Link 下载设置、实物接线与运行验证，以及实践说明整理；未重新编写原 OLED 驱动或原创动画素材。
- AI 辅助范围：理解代码、分析编译和下载报错、整理文档；硬件连接与最终运行效果由本人实际验证。
- 原仓库未找到覆盖整体项目的明确许可证，动画素材的原始作者及授权也尚未核实。本记录不代表获得了对原代码或素材的再授权；公开分发源码、固件或商业使用前，应向相关权利人确认许可。
- HAL、CMSIS 等第三方组件保留各自的许可证与版权声明，不将其授权范围扩展为整个项目的授权。

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
- 原作者环境：ARM Compiler 5.06
- 本次实测环境：Keil MDK 5.43a、ARM Compiler 6.24、ST-Link V2

工程自带 HAL、CMSIS 和 OLED 驱动源码，使用现成工程无需重新安装 CubeMX 或下载整个固件库包。本次使用的芯片支持包为 `Keil.STM32F1xx_DFP.2.2.0`。

## 编译和下载

1. 使用 Keil 打开 `MDK-ARM/Stm32_Cat_hal.uvprojx`。
2. 在 Options for Target → Target 选择已安装的 ARM Compiler 6.24，执行 Rebuild。本次实测为 0 Error、0 Warning；编译未成功时不要继续下载。
3. 在 Debug 页选择 ST-Link Debugger，Settings 中接口选择 SW；Utilities 页勾选 Use Target Driver for Flash Programming。连接开发板后通过 Keil Download 下载固件。克隆版 ST-Link 出现固件升级提示时选择 No，可取消 Check version on start 避免重复提示。
4. 复位后 OLED 将循环播放 14 帧动画。

本次 AC6.24 构建占用为（与原作者 AC5 构建数值不同）：

```text
Code=7532  RO-data=6024  RW-data=12  ZI-data=1124
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
