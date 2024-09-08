#include "mpuiic.h"
#include "tim.h"
 

void MPU_SDA_IN()
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = MPU_I2C_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
} 
void MPU_SDA_OUT()
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = MPU_I2C_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
} 
/**********************************************
函数名称：MPU_IIC_Delay
函数功能：MPU IIC延时函数，延时2ms
函数参数：无
函数返回值：无
**********************************************/
void MPU_IIC_Delay(void)
{
	Tims_delay_us(2);
}

/**********************************************
函数名称：MPU_IIC_Init
函数功能：MPU IIC初始化
函数参数：无
函数返回值：无
**********************************************/
void MPU_IIC_Init(void)
{					     
  // GPIO_InitTypeDef  GPIO_InitStructure;
	
	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);			//先使能外设IO PORTB时钟 
		
  // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;	  //端口配置
  // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 				  //推挽输出
  // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				  //IO口速度为50MHz
  // GPIO_Init(GPIOB, &GPIO_InitStructure);					 					//根据设定参数初始化GPIO 
	
  // GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11);						  //PB10,PB11 输出高	
 
}

/**********************************************
函数名称：MPU_IIC_Start
函数功能：MPU IIC发送起始信号
函数参数：无
函数返回值：无
**********************************************/
void MPU_IIC_Start(void)
{
	MPU_SDA_OUT();     //SDA线 输出
	MPU_IIC_SDA_H();	  	  
	MPU_IIC_SCL_H();
	MPU_IIC_Delay();
 	MPU_IIC_SDA_L();     //START:当SCL线处于高电平时,SDA线突然从高变低,发送起始信号
	MPU_IIC_Delay();
	MPU_IIC_SCL_L();		 //钳住I2C总线，准备发送或接收数据 
}

/**********************************************
函数名称：MPU_IIC_Stop
函数功能：MPU IIC发送停止信号
函数参数：无
函数返回值：无
**********************************************/
void MPU_IIC_Stop(void)
{
	MPU_SDA_OUT();		 //SDA线输出
	MPU_IIC_SCL_L();
	MPU_IIC_SDA_L();		 //STOP:当SCL线处于高电平时,SDA线突然从低变高,发送停止信号
 	MPU_IIC_Delay();
	MPU_IIC_SCL_H(); 
	MPU_IIC_SDA_H();		 //发送I2C总线结束信号
	MPU_IIC_Delay();							   	
}

/**********************************************
函数名称：MPU_IIC_Wait_Ack
函数功能：MPU IIC等待信号到来
函数参数：无
函数返回值：1:接收应答信号成功  0:接收应答信号失败
**********************************************/
uint8_t MPU_IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	MPU_SDA_IN();  //SDA设置为输入  
	MPU_IIC_SDA_H();MPU_IIC_Delay();
	MPU_IIC_SCL_H();MPU_IIC_Delay();
	while(MPU_READ_SDA())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			MPU_IIC_Stop();
			return 1;
		}
	}
	MPU_IIC_SCL_L();//时钟输出0
	return 0;
} 

/**********************************************
函数名称：MPU_IIC_Ack
函数功能：MPU IIC产生应答信号
函数参数：无
函数返回值：无
**********************************************/
void MPU_IIC_Ack(void)
{
	MPU_IIC_SCL_L();
	MPU_SDA_OUT();
	MPU_IIC_SDA_L();
	MPU_IIC_Delay();
	MPU_IIC_SCL_H();
	MPU_IIC_Delay();
	MPU_IIC_SCL_L();
}

/**********************************************
函数名称：MPU_IIC_NAck
函数功能：MPU IIC不产生应答信号
函数参数：无
函数返回值：无
**********************************************/   
void MPU_IIC_NAck(void)
{
	MPU_IIC_SCL_L();
	MPU_SDA_OUT();
	MPU_IIC_SDA_H();
	MPU_IIC_Delay();
	MPU_IIC_SCL_H();
	MPU_IIC_Delay();
	MPU_IIC_SCL_L();
}

/**********************************************
函数名称：MPU_IIC_Send_Byte
函数功能：MPU IIC发送一个字节
函数参数：txd：要发送的数据
函数返回值：无
注意：IIC发送字节是一个一个位发送的，发送一个字节需要发送八次
**********************************************/
void MPU_IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
		MPU_SDA_OUT(); 	    
    MPU_IIC_SCL_L();		//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {          
      if ((txd&0x80)>>7) MPU_IIC_SDA_H();else MPU_IIC_SDA_L();
        txd<<=1; 	  
		    MPU_IIC_SCL_H();
		    MPU_IIC_Delay(); 
		    MPU_IIC_SCL_L();	
		    MPU_IIC_Delay();
    }	 
} 	    

/**********************************************
函数名称：MPU_IIC_Read_Byte
函数功能：MPU IIC读取一个字节
函数参数：ack: 1,发送ACK   0,发送NACK 
函数返回值：接收到的数据
注意：IIC读取字节是一个一个位读取的，读取一个字节需要读取八次
**********************************************/ 
uint8_t MPU_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
  MPU_SDA_IN();              //SDA设置为输入
    for(i=0;i<8;i++)
	  {
        MPU_IIC_SCL_L();
        MPU_IIC_Delay();
				MPU_IIC_SCL_H();
        receive<<=1;
        if(MPU_READ_SDA())receive++;   //如果读到了数据
				MPU_IIC_Delay(); 
    }					 
    if (!ack)
        MPU_IIC_NAck();   //发送nACK
    else
        MPU_IIC_Ack();    //发送ACK   
    return receive;
}
