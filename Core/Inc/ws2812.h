#ifndef __WS2812_H
#define __WS2812_H

#include "main.h"
 
#define WS_H           70   // 1 码相对计数值
#define WS_L           34   // 0 码相对计数值
// #define WS_REST        40   // 复位信号脉冲数量
#define LED_NUM        16   // WS2812灯个数
#define DATA_LEN       24   // WS2812数据长度，单个需要24个字节
// note 这里改成60避免后面灯闪烁
#define WS2812_RST_NUM 60   // 官方复位时间为50us（40个周期），保险起见使用50个周期

#define BLINK_INTERVAL 100
 
extern uint8_t g_rgb_led_enabled;

void WS2812_Init(void);
void WS2812_Set(uint16_t num,uint8_t R,uint8_t G,uint8_t B);
void WS2812_test(void);


void WS2812_breathe(void);
void WS2812_breathe_step(void);
void WS2812_blink(void);


#endif
