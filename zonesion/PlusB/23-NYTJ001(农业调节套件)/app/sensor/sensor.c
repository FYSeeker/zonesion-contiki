/*********************************************************************************************
* 文件：sensor.c
* 作者：
* 说明：通用传感器控制接口程序
* 修改：
* 注释：
*********************************************************************************************/
#include "sensor.h"

unsigned char D0 = 0xe0;                                        // 主动上报使能，默认只允许A0主动上报
unsigned char D1 = 0;                                           // 默认关闭控制类传感器
unsigned int V0 = 5;                                           // 主动上报时间间隔，单位秒,0不主动上报
float A5 =0, A6=0, A7=0;                                        // 板载温度、湿度、电压

unsigned long V1 = 2*360;                                     //脉冲数
char motorRun=0;
unsigned long pulseNum = 0;                                     //脉冲数

/*********************************************************************************************
*名称：step_bit_toggle
*功能：电机step信号线引脚反转
*参数：无
*返回：无
*********************************************************************************************/
void step_bit_toggle(unsigned int  flag)
{
    if(flag)
    {
        GPIO_ResetBits(GPIOB,SteppingMotor_STEP);
    }
    else
    {
        GPIO_SetBits(GPIOB,SteppingMotor_STEP);
    }
}

/*********************************************************************************************
*名称：motor_forward
*功能：电机正转
*参数：无
*返回：无
*********************************************************************************************/
void motor_forward()
{
    GPIO_ResetBits(GPIOB,SteppingMotor_DIR);                         //方向线低电平
    GPIO_SetBits(GPIOB,SteppingMotor_EN);                            //使能线高电平
}

/*********************************************************************************************
*名称：motor_reverse
*功能：电机反转
*参数：无
*返回：无
*********************************************************************************************/
void motor_reverse()
{
    GPIO_SetBits(GPIOB,SteppingMotor_DIR);                           //方向线高电平
    GPIO_SetBits(GPIOB,SteppingMotor_EN);                            //使能线高电平
}

/*********************************************************************************************
*名称：motor_stop
*功能：电机停止
*参数：无
*返回：无
*********************************************************************************************/
void motor_stop()
{
    GPIO_ResetBits(GPIOB,SteppingMotor_EN);                          //使能线低电平
    GPIO_ResetBits(GPIOB,SteppingMotor_DIR);                         //方向线低电平
}

