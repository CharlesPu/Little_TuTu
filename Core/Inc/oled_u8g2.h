#ifndef __OLED_U8G2_H_
#define __OLED_U8G2_H_	

#include "stm32_u8g2.h"
#include "main.h"
#include "u8g2.h"
#include "mpu6050.h"
 
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
void OLED_U8G2_draw_hex(uint8_t * buf, int buf_len);
void OLED_U8G2_draw_kdr(uint8_t * buf, uint8_t);
void OLED_U8G2_draw_mpu6050(imu_data_t *);


#endif
