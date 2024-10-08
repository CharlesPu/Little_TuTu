#include "mpu6050.h"
#include "main.h"
#include "usart.h"   

/**********************************************
�������ƣ�MPU_Init
�������ܣ���ʼ��MPU6050
������������
��������ֵ��0,��ʼ���ɹ�  ����,��ʼ��ʧ��
**********************************************/
uint8_t MPU_Init(void)
{ 
	uint8_t res;
	
	// MPU_AD0_CTRL=0;			//����MPU6050��AD0��Ϊ�͵�ƽ,�ӻ���ַΪ:0X68
	
	MPU_IIC_Init();			//��ʼ��IIC����
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X80);	//��λMPU6050
  HAL_Delay(100);
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X00);	//����MPU6050 
	MPU_Set_Gyro_Fsr(3);										//�����Ǵ�����,��2000dps
	MPU_Set_Accel_Fsr(0);										//���ٶȴ�����,��2g
	MPU_Set_Rate(50);												//���ò�����50Hz
	MPU_Write_Byte(MPU_INT_EN_REG,0X00);		//�ر������ж�
	MPU_Write_Byte(MPU_USER_CTRL_REG,0X00);	//I2C��ģʽ�ر�
	MPU_Write_Byte(MPU_FIFO_EN_REG,0X00);		//�ر�FIFO
	MPU_Write_Byte(MPU_INTBP_CFG_REG,0X80);	//INT���ŵ͵�ƽ��Ч
	
	res=MPU_Read_Byte(MPU_DEVICE_ID_REG);
	if(res==MPU_ADDR)												//����ID��ȷ,��res = MPU_ADDR = 0x68
	{
		MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X01);		//����CLKSEL,PLL X��Ϊ�ο�
		MPU_Write_Byte(MPU_PWR_MGMT2_REG,0X00);		//���ٶ��������Ƕ�����
		MPU_Set_Rate(50);													//���ò�����Ϊ50Hz
 	}else return 1;    //��ַ���ô���,����1
	return 0;					 //��ַ������ȷ,����0
}

/**********************************************
�������ƣ�MPU_Set_Gyro_Fsr
�������ܣ�����MPU6050�����Ǵ����������̷�Χ
����������fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
��������ֵ��0,���óɹ�  ����,����ʧ��
**********************************************/
uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
{
	return MPU_Write_Byte(MPU_GYRO_CFG_REG,fsr<<3); //���������������̷�Χ
}

/**********************************************
�������ƣ�MPU_Set_Accel_Fsr
�������ܣ�����MPU6050���ٶȴ����������̷�Χ
����������fsr:0,��2g;1,��4g;2,��8g;3,��16g
��������ֵ��0,���óɹ�  ����,����ʧ��
**********************************************/
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
	return MPU_Write_Byte(MPU_ACCEL_CFG_REG,fsr<<3); //���ü��ٶȴ����������̷�Χ  
}

/**********************************************
�������ƣ�MPU_Set_LPF
�������ܣ�����MPU6050�����ֵ�ͨ�˲���
����������lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
��������ֵ��0,���óɹ�  ����,����ʧ��
**********************************************/
uint8_t MPU_Set_LPF(uint16_t lpf)
{
	uint8_t data=0;
	
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(MPU_CFG_REG,data);//�������ֵ�ͨ�˲���  
}

/**********************************************
�������ƣ�MPU_Set_Rate
�������ܣ�����MPU6050�Ĳ�����(�ٶ�Fs=1KHz)
����������rate:4~1000(Hz)  ��ʼ����rateȡ50
��������ֵ��0,���óɹ�  ����,����ʧ��
**********************************************/
uint8_t MPU_Set_Rate(uint16_t rate)
{
	uint8_t data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);	//�������ֵ�ͨ�˲���
 	return MPU_Set_LPF(rate/2);											//�Զ�����LPFΪ�����ʵ�һ��
}

/**********************************************
�������ƣ�MPU_Get_Temperature
�������ܣ��õ��¶ȴ�����ֵ
������������
��������ֵ���¶�ֵ(������100��)
**********************************************/
short MPU_Get_Temperature(void)
{
   uint8_t buf[2]; 
   short raw;
	 float temp;
	
	 MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf); 
   raw=((uint16_t)buf[0]<<8)|buf[1];
   temp=36.53+((double)raw)/340;
   return temp*100;
}

/**********************************************
�������ƣ�MPU_Get_Gyroscope
�������ܣ��õ�������ֵ(ԭʼֵ)
����������gx,gy,gz:������x,y,z���ԭʼ����(������)
��������ֵ��0,��ȡ�ɹ�  ����,��ȡʧ��
**********************************************/
uint8_t MPU_Get_Gyroscope(imu_data_t* data)
{
  uint8_t buf[6],res;
	
	res=MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		data->gyrox=((uint16_t)buf[0]<<8)|buf[1];
		data->gyroy=((uint16_t)buf[2]<<8)|buf[3];
		data->gyroz=((uint16_t)buf[4]<<8)|buf[5];
	} 	
  return res;
}