/*********************************************************************************************
*名称：motor_control
*功能：步进电机综合控制函数
*参数：Cmd
*返回：无
*********************************************************************************************/
void motor_control(unsigned char Cmd)
{
    if(Cmd & 0x04)                                                //若检测到步进电机开
    {
        if(Cmd & 0x08)                                              //若检测到指令为反转
        {
            motor_reverse();                                          //执行电机反转程序
            pulseNum = V1 * 5 / 9 + 4;                                //获取步进电机输入脉冲
        }
        else
        {
            motor_forward();                                          //否则执行电机正转程序
            pulseNum = V1 * 5 / 9 + 4;                                //获取步进电机输入脉冲
        }
        motorRun = 1;
        TIM_Cmd(TIM2, ENABLE);
    }
    else
    {
        motor_stop();                                               //否则关闭步进电机
        motorRun = 0;
        pulseNum = 0;                                               //脉冲数置零
    }
}
/*********************************************************************************************
* 名称：SteppingMotorIOInit
* 功能：步进电机IO口初始化
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void SteppingMotorIOInit()
{
    GPIO_InitTypeDef  GPIO_InitStructure;                         //初始化结构体
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);         //使能时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 |GPIO_Pin_9 |GPIO_Pin_10;//配置端口
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                 //配置为输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;                //配置为推挽模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;             //时钟为50MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;              //电平配置为上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);                        //使能端口
}

/*********************************************************************************************
* 名称：TIM2IntInit
* 功能：定时器2中断初始化
* 参数：arr:自动重重装值，psc:分频系数
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void TIM2IntInit(unsigned short arr,unsigned short psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;             //初始化定时器结构体
    NVIC_InitTypeDef NVIC_InitStructure;                        //初始化NVIC结构体

    //打开定时器2时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);        //开启定时器时钟
    //这个就是自动装载的计数值，由于计数是从0开始的，周期为20000us
    TIM_TimeBaseStructure.TIM_Period = arr;
    // 这个就是预分频系数，当由于为0时表示不分频所以要减1
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    // 高级应用本次不涉及。定义在定时器时钟(CK_INT)频率与数字滤波器(ETR,TIx)
    // 使用的采样频率之间的分频比例
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    //向上计数
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    //初始化定时器2
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    // 中断参数配置
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;             //开启通用定时器2中断控制线
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //配置响应优先级为0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          //配置抢占优先级为1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //开通用定时器2中断
    NVIC_Init(&NVIC_InitStructure);                             //使能中断
    
    //清除TIM2溢出中断标志
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    //打开溢出中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, DISABLE);
}

/*********************************************************************************************
* 名称：TIM2_IRQHandler
* 功能：定时器2中断服务函数
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void TIM2_IRQHandler(void)
{
    static char edge=0;
    char obuf[8] = {0};
    
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)            //若中断响应为真
    {
        if(pulseNum>0)
        {
            step_bit_toggle(edge);                                    //输出脉冲信号
            edge = ~edge;                                             //电平边沿反转
            if(!edge) pulseNum --;                                    //在低电平时脉冲数减一
        }
        else if(motorRun && !pulseNum)
        {
            D1 &= 0xF3;                                             //关闭步进电机
            sprintf(obuf, "{D1=%d}", D1);
            rfUartSendData(obuf);
            motorRun = 0;
            TIM_Cmd(TIM2, DISABLE);
        }
    }
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);                    //清空定时器中断标志位
}

/*********************************************************************************************
* 名称：SteppingMotorInit
* 功能：步进电机初始化
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void SteppingMotorInit()
{
    SteppingMotorIOInit();
    TIM2IntInit(499,167);
}

/*********************************************************************************************
* 名称：sensor_init()
* 功能：
* 参数：
* 返回：
* 修改：
* 注释：
*********************************************************************************************/
void sensor_init(void)
{
    relay_init();                                                 //初始化控制类传感器（继电器）
    SteppingMotorInit();
}

/*********************************************************************************************
* 名称：sensor_type()
* 功能：
* 参数：
* 返回：返回传感器类型，3字节字符串表示
* 修改：
* 注释：
*********************************************************************************************/
char *sensor_type(void)
{
    return SENSOR_TYPE;                                           //返回传感器类型
}

/*********************************************************************************************
* 名称：sensor_control()
* 功能：传感器控制
* 参数：cmd - 控制命令
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void sensor_control(unsigned char cmd)
{
    if(cmd & 0x01)
    {
        relay_on(1);
    }
    else
    {
        relay_off(1);
    }
    if(cmd & 0x02)
    {
        relay_on(2);
    }
    else
    {
        relay_off(2);
    }
    motor_control(cmd);
}

/*********************************************************************************************
* 名称：updateA5
* 功能：更新A5的值
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void updateA5(void)
{
    A5 = Htu21dTemperature_Get();                               // 更新A5 板载温度
}

/*********************************************************************************************
* 名称：updateA6
* 功能：更新A6的值
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void updateA6(void)
{
    A6 = Htu21dHumidity_Get();                                  // 更新A6 板载湿度
}

/*********************************************************************************************
* 名称：updateA7
* 功能：更新A7的值
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void updateA7(void)
{
    A7 = BatteryVotage_Get();                                   // 更新A7 电池电压
}

/*********************************************************************************************
* 名称：sensor_poll()
* 功能：轮询传感器，并主动上报传感器数据
* 参数：t: 调用次数
* 返回：
* 修改：
* 注释：此函数每秒钟调用1次，t为调用次数
*********************************************************************************************/
void sensor_poll(unsigned int t)
{
    char buf[16] = {0};
    
    updateA5();
    updateA6();
    updateA7();
    
    if (V0 != 0)
    {
        if (t % V0 == 0)
        {
            zxbeeBegin();
            if (D0 & 0x20)
            {
                sprintf(buf, "%.1f", A5);
                zxbeeAdd("A5", buf);
            }
            if (D0 & 0x40)
            {
                sprintf(buf, "%.1f", A6);
                zxbeeAdd("A6", buf);
            }
            if (D0 & 0x80)
            {
                sprintf(buf, "%.1f", A7);
                zxbeeAdd("A7", buf);
            }
            char *p = zxbeeEnd();
            if (p != NULL)
            {
                rfUartSendData(p);                              //发送无线数据
            }
        }
    }
}

