/*********************************************************************************************
* 文件：stadiometry.h
* 作者：Lixm 2017.10.17
* 说明：红外测距传感器驱动代码头文件  
* 修改：
* 注释：
*********************************************************************************************/

/*********************************************************************************************
* 宏条件编译
*********************************************************************************************/
#ifndef __STADIOMETRY_H__
#define __STADIOMETRY_H__

/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include "stm32f4xx.h"

/*********************************************************************************************
* 内部原型函数
*********************************************************************************************/
void stadiometry_init(void);                                    //红外测距传感器初始化
float get_stadiometry_data(void);

#endif /*__STADIOMETRY_H__*/

