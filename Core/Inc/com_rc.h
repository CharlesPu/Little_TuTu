#ifndef __COM_RC_H
#define __COM_RC_H

#include "main.h"

// 控制信号协议
// 10 Bytes ，大端序
//  B0   | B1   B2  |   B3   B4   |  B5   B6  |   B7   B8   
// 'P'    ( rk1-x )    ( rk1-y )    ( rk2-x )    ( rk2-y ) 
// | B9[ b7     b6     b5     b4     b3-b0]   
//       rk1-z  rk2-z  key_l   key_r      0      
// | B10[ b7     b6     b5     b4     b3     b2     b1-b0] |    B11   
//       sw1-1  sw1-2  sw1-3  sw2-1  sw2-2  sw2-3     0      checksum
#define RC_DATA_HEAD 'P'
#define RC_DATA_LEN 12

typedef struct 
{
  uint16_t rk_l_x;
  uint16_t rk_l_y;
  uint8_t  rk_l_z;

  uint16_t rk_r_x;
  uint16_t rk_r_y;
  uint8_t  rk_r_z;

  uint8_t key_l;
  uint8_t key_r;

  uint8_t sw_l_1;
  uint8_t sw_l_2;
  uint8_t sw_l_3;

  uint8_t sw_r_1;
  uint8_t sw_r_2;
  uint8_t sw_r_3;

  uint8_t buf[RC_DATA_LEN];
}rc_data_t;


void rc_data_init(rc_data_t*);
// 0 - 成功
uint8_t rc_data_encode(rc_data_t*);
uint8_t rc_data_decode(rc_data_t*);

uint8_t checksum_gen(uint8_t *data); //data为数组，len为数组长度
 // 0 - 正确
uint8_t checksum_check(uint8_t *data);

#endif