/*********************************************************************************************
* 名称：sensor_check()
* 功能：周期性检查函数，可设定轮询时间
* 参数：无
* 返回：设置轮询的时间，单位ms,范围:1-65535
* 修改：
* 注释：此函数用于需要快速做出相应的传感器
*********************************************************************************************/
unsigned short sensor_check()
{
    return 100;                                                 //返回值决定下次调用时间，此处为100ms
}

/*********************************************************************************************
* 名称：z_process_command_call()
* 功能：处理上层应用发过来的指令
* 参数：ptag: 指令标识 D0，D1， A0 ...
*       pval: 指令值， “？”表示读取，
*       obuf: 指令处理结果存放地址
* 返回：>0指令处理结果返回数据长度，0：没有返回数据，<0：不支持指令。
* 修改：
* 注释：
*********************************************************************************************/
int z_process_command_call(char* ptag, char* pval, char* obuf)
{
    int ret = -1;
    if (memcmp(ptag, "D0", 2) == 0)
    {
        if (pval[0] == '?')
        {
            ret = sprintf(obuf, "D0=%d", D0);
        }
    }
    if (memcmp(ptag, "CD0", 3) == 0)
    {
        int v = atoi(pval);
        if (v > 0)
        {
            D0 &= ~v;
        }
    }
    if (memcmp(ptag, "OD0", 3) == 0)
    {
        int v = atoi(pval);
        if (v > 0)
        {
            D0 |= v;
        }
    }
    if (memcmp(ptag, "D1", 2) == 0)
    {
        if (pval[0] == '?')
        {
            ret = sprintf(obuf, "D1=%d", D1);
        }
    }
    if (memcmp(ptag, "CD1", 3) == 0)  				            //若检测到CD1指令
    {
        int v = atoi(pval);                                     //获取CD1数据
        D1 &= ~v;                                               //更新D1数据
        sensor_control(D1);                                     //更新电磁阀直流电机状态
    }
    if (memcmp(ptag, "OD1", 3) == 0)  				            //若检测到OD1指令
    {
        int v = atoi(pval);                                     //获取OD1数据
        D1 |= v;                                                //更新D1数据
        sensor_control(D1);                                     //更新电磁阀直流电机状态
    }
    if (memcmp(ptag, "V0", 2) == 0)
    {
        if (pval[0] == '?')
        {
            ret = sprintf(obuf, "V0=%d", V0);
        }
        else
        {
            V0 = atoi(pval);
        }
    }
    if (memcmp(ptag, "A5", 2) == 0)
    {
        if (pval[0] == '?')
        {
            ret = sprintf(obuf, "A5=%.1f", A5);
        }
    }
    if (memcmp(ptag, "A6", 2) == 0)
    {
        if (pval[0] == '?')
        {
            ret = sprintf(obuf, "A6=%.1f", A6);
        }
    }
    if (memcmp(ptag, "A7", 2) == 0)
    {
        if (pval[0] == '?')
        {
            ret = sprintf(obuf, "A7=%.1f", A7);
        }
    }
    return ret;
}

