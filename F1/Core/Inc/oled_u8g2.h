#ifndef __OLED_U8G2_H_
#define __OLED_U8G2_H_	

#include "stm32_u8g2.h"
#include "main.h"
#include "u8g2.h"
 
void testDrawProcess(u8g2_t *u8g2);
void testShowFont(u8g2_t *u8g2);
void testDrawFrame(u8g2_t *u8g2);
void testDrawRBox(u8g2_t *u8g2);
void testDrawCircle(u8g2_t *u8g2);
void testDrawFilledEllipse(u8g2_t *u8g2);
void testDrawMulti(u8g2_t *u8g2);
void testDrawXBM(u8g2_t *u8g2);
 
void u8g2DrawTest(u8g2_t *u8g2);


void OLED_U8G2_init(void);
void OLED_U8G2_draw_test(void);
void OLED_U8G2_draw_buf(uint8_t * buf, int buf_len);
void OLED_U8G2_draw_mpu6050(float *pitch,float *roll,float *yaw,short temp);


#endif
