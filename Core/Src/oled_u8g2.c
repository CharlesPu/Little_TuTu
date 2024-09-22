#include "oled_u8g2.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"


static u8g2_t my_u8g2;
//---------------U8g2测试函数
 
#define SEND_BUFFER_DISPLAY_MS(u8g2, ms)\
  do {\
    u8g2_SendBuffer(u8g2); \
    HAL_Delay(ms);\
  }while(0);
 
void OLED_U8G2_init(void)
{
  u8g2Init(&my_u8g2);
  OLED_U8G2_init_animation();
}

void OLED_U8G2_init_animation(void)
{
  u8g2_ClearBuffer(&my_u8g2); 

	u8g2_SetFont(&my_u8g2,u8g2_font_lubBI24_tf);
  u8g2_DrawStr(&my_u8g2,0,32,"X TuTu");
  u8g2_DrawStr(&my_u8g2,16,64,"Start!");

  u8g2_SendBuffer(&my_u8g2);
}

void OLED_U8G2_draw_test(void) 
{
  draw(&my_u8g2);
  u8g2DrawTest(&my_u8g2);
}

void OLED_U8G2_draw_buf(uint8_t * buf, int buf_len)
{
  u8g2_ClearBuffer(&my_u8g2); 

  char *tmp = malloc(sizeof(uint8_t)*buf_len +1);
  memcpy(tmp, buf, buf_len);
  tmp[buf_len] = '\0';
	u8g2_SetFont(&my_u8g2,u8g2_font_ncenB08_tf);
  char tmp2[24]={0};
  sprintf(tmp2, "len:%d",buf_len);
  u8g2_DrawStr(&my_u8g2,8,8,tmp2);
  u8g2_DrawStr(&my_u8g2,64,8,"buf:");
  u8g2_DrawStr(&my_u8g2,32,24,tmp);

  u8g2_SendBuffer(&my_u8g2);
  free(tmp);
}

void OLED_U8G2_draw_hex(uint8_t * buf, int buf_len)
{
  u8g2_ClearBuffer(&my_u8g2); 

  char *tmp = malloc(sizeof(uint8_t)*buf_len*3+1);
  for (int i = 0; i < buf_len; i++)
  {
    sprintf(tmp+3*i, "%02X,",buf[i]);
  }
  tmp[buf_len*3+1] = '\0';
	u8g2_SetFont(&my_u8g2,u8g2_font_ncenB08_tf);
  char tmp2[24]={0};
  sprintf(tmp2, "len:%d",buf_len);
  u8g2_DrawStr(&my_u8g2,8,8,tmp2);
  u8g2_DrawStr(&my_u8g2,64,8,"buf:");
  u8g2_DrawStr(&my_u8g2,0,24,tmp);

  u8g2_SendBuffer(&my_u8g2);
  free(tmp);
}

void OLED_U8G2_draw_kdr(uint8_t * buf, uint8_t len)
{
  u8g2_ClearBuffer(&my_u8g2); 

  char *tmpx = malloc(sizeof(uint8_t)*len/2*3+1);
  for (int i = 0; i < len/2; i++)
  {
    sprintf(tmpx+3*i, "%02X,",buf[i]);
  }
  tmpx[len/2*3+1] = '\0';
  char *tmpy = malloc(sizeof(uint8_t)*len/2*3+1);
  for (int i = 0; i < len/2; i++)
  {
    sprintf(tmpy+3*i, "%02X,",buf[i+len/2]);
  }
  tmpy[len/2*3+1] = '\0';
  
	u8g2_SetFont(&my_u8g2,u8g2_font_ncenB10_tf);
  char tmp2[24]={0};
  sprintf(tmp2, "len: %d",len);
  u8g2_DrawStr(&my_u8g2,0,16,tmp2);
  u8g2_DrawStr(&my_u8g2,72,16,"kdr:");
  u8g2_DrawStr(&my_u8g2,0,38,tmpx);
  u8g2_DrawStr(&my_u8g2,0,58,tmpy);

  u8g2_SendBuffer(&my_u8g2);
  free(tmpx);
  free(tmpy);
}

