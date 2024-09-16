#include "kdr_com.h"
#include "main.h"


uint8 KDCOM_Check(KDRobot* buf)
{
    return '\n';
}

uint8 KDCOM_GetCmd(KDRobot* buf)
{
    return buf->buf[0];
}

uint8 KDCOM_GetCmdPara(KDRobot* buf, uint8 paraNum)
{
    if(paraNum > 3 || paraNum < 1)
        return 0;
    return buf->buf[paraNum];
}

float KDCOM_GetCmdData(KDRobot* buf)
{
  float res;
  uint8 *pp = (uint8*)&res;
  pp[0] = buf->buf[4];
  pp[1] = buf->buf[5];  
  pp[2] = buf->buf[6];
  pp[3] = buf->buf[7];
  return res;
    // return *(float*)&(buf->buf[4]);// note：在stm32上有问题!!!!!!!!!!!会卡死
}

float KDCOM_GetData(KDRobot* buf, uint8 dataNum)
{
    if(dataNum > 2 || dataNum < 1)
        return 0;
    float res;
    uint8 *pp = (uint8*)&res;
    pp[0] = buf->buf[(dataNum - 1) * 4];
    pp[1] = buf->buf[(dataNum - 1) * 4 + 1];  
    pp[2] = buf->buf[(dataNum - 1) * 4 + 2];
    pp[3] = buf->buf[(dataNum - 1) * 4 + 3];
    return res;
    // return *(float*)&(buf->buf[(dataNum - 1) * 4]);// note：在stm32上有问题!!!!!!!!!!!会卡死
}

uint8* KDCOM_SetCmd(KDRobot* buf, uint8 cmd, uint8 para1, uint8 para2, uint8 para3, float data)
{
    buf->head = 'C';
    buf->buf[0] = cmd;
    buf->buf[1] = para1;
    buf->buf[2] = para2;
    buf->buf[3] = para3;

    buf->buf[4] = *((uint8*)&data);  //获取a的首地址
    buf->buf[5] = *((uint8*)&data + 1);//获取a的首地址的后一个字节地址
    buf->buf[6] = *((uint8*)&data + 2);//获取a的首地址的后两个字节地址
    buf->buf[7] = *((uint8*)&data + 3);//获取a的首地址的后三个字节地址

    // *(float*)&buf->buf[4] = data; // note：在stm32上有问题!!!!!!!!!!!会卡死
    buf->check = KDCOM_Check(buf);
    return (uint8*)buf;
}