/**********************************************
�������ƣ�MPU_Get_Accelerometer
�������ܣ��õ����ٶ�ֵ(ԭʼֵ)
����������ax,ay,az:���ٶȴ�����x,y,z���ԭʼ����(������)
��������ֵ��0,��ȡ�ɹ�  ����,��ȡʧ��
**********************************************/
uint8_t MPU_Get_Accelerometer(imu_data_t *data)
{
    uint8_t buf[6],res;  
	res=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		data->aacx=((uint16_t)buf[0]<<8)|buf[1];  
		data->aacy=((uint16_t)buf[2]<<8)|buf[3];  
		data->aacz=((uint16_t)buf[4]<<8)|buf[5];
	} 	
    return res;
}

/**********************************************
�������ƣ�MPU_Write_Len
�������ܣ�IIC����д(д������ַ���Ĵ�����ַ������)
����������addr:������ַ      reg:�Ĵ�����ַ
				 len:д�����ݵĳ���  buf:������
��������ֵ��0,д��ɹ�  ����,д��ʧ��
**********************************************/
uint8_t MPU_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
	uint8_t i;
	
	MPU_IIC_Start();
	MPU_IIC_Send_Byte((addr<<1)|0);      //����������ַ+д����(0Ϊд,1Ϊ��)	
	if(MPU_IIC_Wait_Ack())							 //�ȴ�Ӧ��
	{
		MPU_IIC_Stop();
		return 1;
	}
    MPU_IIC_Send_Byte(reg);						 //д�Ĵ�����ַ
    MPU_IIC_Wait_Ack();		             //�ȴ�Ӧ��
	for(i=0;i<len;i++)
	{
		MPU_IIC_Send_Byte(buf[i]);	       //��������
		if(MPU_IIC_Wait_Ack())		         //�ȴ�ACK
		{
			MPU_IIC_Stop();
			return 1;
		}
	}
    MPU_IIC_Stop();
	return 0;
}

/**********************************************
�������ƣ�MPU_Read_Len
�������ܣ�IIC������(д��������ַ��,���Ĵ�����ַ������)
����������addr:������ַ        reg:Ҫ���ļĴ�����ַ
				 len:Ҫ��ȡ�����ݳ���  buf:��ȡ�������ݴ洢��
��������ֵ��0,��ȡ�ɹ�  ����,��ȡʧ��
**********************************************/
uint8_t MPU_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
		MPU_IIC_Start();
		MPU_IIC_Send_Byte((addr<<1)|0);		//����������ַ+д����
		if(MPU_IIC_Wait_Ack())						//�ȴ�Ӧ��
		{
			MPU_IIC_Stop();		 
			return 1;
		}
    MPU_IIC_Send_Byte(reg);						//д�Ĵ�����ַ
    MPU_IIC_Wait_Ack();								//�ȴ�Ӧ��
    MPU_IIC_Start();
		MPU_IIC_Send_Byte((addr<<1)|1);		//����������ַ+������	
    MPU_IIC_Wait_Ack();								//�ȴ�Ӧ�� 
		while(len)
		{
			if(len==1) *buf=MPU_IIC_Read_Byte(0);   //������,����nACK 
			else 			 *buf=MPU_IIC_Read_Byte(1);		//������,����ACK  
			len--;
			buf++;
		}
    MPU_IIC_Stop();	//����һ��ֹͣ���� 
		return 0;	
}

/**********************************************
�������ƣ�MPU_Write_Byte
�������ܣ�IICдһ���ֽ�
����������data:д�������    reg:Ҫд�ļĴ�����ַ
��������ֵ��0,д��ɹ�  ����,д��ʧ��
**********************************************/
uint8_t MPU_Write_Byte(uint8_t reg,uint8_t data)
{
  MPU_IIC_Start();
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);//����������ַ+д����
	if(MPU_IIC_Wait_Ack())						 //�ȴ�Ӧ��
	{
		MPU_IIC_Stop();
		return 1;
	}
  MPU_IIC_Send_Byte(reg);		//д�Ĵ�����ַ
  MPU_IIC_Wait_Ack();				//�ȴ�Ӧ��
	MPU_IIC_Send_Byte(data);	//��������
	if(MPU_IIC_Wait_Ack())	  //�ȴ�ACK
	{
		MPU_IIC_Stop();
		return 1;
	}
  MPU_IIC_Stop();
	return 0;
}

/**********************************************
�������ƣ�MPU_Read_Byte
�������ܣ�IIC��һ���ֽ�
����������reg:Ҫ���ļĴ�����ַ
��������ֵ��res:��ȡ��������
**********************************************/
uint8_t MPU_Read_Byte(uint8_t reg)
{
	uint8_t res;
	
  MPU_IIC_Start(); 
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);		//����������ַ+д����	
	MPU_IIC_Wait_Ack();										//�ȴ�Ӧ�� 
  MPU_IIC_Send_Byte(reg);								//д�Ĵ�����ַ
  MPU_IIC_Wait_Ack();										//�ȴ�Ӧ��
  MPU_IIC_Start();
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|1);		//����������ַ+������	
  MPU_IIC_Wait_Ack();										//�ȴ�Ӧ�� 
	res=MPU_IIC_Read_Byte(0);							//��ȡ����,����nACK 
  MPU_IIC_Stop();												//����һ��ֹͣ���� 
	return res;		
}