void OLED_U8G2_draw_mpu6050(imu_data_t *data)
{
  u8g2_ClearBuffer(&my_u8g2); 

  char tmp[30]={0};
	u8g2_SetFont(&my_u8g2,u8g2_font_ncenB12_tf);
  sprintf(tmp, "pitch: %.04f", data->pitch);
  u8g2_DrawStr(&my_u8g2,8,12, tmp);
  sprintf(tmp, "roll: %.04f", data->roll);
  u8g2_DrawStr(&my_u8g2,8,28,tmp);
  sprintf(tmp, "yaw: %.04f", data->yaw);
  u8g2_DrawStr(&my_u8g2,8,44, tmp);
  sprintf(tmp, "temp: %.02f", ((float)(data->temp))/100);
  u8g2_DrawStr(&my_u8g2,8,60, tmp);

  u8g2_SendBuffer(&my_u8g2);
}

void OLED_U8G2_draw_hc_sr04(uint16_t mm)
{
  u8g2_ClearBuffer(&my_u8g2); 

  char tmp[30]={0};
	u8g2_SetFont(&my_u8g2,u8g2_font_ncenB12_tf);
  u8g2_DrawStr(&my_u8g2,0,12, "distance:");
  sprintf(tmp, "%d mm", mm);
  u8g2_DrawStr(&my_u8g2,0,40, tmp);

  u8g2_SendBuffer(&my_u8g2);
}













/////////////////////////////////////////////////////////////////////////////////////
//进度条显�?
void testDrawProcess(u8g2_t *u8g2)
{
	for(int i=10;i<=80;i=i+2)
	{
		u8g2_ClearBuffer(u8g2); 
			
		char buff[20];
		sprintf(buff,"%d%%",(int)(i/80.0*100));
		
		u8g2_SetFont(u8g2,u8g2_font_ncenB12_tf);
		u8g2_DrawStr(u8g2,16,32,"STM32 U8g2");//字�?�显�?
		
		u8g2_SetFont(u8g2,u8g2_font_ncenB08_tf);
		u8g2_DrawStr(u8g2,100,49,buff);//当前进度显示
		
		u8g2_DrawRBox(u8g2,16,40,i,10,4);//圆�?�填充�?�矩形�??
		u8g2_DrawRFrame(u8g2,16,40,80,10,4);//圆�?�矩�?
		
		u8g2_SendBuffer(u8g2);
	}
	HAL_Delay(500);
}
 
 
//字体测试 数字英文�?选用 u8g2_font_ncenB..(�?) 系列字体
//u8g2_font_unifont_t_symbols/u8g2_font_unifont_h_symbols(�? 圆润)
void testShowFont(u8g2_t *u8g2)
{
	int t = 1000;
	char testStr[14] = "STM32F103C8T6";
	
	u8g2_ClearBuffer(u8g2);
	
	u8g2_SetFont(u8g2,u8g2_font_u8glib_4_tf);
	u8g2_DrawStr(u8g2,0,5,testStr);
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	
	u8g2_SetFont(u8g2,u8g2_font_ncenB08_tf);
	u8g2_DrawStr(u8g2,0,30,testStr);
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	
  u8g2_SetFont(u8g2,u8g2_font_ncenB10_tr);
	u8g2_DrawStr(u8g2,0,60,testStr);
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
}
 
//画空心矩�?
void testDrawFrame(u8g2_t *u8g2)
{
	int t = 1000;
	int x = 16;
	int y = 32;
	int w = 50;
	int h = 20;
	u8g2_ClearBuffer(u8g2);
	u8g2_DrawStr(u8g2,0, 15, "DrawFrame");
 
	u8g2_DrawFrame(u8g2, x, y, w, h);
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	u8g2_DrawFrame(u8g2, x+w+5, y-10, w-20, h+20);
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
}
 