uint8* KDCOM_SetData(KDRobot* buf, uint8 channel, float data1, float data2)
{
    buf->head = channel;
    buf->buf[0] = *((uint8*)&data1);  //获取a的首地址
    buf->buf[1] = *((uint8*)&data1 + 1);//获取a的首地址的后一个字节地址
    buf->buf[2] = *((uint8*)&data1 + 2);//获取a的首地址的后两个字节地址
    buf->buf[3] = *((uint8*)&data1 + 3);//获取a的首地址的后三个字节地址

    buf->buf[4] = *((uint8*)&data2);  //获取a的首地址
    buf->buf[5] = *((uint8*)&data2 + 1);//获取a的首地址的后一个字节地址
    buf->buf[6] = *((uint8*)&data2 + 2);//获取a的首地址的后两个字节地址
    buf->buf[7] = *((uint8*)&data2 + 3);//获取a的首地址的后三个字节地址
    // *(float*)&buf->buf[0] = data1;
    // *(float*)&buf->buf[4] = data2;
    buf->check = KDCOM_Check(buf);
    return (uint8*)buf;
}
//0 finish
//1 error head
//2 check error
// uint8 KDCOM_Process(uint8* buf, uint8 *channel, float (*p)[4], float (*i)[4], float (*d)[4], float (*target)[4], float (*actual)[4], 
//                     uint8 *start, uint8 *stop, float *data1, float *data2)
uint8 KDCOM_Process(uint8* buf, uint8 *channel, float *p[4], float *i[4], float *d[4], float *target[4], float *actual[4], 
                    uint8 *start, uint8 *stop, float *data1, float *data2)
{
	KDRobot *kdrobot = (KDRobot*)buf;
    if(KDCOM_Check(kdrobot) == kdrobot->check)
    {
        if(buf[0] == 'D')
        {
            if(data1)
            {
                *data1 = KDCOM_GetData((KDRobot*)buf, 1);
            }
            if(data2)
            {
                *data2 = KDCOM_GetData((KDRobot*)buf, 2);
            }
        }
        else if(buf[0] == 'C')
        {
            switch (buf[1])
            {
            case 1:
                if(buf[2])//p
                {//write 1
                    *channel = KDCOM_GetCmdPara(kdrobot, 2);
                    *(p)[*channel] = KDCOM_GetCmdData(kdrobot);
                }
                else
                {
                    KDRobot kd;//read 0
                    *channel = KDCOM_GetCmdPara(kdrobot, 2);
                          // printf("2222 %d, %.2f, %d\r\n",*channel,*(p)[*channel],sizeof(KDRobot));
                          // uint8_t *bb = KDCOM_SetCmd(&kd, 1, 1, 0, 0, *(p)[*channel]);
                          // for (int i = 0; i < 10; i++)
                          // {
                          //   printf("3333 %02x\r\n", bb[i]);
                          // }
                          //  KDCOM_SendData(bb, sizeof(KDRobot));
                    KDCOM_SendData(KDCOM_SetCmd(&kd, 1, 1, 0, 0, *(p)[*channel]), sizeof(KDRobot));
                }
                break;
            case 2:
                if(buf[2])
                {
                    *channel = KDCOM_GetCmdPara(kdrobot, 2);
                    *(i)[*channel] = KDCOM_GetCmdData(kdrobot);
                }
                else
                {
                    KDRobot kd;//read 0
                    *channel = KDCOM_GetCmdPara(kdrobot, 2);
                    KDCOM_SendData(KDCOM_SetCmd(&kd, 2, 1, 0, 0, *(i)[*channel]), sizeof(KDRobot));
                }
                break;
            case 3:
                if(buf[2])
                {
                    *channel = KDCOM_GetCmdPara(kdrobot, 2);
                    *(d)[*channel] = KDCOM_GetCmdData(kdrobot);
                }
                else
                {
                    KDRobot kd;//read 0
                    *channel = KDCOM_GetCmdPara(kdrobot, 2);
                    KDCOM_SendData(KDCOM_SetCmd(&kd, 3, 1, 0, 0, *(d)[*channel]), sizeof(KDRobot));
                }
                break;
            case 4:
                if(buf[2])
                {
                    *channel = KDCOM_GetCmdPara(kdrobot, 2);
                    *(target)[*channel] = KDCOM_GetCmdData(kdrobot);
                }
                else
                {
                    KDRobot kd;//read 0
                    *channel = KDCOM_GetCmdPara(kdrobot, 2);
                    KDCOM_SendData(KDCOM_SetCmd(&kd, 4, 1, 0, 0, (float)*(target)[*channel]), sizeof(KDRobot));
                }
                break;
            case 6:
                if(!buf[2])
                {
                    KDRobot kd;//read 0
                    *channel = KDCOM_GetCmdPara(kdrobot, 2);
                    KDCOM_SendData(KDCOM_SetCmd(&kd, 6, 1, 0, 0, (float)*(actual)[*channel]), sizeof(KDRobot));
                }
                break;
            case 7:
                *start = 1;
                break;
            case 8:
                *stop = 1;
                break;
            case 5:
            default:
                break;
            }
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 2;
    }
    return 0;
}

void KDCOM_send(float actual1,float target1,float actual2,float target2,float actual3,float target3,float actual4,float target4)  
{
  printf("%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",actual1,target1,actual2,target2,actual3,target3,actual4,target4);
}

