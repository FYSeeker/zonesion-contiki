#include "mlx90615.h"

//初始化红外体温传感器
int mlx90615_init()
{
    char id[2];
    IIC3_Init();
    mlx90615_smb();

    if (mlx90615_read(EE_ADDR(0x0e), id, 2) != 2)
    {
        printf("error: can't find ic mlx90615 \r\n");
        return -1;
    }
    else
    {
        printf("mlx90615 id : %02X%02X\r\n", id[0], id[1]);
        return 0;
    }
}

float mlx90615_t()
{
    char t[2];
    unsigned int value=0;
    float temp=0;
    
    if (mlx90615_read(RAM_ADDR(0x07), t, 2) == 2)
    {
        value = t[1]<<8 | t[0];
        temp = (value*0.02)-273.15;
        return temp;
    }
    return 0;
}

int mlx90615_read(char reg, char *buf, int len)
{
    int i;
    IIC3_Start();
    IIC3_Send_Byte(DEV_ADDR<<1 | 0);
    if (IIC3_Wait_Ack())
    {
        return -1;
    }
    IIC3_Send_Byte(reg);
    if (IIC3_Wait_Ack())
    {
        return -1;
    }
    IIC3_Start();
    IIC3_Send_Byte(DEV_ADDR<<1 | 1);
    if (IIC3_Wait_Ack())
    {
        return -1;
    }
    for (i=0; i<len-1; i++)
    {
        buf[i] = IIC3_Read_Byte(1);
    }
    buf[i] = IIC3_Read_Byte(0);
    IIC3_Stop();
    return len;
}