//画实心圆角矩�?
void testDrawRBox(u8g2_t *u8g2)
{
	int t = 1000;
	int x = 16;
	int y = 32;
	int w = 50;
	int h = 20;
	int r = 3;
	u8g2_ClearBuffer(u8g2);
	u8g2_DrawStr(u8g2,0, 15, "DrawRBox");
 
	u8g2_DrawRBox(u8g2, x, y, w, h, r);
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	u8g2_DrawRBox(u8g2, x+w+5, y-10, w-20, h+20, r);
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
}
 
//画空心圆
void testDrawCircle(u8g2_t *u8g2)
{
	int t = 600;
	int stx = 0;  //画图起�?�x
	int sty = 16; //画图起�?�y
	int with = 16;//一�?图块的间�?
	int r = 15;   //圆的半径
	u8g2_ClearBuffer(u8g2);
	u8g2_DrawStr(u8g2, 0, 15, "DrawCircle");
 
	u8g2_DrawCircle(u8g2, stx, sty - 1 + with, r, U8G2_DRAW_UPPER_RIGHT); //右上
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	u8g2_DrawCircle(u8g2, stx + with, sty, r, U8G2_DRAW_LOWER_RIGHT); //右下
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	u8g2_DrawCircle(u8g2, stx - 1 + with * 3, sty - 1 + with, r, U8G2_DRAW_UPPER_LEFT); //左上
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	u8g2_DrawCircle(u8g2, stx - 1 + with * 4, sty, r, U8G2_DRAW_LOWER_LEFT); //左下
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	u8g2_DrawCircle(u8g2, stx - 1 + with * 2, sty - 1 + with * 2, r, U8G2_DRAW_ALL);//整个�?
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	
    u8g2_DrawCircle(u8g2, 32*3, 32, 31, U8G2_DRAW_ALL);//右侧整个�?
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
}
 
//画实心椭�?
void testDrawFilledEllipse(u8g2_t *u8g2)
{
	int t = 800;
	int with = 16;//一�?图块的间�?
	int rx = 27;  //�?圆x方向的半�?
	int ry = 22;  //�?圆y方向的半�?
	u8g2_ClearBuffer(u8g2);
	u8g2_DrawStr(u8g2,0, 14, "DrawFilledEllipse");
 
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	u8g2_DrawFilledEllipse(u8g2, 0, with, rx, ry, U8G2_DRAW_LOWER_RIGHT);//右下
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	u8g2_DrawFilledEllipse(u8g2, with * 4 - 1, with, rx, ry, U8G2_DRAW_LOWER_LEFT); //左下
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	u8g2_DrawFilledEllipse(u8g2, 0, with * 4 - 1, rx, ry, U8G2_DRAW_UPPER_RIGHT); //右上
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	u8g2_DrawFilledEllipse(u8g2, with * 4 - 1, with * 4 - 1, rx, ry, U8G2_DRAW_UPPER_LEFT); //左上
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	u8g2_DrawFilledEllipse(u8g2, with * 6, with * 2.5, rx, ry, U8G2_DRAW_ALL);//整个�?�?
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
}
 
