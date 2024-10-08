#include "com_rc.h"

void rc_data_init(rc_data_t* d)
{
    d->rk_l_x =  0;
    d->rk_l_y =  0;
    d->rk_r_x =  0;
    d->rk_r_y =  0;
    d->rk_l_z =  0;
    d->rk_r_z =  0;
    d->key_l   =  0;
    d->key_r   =  0;
    d->sw_l_1 =  0;
    d->sw_l_2 =  0;
    d->sw_l_3 =  0;
    d->sw_r_1 =  0;
    d->sw_r_2 =  0;
    d->sw_r_3 =  0;

    for (int i = 0; i < RC_DATA_LEN; i++)
      d->buf[i] = 0;
}

// 0 - 成功
uint8_t rc_data_encode(rc_data_t* rc)
{
  rc->buf[0] = RC_DATA_HEAD;
  rc->buf[1] = (rc->rk_l_x >> 8) & 0xff;
  rc->buf[2] =  rc->rk_l_x & 0xff;
  rc->buf[3] = (rc->rk_l_y >> 8) & 0xff;
  rc->buf[4] =  rc->rk_l_y & 0xff;

  rc->buf[5] = (rc->rk_r_x >> 8) & 0xff;
  rc->buf[6] =  rc->rk_r_x & 0xff;
  rc->buf[7] = (rc->rk_r_y >> 8) & 0xff;
  rc->buf[8] =  rc->rk_r_y & 0xff;

  rc->buf[9] = 0x00;
  rc->buf[9] |= (rc->rk_l_z & 0x01) << 7;
  rc->buf[9] |= (rc->rk_r_z & 0x01) << 6;
  rc->buf[9] |= (rc->key_l   & 0x01) << 5;
  rc->buf[9] |= (rc->key_r   & 0x01) << 4;

  rc->buf[10] = 0x00;
  rc->buf[10] |= (rc->sw_l_1 & 0x01) << 7;
  rc->buf[10] |= (rc->sw_l_2 & 0x01) << 6;
  rc->buf[10] |= (rc->sw_l_3 & 0x01) << 5;
  rc->buf[10] |= (rc->sw_r_1 & 0x01) << 4;
  rc->buf[10] |= (rc->sw_r_2 & 0x01) << 3;
  rc->buf[10] |= (rc->sw_r_3 & 0x01) << 2;

  rc->buf[RC_DATA_LEN-1] = checksum_gen(rc->buf); 
  return 0;
}

uint8_t rc_data_decode(rc_data_t* rc)
{
  if (rc->buf[0] != RC_DATA_HEAD || checksum_check(rc->buf)) return 1;

  rc->rk_l_x = rc->buf[1] << 8 | rc->buf[2];
  rc->rk_l_y = rc->buf[3] << 8 | rc->buf[4];

  rc->rk_r_x = rc->buf[5] << 8 | rc->buf[6];
  rc->rk_r_y = rc->buf[7] << 8 | rc->buf[8];

  rc->rk_l_z = (rc->buf[9] >> 7) & 0x01;
  rc->rk_r_z = (rc->buf[9] >> 6) & 0x01;
  rc->key_l   = (rc->buf[9] >> 5) & 0x01;
  rc->key_r   = (rc->buf[9] >> 4) & 0x01;

  rc->sw_l_1 = (rc->buf[10] >> 7) & 0x01;
  rc->sw_l_2 = (rc->buf[10] >> 6) & 0x01;
  rc->sw_l_3 = (rc->buf[10] >> 5) & 0x01;
  rc->sw_r_1 = (rc->buf[10] >> 4) & 0x01;
  rc->sw_r_2 = (rc->buf[10] >> 3) & 0x01;
  rc->sw_r_3 = (rc->buf[10] >> 2) & 0x01;

  return 0;
}

uint8_t checksum_gen(uint8_t *data) 
{ 
  uint8_t i, crc = 0;

  for(i=1; i<RC_DATA_LEN-1; i++)
  {
    crc += data[i];
  }
  crc = ~crc;
  return crc;
}
// 0 - 正确
uint8_t checksum_check(uint8_t *data)
{ 
  uint8_t i, crc = 0;

  for(i=1; i<RC_DATA_LEN; i++)
  {
    crc += data[i];
  }
  return crc+1;
}
