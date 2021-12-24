#ifndef _mlx90615_h_
#define _mlx90615_h_

#include "stm32f4xx.h"
#include "IIC3.h"
#include "extend.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "delay.h"

#define DEV_ADDR        0x5B
#define EE_ADDR(x)      ((0x01<<4)|((x)&0x0f))
#define RAM_ADDR(x)     ((0x02<<4)|((x)&0x0f))
#define mlx90615_smb()  do{ PCout(2) = 0; delay_ms(50);  PCout(2)=1;}while(0);

int mlx90615_init(void);
float mlx90615_t(void);
int mlx90615_read(char reg, char *buf, int len);

#endif