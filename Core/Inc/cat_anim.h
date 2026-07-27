/**
  ******************************************************************************
  * @file    cat_anim.h
  * @brief   月薪喵单色动画尺寸、播放参数和数据声明
  * @note    每帧均按照 SSD1306 页优先字节顺序存储
  ******************************************************************************
  */
#ifndef CAT_ANIM_H
#define CAT_ANIM_H

#include <stdint.h>

/** @brief 动画总帧数。 */
#define CAT_FRAME_COUNT       14U

/** @brief 单帧宽度，单位为像素。 */
#define CAT_FRAME_WIDTH       51U

/** @brief 单帧高度，单位为像素。 */
#define CAT_FRAME_HEIGHT      64U

/** @brief 单帧占用的 SSD1306 页数，每页高度为 8 个像素。 */
#define CAT_FRAME_PAGE_COUNT  (CAT_FRAME_HEIGHT / 8U)

/** @brief 单帧数据字节数。 */
#define CAT_FRAME_DATA_SIZE   (CAT_FRAME_WIDTH * CAT_FRAME_PAGE_COUNT)

/** @brief 动画水平居中显示时的起始列坐标。 */
#define CAT_FRAME_X           ((128U - CAT_FRAME_WIDTH) / 2U)

/** @brief 每帧显示完成后的等待时间，单位为毫秒。 */
#define CAT_FRAME_DELAY_MS    120U

/** @brief 14 帧 SSD1306 页格式动画数据。 */
extern const uint8_t cat_anim_frames[CAT_FRAME_COUNT][CAT_FRAME_DATA_SIZE];

#endif /* CAT_ANIM_H */
