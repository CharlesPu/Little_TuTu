#include "ws2812.h"
#include "tim.h"

uint8_t g_rgb_led_enabled = 1;
// 显存数组，长度为 灯的数量*24+复位周期
uint16_t WS2812_RGB_Buff[LED_NUM * DATA_LEN + WS2812_RST_NUM] = {0};

/**
 * 函数：WS2812单灯设置函数
 * 参数：num:灯的位置，R、G、B分别为三个颜色通道的亮度，最大值为255
 * 作用：单独设置每一个WS2812的颜色
 ***/
void WS2812_Set(uint16_t num, uint8_t R, uint8_t G, uint8_t B)
{
  uint32_t indexx = (num * DATA_LEN);
  for (uint8_t i = 0; i < 8; i++)
  {
    // 填充数组
    WS2812_RGB_Buff[indexx + i] = (G << i) & (0x80) ? WS_H : WS_L;
    WS2812_RGB_Buff[indexx + i + 8] = (R << i) & (0x80) ? WS_H : WS_L;
    WS2812_RGB_Buff[indexx + i + 16] = (B << i) & (0x80) ? WS_H : WS_L;
  }
}

// WS2812初始化函数
void WS2812_Init()
{
  // 设置关闭所有灯
  for (int i = 0; i < LED_NUM; i++)
  {
    WS2812_Set(i, 0, 0, 0);
  }
  // 作用：调用DMA将显存中的内容实时搬运至定时器的比较寄存器
  HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_3, (uint32_t *)WS2812_RGB_Buff, sizeof(WS2812_RGB_Buff) / sizeof(uint16_t));
}

void WS2812_test(void)
{
  for (int i = 0; i < LED_NUM; i++)
  {
    HAL_Delay(200);
    // WS2812_Set(i, 2 * (i + 1), 4 * (i + 1), 10 * (i + 1));
    WS2812_Set(i, 255, 0, 255);
  }
  HAL_Delay(1000);
  for (int i = 0; i < LED_NUM; i++)
  {
    WS2812_Set(i, 0, 0, 0);
  }
  HAL_Delay(100);
}

const uint16_t R[] = {
	255, 226, 198, 170, 141, 113,  85,  56,  28,   0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0,  28,  56,  85, 113, 141, 170, 198, 226, 255
};
const uint16_t G[] = {
	0,  28,  56,  85, 113, 141, 170, 198, 226, 255,
	255, 226, 198, 170, 141, 113,  85,  56,  28,   0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,0
};
const uint16_t B[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0,  28,  56,  85, 113, 141, 170, 198, 226, 255,
	255, 226, 198, 170, 141, 113,  85,  56,  28, 0
};
void WS2812_breathe(void)
{
  for (int j = 0; j < 30; j++)
  {
    for (int i = 0; i < LED_NUM; i++)
    {
      HAL_Delay(10);
      WS2812_Set(i, R[j], G[j], B[j]);
    }
  }
}

uint16_t s_breathe_step_n = 0;
void WS2812_breathe_step(void)
{
  if (!g_rgb_led_enabled) 
  {
    for (int i = 0; i < LED_NUM; i++)
      WS2812_Set(i, 0, 0, 0);
    return;
  }
  
  if (s_breathe_step_n >= 30) s_breathe_step_n = 0;
  for (int i = 0; i < LED_NUM; i++)
  {
    WS2812_Set(i, R[s_breathe_step_n], G[s_breathe_step_n], B[s_breathe_step_n]);
  }
  s_breathe_step_n++;
}

void WS2812_blink(void)
{
  for (int i = 0; i < LED_NUM; i++)
    WS2812_Set(i, 255, 0, 255);
  HAL_Delay(BLINK_INTERVAL);
  for (int i = 0; i < LED_NUM; i++)
    WS2812_Set(i, 0, 0, 0);
  HAL_Delay(BLINK_INTERVAL);
  for (int i = 0; i < LED_NUM; i++)
    WS2812_Set(i, 255, 0, 255);
  HAL_Delay(BLINK_INTERVAL);
  for (int i = 0; i < LED_NUM; i++)
    WS2812_Set(i, 0, 0, 0);
  HAL_Delay(BLINK_INTERVAL);
}


