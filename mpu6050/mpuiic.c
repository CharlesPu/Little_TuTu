#include "mpuiic.h"
#include "tim.h"
 

void MPU_SDA_IN()
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = MPU_I2C_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP; // note!!!!!
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
} 
void MPU_SDA_OUT()
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = MPU_I2C_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
} 
/**********************************************
�������ƣ�MPU_IIC_Delay
�������ܣ�MPU IIC��ʱ��������ʱ2ms
������������
��������ֵ����
**********************************************/
void MPU_IIC_Delay(void)
{
	Tims_delay_us(2);
}

/**********************************************
�������ƣ�MPU_IIC_Init
�������ܣ�MPU IIC��ʼ��
������������
��������ֵ����
**********************************************/
void MPU_IIC_Init(void)
{					     
  // GPIO_InitTypeDef  GPIO_InitStructure;
	
	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);			//��ʹ������IO PORTBʱ�� 
		
  // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;	  //�˿�����
  // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 				  //�������
  // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				  //IO���ٶ�Ϊ50MHz
  // GPIO_Init(GPIOB, &GPIO_InitStructure);					 					//�����趨������ʼ��GPIO 
	
  // GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11);						  //PB10,PB11 �����	
 
}

/**********************************************
�������ƣ�MPU_IIC_Start
�������ܣ�MPU IIC������ʼ�ź�
������������
��������ֵ����
**********************************************/
void MPU_IIC_Start(void)
{
	MPU_SDA_OUT();     //SDA�� ���
	MPU_IIC_SDA_H();	  	  
	MPU_IIC_SCL_H();
	MPU_IIC_Delay();
 	MPU_IIC_SDA_L();     //START:��SCL�ߴ��ڸߵ�ƽʱ,SDA��ͻȻ�Ӹ߱��,������ʼ�ź�
	MPU_IIC_Delay();
	MPU_IIC_SCL_L();		 //ǯסI2C���ߣ�׼�����ͻ�������� 
}

/**********************************************
�������ƣ�MPU_IIC_Stop
�������ܣ�MPU IIC����ֹͣ�ź�
������������
��������ֵ����
**********************************************/
void MPU_IIC_Stop(void)
{
	MPU_SDA_OUT();		 //SDA�����
	MPU_IIC_SCL_L();
	MPU_IIC_SDA_L();		 //STOP:��SCL�ߴ��ڸߵ�ƽʱ,SDA��ͻȻ�ӵͱ��,����ֹͣ�ź�
 	MPU_IIC_Delay();
	MPU_IIC_SCL_H(); 
	MPU_IIC_SDA_H();		 //����I2C���߽����ź�
	MPU_IIC_Delay();							   	
}

/**********************************************
�������ƣ�MPU_IIC_Wait_Ack
�������ܣ�MPU IIC�ȴ��źŵ���
������������
��������ֵ��1:����Ӧ���źųɹ�  0:����Ӧ���ź�ʧ��
**********************************************/
uint8_t MPU_IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	MPU_SDA_IN();  //SDA����Ϊ����  
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
	MPU_IIC_SCL_L();//ʱ�����0
	return 0;
} 

/**********************************************
�������ƣ�MPU_IIC_Ack
�������ܣ�MPU IIC����Ӧ���ź�
������������
��������ֵ����
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
�������ƣ�MPU_IIC_NAck
�������ܣ�MPU IIC������Ӧ���ź�
������������
��������ֵ����
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
�������ƣ�MPU_IIC_Send_Byte
�������ܣ�MPU IIC����һ���ֽ�
����������txd��Ҫ���͵�����
��������ֵ����
ע�⣺IIC�����ֽ���һ��һ��λ���͵ģ�����һ���ֽ���Ҫ���Ͱ˴�
**********************************************/
void MPU_IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
		MPU_SDA_OUT(); 	    
    MPU_IIC_SCL_L();		//����ʱ�ӿ�ʼ���ݴ���
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
�������ƣ�MPU_IIC_Read_Byte
�������ܣ�MPU IIC��ȡһ���ֽ�
����������ack: 1,����ACK   0,����NACK 
��������ֵ�����յ�������
ע�⣺IIC��ȡ�ֽ���һ��һ��λ��ȡ�ģ���ȡһ���ֽ���Ҫ��ȡ�˴�
**********************************************/ 
uint8_t MPU_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
  MPU_SDA_IN();              //SDA����Ϊ����
    for(i=0;i<8;i++)
	  {
        MPU_IIC_SCL_L();
        MPU_IIC_Delay();
				MPU_IIC_SCL_H();
        receive<<=1;
        if(MPU_READ_SDA())receive++;   //�������������
				MPU_IIC_Delay(); 
    }					 
    if (!ack)
        MPU_IIC_NAck();   //����nACK
    else
        MPU_IIC_Ack();    //����ACK   
    return receive;
}