//�?形测�?
void testDrawMulti(u8g2_t *u8g2)
{
	u8g2_ClearBuffer(u8g2);
	for (int j = 0; j < 64; j+=16)
	{
		for (int i = 0; i < 128; i+=16)
		{
			u8g2_DrawPixel(u8g2, i, j);
			u8g2_SendBuffer(u8g2);
		}
	}
  
	//实心矩形逐渐变大
    u8g2_ClearBuffer(u8g2);
	for(int i=30; i>0; i-=2)
	{
		u8g2_DrawBox(u8g2,i*2,i,128-i*4,64-2*i);
		u8g2_SendBuffer(u8g2);
	}
	//空心矩形逐渐变小
	u8g2_ClearBuffer(u8g2);
	for(int i=0; i<32; i+=2)
	{
		u8g2_DrawFrame(u8g2,i*2,i,128-i*4,64-2*i);
		u8g2_SendBuffer(u8g2);
	}
	
	//实心圆�?�矩形逐渐变大
	u8g2_ClearBuffer(u8g2);
	for(int i=30; i>0; i-=2)
	{
		u8g2_DrawRBox(u8g2,i*2,i,128-i*4,64-2*i,10-i/3);
		u8g2_SendBuffer(u8g2);
	}
    //空心圆�?�矩形逐渐变小
	u8g2_ClearBuffer(u8g2);
	for(int i=0; i<32; i+=2)
	{
		u8g2_DrawRFrame(u8g2,i*2,i,128-i*4,64-2*i,10-i/3);
		u8g2_SendBuffer(u8g2);
	}
	
	//实心圆逐渐变大
	u8g2_ClearBuffer(u8g2);
	for(int i=2; i<64; i+=3)
	{
		u8g2_DrawDisc(u8g2,64,32,i, U8G2_DRAW_ALL);
		u8g2_SendBuffer(u8g2);
	}
	//空心圆逐渐变小
	u8g2_ClearBuffer(u8g2);
	for(int i=64; i>0; i-=3)
	{
		u8g2_DrawCircle(u8g2,64,32,i, U8G2_DRAW_ALL);
		u8g2_SendBuffer(u8g2);
	}
	
	//实心�?圆逐渐变大
    u8g2_ClearBuffer(u8g2);
	for(int i=2; i<32; i+=3)
	{
		u8g2_DrawFilledEllipse(u8g2,64,32, i*2, i, U8G2_DRAW_ALL);
		u8g2_SendBuffer(u8g2);
	}
    //空心�?圆逐渐变小
    u8g2_ClearBuffer(u8g2);
	for(int i=32; i>0; i-=3)
	{
		u8g2_DrawEllipse(u8g2,64,32, i*2, i, U8G2_DRAW_ALL);
		u8g2_SendBuffer(u8g2);
	}
}
 
 
// width: 128, height: 48
const unsigned char bilibili[] U8X8_PROGMEM = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0xe0, 0x03, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0xf0, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x80, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0xff, 0xff, 0x01, 0xfc, 0x00, 0x00, 0x00, 0xc0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x03, 0xfc, 0x00, 0x00, 0x3c, 0xc0, 0x0f, 0x00, 0x80, 0x03, 0x00, 0x80, 0xff, 0xff, 0xff, 0xff, 0x07, 0xfc, 0x00, 0x00, 0x3c, 0xc0, 0x0f, 0x00, 0xc0, 0x07, 0x00, 0x80, 0xff, 0xff, 0xff, 0xff, 0x0f, 0xfc, 0x00, 0x00, 0x3c, 0x80, 0x0f, 0x00, 0xc0, 0x07, 0x00, 0xc0, 0x0f, 0x00, 0x00, 0x80, 0x0f, 0xf8, 0x00, 0x00, 0x3c, 0x80, 0x0f, 0x00, 0x80, 0x07, 0x00, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x78, 0x80, 0x0f, 0x00, 0x80, 0x07, 0x00, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x78, 0x80, 0x0f, 0x00, 0x80, 0x07, 0x00, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x80, 0x79, 0x80, 0x0f, 0x00, 0x98, 0x07, 0x00, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0xe0, 0x79, 0x9f, 0x0f, 0x00, 0xbe, 0xe7, 0x01, 0xc0, 0x07, 0x10, 0x40, 0x00, 0x1f, 0xf8, 0x00, 0xe0, 0x7b, 0x1f, 0x0f, 0x00, 0xbe, 0xe7, 0x01, 0xc0, 0x87, 0x1f, 0xe0, 0x0f, 0x1f, 0xf8, 0x00, 0xe0, 0x7b, 0x1e, 0x0f, 0x00, 0x3e, 0xe7, 0x01, 0xc0, 0xe7, 0x3f, 0xe0, 0x3f, 0x1f, 0xf0, 0x00, 0xe0, 0x7b, 0x1e, 0x0f, 0x00, 0x3e, 0xe7, 0x01, 0xc0, 0xe7, 0x3f, 0xe0, 0x3f, 0x1f, 0xf0, 0x00, 0x60, 0x71, 0x1e, 0x0f, 0x00, 0x34, 0xe7, 0x01, 0xc0, 0xe7, 0x07, 0x00, 0x3f, 0x1f, 0xf0, 0x00, 0x00, 0x70, 0x00, 0x1f, 0x00, 0x00, 0x07, 0x00, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0xc0, 0x73, 0x1e, 0x1f, 0x00, 0x3c, 0xc7, 0x01, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0xc0, 0x73, 0x1e, 0x1f, 0x00, 0x7c, 0xe7, 0x01, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0xc0, 0x73, 0x1e, 0x1f, 0x00, 0x7c, 0xef, 0x01, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x1f, 0xf0, 0x01, 0xc0, 0x77, 0x1e, 0x1e, 0x00, 0x7c, 0xef, 0x01, 0xc0, 0x07, 0x00, 0x03, 0x00, 0x1f, 0xf0, 0xff, 0xc1, 0xf7, 0x1e, 0xfe, 0x1f, 0x78, 0xef, 0x01, 0xc0, 0x07, 0x70, 0x37, 0x00, 0x1f, 0xe0, 0xff, 0x87, 0xf7, 0x1e, 0xfe, 0xff, 0x78, 0xee, 0x01, 0xc0, 0x07, 0xe0, 0x3f, 0x00, 0x1f, 0xe0, 0xff, 0x9f, 0xf7, 0x1e, 0xfe, 0xff, 0x79, 0xce, 0x01, 0xc0, 0x07, 0xc0, 0x18, 0x00, 0x1f, 0xe0, 0xff, 0xbf, 0xe7, 0x1e, 0xfe, 0xff, 0x7b, 0xce, 0x01, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0xc7, 0xbf, 0xe7, 0x1e, 0xfe, 0xf8, 0x77, 0xce, 0x01, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x0f, 0x3f, 0xe7, 0x1c, 0xfe, 0xf0, 0x77, 0xce, 0x03, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0xcf, 0x3f, 0xe7, 0x1c, 0xfe, 0xf8, 0xf3, 0xce, 0x03, 0xc0, 0xff, 0xff, 0xff, 0xff, 0x0f, 0xe0, 0xef, 0x1f, 0xe7, 0x1c, 0xfe, 0xfe, 0xf1, 0xce, 0x03, 0x80, 0xff, 0xff, 0xff, 0xff, 0x0f, 0xe0, 0xff, 0x0f, 0xcf, 0x1c, 0xfc, 0xff, 0xf0, 0xc0, 0x03, 0x00, 0xff, 0xff, 0xff, 0xff, 0x07, 0xe0, 0xff, 0x03, 0x06, 0x1c, 0xfc, 0x7f, 0x60, 0xc0, 0x01, 0x00, 0xfe, 0xff, 0xff, 0xff, 0x03, 0xe0, 0xff, 0x00, 0x00, 0x00, 0xfc, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0xff, 0xff, 0x00, 0xc0, 0x0f, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x03, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
// width: 128, height: 48
const unsigned char three_support[] U8X8_PROGMEM = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x01, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x03, 0x00, 0x00, 0x00, 0xfc, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x03, 0x00, 0x00, 0x00, 0xfe, 0x7f, 0x00, 0x00, 0x00, 0x80, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x03, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x80, 0x1f, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x03, 0x00, 0x00, 0x80, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0xc0, 0x3f, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x03, 0x00, 0x00, 0x80, 0x0f, 0xf0, 0x01, 0x00, 0x00, 0xfc, 0xff, 0x01, 0x00, 0x00, 0xc0, 0xfd, 0xff, 0x00, 0x00, 0xc0, 0x7f, 0xfe, 0x01, 0x00, 0x00, 0xff, 0xff, 0x0f, 0x00, 0x00, 0xe0, 0xfd, 0xff, 0x01, 0x00, 0xc0, 0x1f, 0xf8, 0x03, 0x00, 0x00, 0xff, 0xff, 0x0f, 0x00, 0x00, 0xe0, 0xfd, 0xff, 0x01, 0x00, 0xc0, 0x0f, 0xf0, 0x03, 0x00, 0x00, 0xfe, 0xff, 0x07, 0x00, 0x00, 0xe0, 0xfd, 0xff, 0x01, 0x00, 0xc0, 0x67, 0xe6, 0x03, 0x00, 0x00, 0xfc, 0xff, 0x03, 0x00, 0x00, 0xe0, 0xfd, 0xff, 0x01, 0x00, 0xc0, 0x67, 0xe6, 0x03, 0x00, 0x00, 0xf8, 0xff, 0x01, 0x00, 0x00, 0xe0, 0xfd, 0xff, 0x00, 0x00, 0xc0, 0x67, 0xe6, 0x03, 0x00, 0x00, 0xf0, 0x7f, 0x00, 0x00, 0x00, 0xe0, 0xfd, 0xff, 0x00, 0x00, 0xc0, 0x67, 0xee, 0x03, 0x00, 0x00, 0xe0, 0x7f, 0x00, 0x00, 0x00, 0xe0, 0xfd, 0xff, 0x00, 0x00, 0x80, 0x7f, 0xfe, 0x01, 0x00, 0x00, 0xe0, 0xff, 0x00, 0x00, 0x00, 0xe0, 0xfd, 0xff, 0x00, 0x00, 0x80, 0x7f, 0xfe, 0x01, 0x00, 0x00, 0xf0, 0xff, 0x00, 0x00, 0x00, 0xe0, 0xfd, 0x7f, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0xf0, 0xff, 0x00, 0x00, 0x00, 0xe0, 0xfd, 0x7f, 0x00, 0x00, 0x00, 0xfe, 0xff, 0x00, 0x00, 0x00, 0xf8, 0xf9, 0x01, 0x00, 0x00, 0xe0, 0xfd, 0x7f, 0x00, 0x00, 0x00, 0xfe, 0x7f, 0x00, 0x00, 0x00, 0xf8, 0xf0, 0x00, 0x00, 0x00, 0xe0, 0xfd, 0x1f, 0x00, 0x00, 0x00, 0xf8, 0x1f, 0x00, 0x00, 0x00, 0x30, 0xc0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
 
void testDrawXBM(u8g2_t *u8g2)
{
	int t = 1000;
	u8g2_ClearBuffer(u8g2);
	u8g2_DrawStr(u8g2,0, 14, "DrawXBM");
 
	u8g2_DrawXBM(u8g2,0, 16, 128, 48, bilibili);
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
 
	u8g2_ClearBuffer(u8g2);
	u8g2_DrawStr(u8g2,0, 14, "bilibili");
	u8g2_DrawXBM(u8g2,0, 16, 128, 48, three_support);
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
}
 
void u8g2DrawTest(u8g2_t *u8g2)
{
	testDrawProcess(u8g2);
	testDrawMulti(u8g2);
	//testDrawFrame(u8g2);
	//testDrawRBox(u8g2);
	//testDrawCircle(u8g2);
	//testDrawFilledEllipse(u8g2);
	testShowFont(u8g2);
	testDrawXBM(u8g2);
 
}

