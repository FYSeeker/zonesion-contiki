#include "stepMotor.h"

#define SteppingMotor_STEP         GPIO_Pin_0
#define SteppingMotor_DIR          GPIO_Pin_1
#define SteppingMotor_EN 		   GPIO_Pin_3

#define PIN_STEP_port              GPIOB  
#define PIN_DIR_port               GPIOB
#define PIN_EN_port 	           GPIOA
/*********************************************************************************************
* 全局变量
*********************************************************************************************/
unsigned long stepNum = 720;                                         //脉冲数
char motorRun = 0;
unsigned long pulseNum = 0;                                     //脉冲数

/*********************************************************************************************
*名称：step_bit_toggle
*功能：电机step信号线引脚反转
*参数：无
*返回：无
*********************************************************************************************/
void step_bit_toggle(unsigned int flag)
{
  if(flag)
    GPIO_ResetBits(PIN_STEP_port, SteppingMotor_STEP);
  else
    GPIO_SetBits(PIN_STEP_port, SteppingMotor_STEP);
}

/*********************************************************************************************
*名称：motor_forward
*功能：电机正转
*参数：无
*返回：无
*********************************************************************************************/
void motor_forward()
{
  GPIO_ResetBits(PIN_DIR_port, SteppingMotor_DIR);              //方向线低电平
  GPIO_SetBits(PIN_EN_port, SteppingMotor_EN);                  //使能线高电平
}

/*********************************************************************************************
*名称：motor_reverse
*功能：电机反转
*参数：无
*返回：无
*********************************************************************************************/
void motor_reverse()
{
  GPIO_SetBits(PIN_DIR_port,SteppingMotor_DIR);                           //方向线高电平
  GPIO_SetBits(PIN_EN_port, SteppingMotor_EN);                            //使能线高电平
}

/*********************************************************************************************
*名称：motor_stop
*功能：电机停止
*参数：无
*返回：无
*********************************************************************************************/
void motor_stop()
{
  GPIO_ResetBits(PIN_EN_port, SteppingMotor_EN);                          //使能线低电平
  GPIO_ResetBits(PIN_DIR_port, SteppingMotor_DIR);                         //方向线低电平
}

/*********************************************************************************************
*名称：motor_control
*功能：步进电机综合控制函数
*参数：Cmd
*返回：无
*********************************************************************************************/
void motor_control(unsigned char Cmd)
{
  if(Cmd & 0x01)                                                //若检测到步进电机开
  {
    if(Cmd & 0x02)                                              //若检测到指令为反转
    {
      motor_reverse();                                          //执行电机反转程序
      pulseNum = stepNum * 5 / 9 + 4;                                //获取步进电机输入脉冲
    }
    else
    {
      motor_forward();                                          //否则执行电机正转程序
      pulseNum = stepNum * 5 / 9 + 4;                                //获取步进电机输入脉冲
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
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	        //使能PA端口时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	        //使能PB端口时钟
  
  GPIO_InitStructure.GPIO_Pin = SteppingMotor_DIR | SteppingMotor_STEP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = SteppingMotor_EN;
  GPIO_Init(PIN_EN_port, &GPIO_InitStructure);
  GPIO_SetBits(PIN_EN_port, SteppingMotor_EN);
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
void stepMotor_init()
{
  SteppingMotorIOInit();
  TIM2IntInit(499,167);
}

