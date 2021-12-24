/*********************************************************************************************
* 文件：hw.h
* 作者：fuyou 
* 说明：plus节点自动识别无线类型驱动
* 修改：
* 注释：
*********************************************************************************************/
#ifndef __HW_H
#define __HW_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "contiki.h"


PROCESS_NAME(getHwType_process);

char hwType_Get(void);
void hwType_Set(char type);

#endif
