#ifndef __MPUIIC_H
#define __MPUIIC_H
#include "gpio.h"

//IO��������  ---PB11
// #define MPU_SDA_IN()  {GPIOB->CRH &= 0XFFFF0FFF;GPIOB->CRH |= 8<<12;}   //����/���� ����ģʽ
// #define MPU_SDA_IN() \
//   GPIO_InitTypeDef GPIO_InitStruct = {0};\
//   GPIO_InitStruct.Pin = MPU_I2C_SDA_Pin;\
//   GPIO_InitStruct.Mode = GPIO_MODE_INPUT;\
//   GPIO_InitStruct.Pull = GPIO_NOPULL;\
//   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;\
//   HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

// #define MPU_SDA_OUT() {GPIOB->CRH &= 0XFFFF0FFF;GPIOB->CRH |= 3<<12;}		//�������  ���ģʽ
// #define MPU_SDA_OUT() \
//   GPIO_InitTypeDef GPIO_InitStruct = {0};\
//   GPIO_InitStruct.Pin = MPU_I2C_SDA_Pin;\
//   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;\
//   GPIO_InitStruct.Pull = GPIO_NOPULL;\
//   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;\
//   HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//IO��������	 
// #define MPU_IIC_SCL    PBout(10) 		//SCL
// #define MPU_IIC_SDA    PBout(11) 		//SDA	 
// #define MPU_READ_SDA   PBin(11) 		//����SDA 
// 注意使用软件iic，而不是硬件！
#define MPU_IIC_SCL_H() HAL_GPIO_WritePin(GPIOB, MPU_I2C_SCL_Pin, GPIO_PIN_SET)
#define MPU_IIC_SCL_L() HAL_GPIO_WritePin(GPIOB, MPU_I2C_SCL_Pin, GPIO_PIN_RESET)

#define MPU_IIC_SDA_H() HAL_GPIO_WritePin(GPIOB, MPU_I2C_SDA_Pin, GPIO_PIN_SET)
#define MPU_IIC_SDA_L() HAL_GPIO_WritePin(GPIOB, MPU_I2C_SDA_Pin, GPIO_PIN_RESET)

#define MPU_READ_SDA() HAL_GPIO_ReadPin(GPIOB, MPU_I2C_SDA_Pin)


//IIC���в�������
void MPU_IIC_Delay(void);								//IIC��ʱ2ms����
void MPU_IIC_Init(void);                //��ʼ��IIC��IO��				 
void MPU_IIC_Start(void);								//����IIC��ʼ�ź�
void MPU_IIC_Stop(void);	  						//����IICֹͣ�ź�
void MPU_IIC_Send_Byte(uint8_t txd);					//IIC����һ���ֽ�
uint8_t MPU_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
uint8_t MPU_IIC_Wait_Ack(void); 							//IIC�ȴ�ACK�ź�
void MPU_IIC_Ack(void);									//IIC����ACK�ź�
void MPU_IIC_NAck(void);								//IIC������ACK�ź�


void IMPU_IC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t MPU_IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	  

#endif
