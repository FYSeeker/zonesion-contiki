/*********************************************************************************************
* �ļ���hw.h
* ���ߣ�fuyou 
* ˵����plus�ڵ��Զ�ʶ��������������
* �޸ģ�
* ע�ͣ�
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
