/*********************************************************************************************
* �ļ���sensor.c
* ���ߣ�
* ˵����ͨ�ô��������ƽӿڳ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
#include "sensor.h"

unsigned char D0 = 0xe0;                                        // �����ϱ�ʹ�ܣ�Ĭ��ֻ����A0�����ϱ�
unsigned char D1 = 0;                                           // Ĭ�Ϲرտ����ഫ����
unsigned int V0 = 5;                                           // �����ϱ�ʱ��������λ��,0�������ϱ�
float A5 =0, A6=0, A7=0;                                        // �����¶ȡ�ʪ�ȡ���ѹ

unsigned long V1 = 2*360;                                     //������
char motorRun=0;
unsigned long pulseNum = 0;                                     //������

/*********************************************************************************************
*���ƣ�step_bit_toggle
*���ܣ����step�ź������ŷ�ת
*��������
*���أ���
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
*���ƣ�motor_forward
*���ܣ������ת
*��������
*���أ���
*********************************************************************************************/
void motor_forward()
{
    GPIO_ResetBits(GPIOB,SteppingMotor_DIR);                         //�����ߵ͵�ƽ
    GPIO_SetBits(GPIOB,SteppingMotor_EN);                            //ʹ���߸ߵ�ƽ
}

/*********************************************************************************************
*���ƣ�motor_reverse
*���ܣ������ת
*��������
*���أ���
*********************************************************************************************/
void motor_reverse()
{
    GPIO_SetBits(GPIOB,SteppingMotor_DIR);                           //�����߸ߵ�ƽ
    GPIO_SetBits(GPIOB,SteppingMotor_EN);                            //ʹ���߸ߵ�ƽ
}

/*********************************************************************************************
*���ƣ�motor_stop
*���ܣ����ֹͣ
*��������
*���أ���
*********************************************************************************************/
void motor_stop()
{
    GPIO_ResetBits(GPIOB,SteppingMotor_EN);                          //ʹ���ߵ͵�ƽ
    GPIO_ResetBits(GPIOB,SteppingMotor_DIR);                         //�����ߵ͵�ƽ
}

/*********************************************************************************************
*���ƣ�motor_control
*���ܣ���������ۺϿ��ƺ���
*������Cmd
*���أ���
*********************************************************************************************/
void motor_control(unsigned char Cmd)
{
    if(Cmd & 0x04)                                                //����⵽���������
    {
        if(Cmd & 0x08)                                              //����⵽ָ��Ϊ��ת
        {
            motor_reverse();                                          //ִ�е����ת����
            pulseNum = V1 * 5 / 9 + 4;                                //��ȡ���������������
        }
        else
        {
            motor_forward();                                          //����ִ�е����ת����
            pulseNum = V1 * 5 / 9 + 4;                                //��ȡ���������������
        }
        motorRun = 1;
        TIM_Cmd(TIM2, ENABLE);
    }
    else
    {
        motor_stop();                                               //����رղ������
        motorRun = 0;
        pulseNum = 0;                                               //����������
    }
}
/*********************************************************************************************
* ���ƣ�SteppingMotorIOInit
* ���ܣ��������IO�ڳ�ʼ��
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void SteppingMotorIOInit()
{
    GPIO_InitTypeDef  GPIO_InitStructure;                         //��ʼ���ṹ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);         //ʹ��ʱ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 |GPIO_Pin_9 |GPIO_Pin_10;//���ö˿�
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                 //����Ϊ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;                //����Ϊ����ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;             //ʱ��Ϊ50MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;              //��ƽ����Ϊ����
    GPIO_Init(GPIOB, &GPIO_InitStructure);                        //ʹ�ܶ˿�
}

/*********************************************************************************************
* ���ƣ�TIM2IntInit
* ���ܣ���ʱ��2�жϳ�ʼ��
* ������arr:�Զ�����װֵ��psc:��Ƶϵ��
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void TIM2IntInit(unsigned short arr,unsigned short psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;             //��ʼ����ʱ���ṹ��
    NVIC_InitTypeDef NVIC_InitStructure;                        //��ʼ��NVIC�ṹ��

    //�򿪶�ʱ��2ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);        //������ʱ��ʱ��
    //��������Զ�װ�صļ���ֵ�����ڼ����Ǵ�0��ʼ�ģ�����Ϊ20000us
    TIM_TimeBaseStructure.TIM_Period = arr;
    // �������Ԥ��Ƶϵ����������Ϊ0ʱ��ʾ����Ƶ����Ҫ��1
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    // �߼�Ӧ�ñ��β��漰�������ڶ�ʱ��ʱ��(CK_INT)Ƶ���������˲���(ETR,TIx)
    // ʹ�õĲ���Ƶ��֮��ķ�Ƶ����
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    //���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    //��ʼ����ʱ��2
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    // �жϲ�������
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;             //����ͨ�ö�ʱ��2�жϿ�����
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //������Ӧ���ȼ�Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          //������ռ���ȼ�Ϊ1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //��ͨ�ö�ʱ��2�ж�
    NVIC_Init(&NVIC_InitStructure);                             //ʹ���ж�
    
    //���TIM2����жϱ�־
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    //������ж�
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, DISABLE);
}

/*********************************************************************************************
* ���ƣ�TIM2_IRQHandler
* ���ܣ���ʱ��2�жϷ�����
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void TIM2_IRQHandler(void)
{
    static char edge=0;
    char obuf[8] = {0};
    
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)            //���ж���ӦΪ��
    {
        if(pulseNum>0)
        {
            step_bit_toggle(edge);                                    //��������ź�
            edge = ~edge;                                             //��ƽ���ط�ת
            if(!edge) pulseNum --;                                    //�ڵ͵�ƽʱ��������һ
        }
        else if(motorRun && !pulseNum)
        {
            D1 &= 0xF3;                                             //�رղ������
            sprintf(obuf, "{D1=%d}", D1);
            rfUartSendData(obuf);
            motorRun = 0;
            TIM_Cmd(TIM2, DISABLE);
        }
    }
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);                    //��ն�ʱ���жϱ�־λ
}

/*********************************************************************************************
* ���ƣ�SteppingMotorInit
* ���ܣ����������ʼ��
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void SteppingMotorInit()
{
    SteppingMotorIOInit();
    TIM2IntInit(499,167);
}

/*********************************************************************************************
* ���ƣ�sensor_init()
* ���ܣ�
* ������
* ���أ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void sensor_init(void)
{
    relay_init();                                                 //��ʼ�������ഫ�������̵�����
    SteppingMotorInit();
}

/*********************************************************************************************
* ���ƣ�sensor_type()
* ���ܣ�
* ������
* ���أ����ش��������ͣ�3�ֽ��ַ�����ʾ
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
char *sensor_type(void)
{
    return SENSOR_TYPE;                                           //���ش���������
}

/*********************************************************************************************
* ���ƣ�sensor_control()
* ���ܣ�����������
* ������cmd - ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
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
* ���ƣ�updateA5
* ���ܣ�����A5��ֵ
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void updateA5(void)
{
    A5 = Htu21dTemperature_Get();                               // ����A5 �����¶�
}

/*********************************************************************************************
* ���ƣ�updateA6
* ���ܣ�����A6��ֵ
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void updateA6(void)
{
    A6 = Htu21dHumidity_Get();                                  // ����A6 ����ʪ��
}

/*********************************************************************************************
* ���ƣ�updateA7
* ���ܣ�����A7��ֵ
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void updateA7(void)
{
    A7 = BatteryVotage_Get();                                   // ����A7 ��ص�ѹ
}

/*********************************************************************************************
* ���ƣ�sensor_poll()
* ���ܣ���ѯ���������������ϱ�����������
* ������t: ���ô���
* ���أ�
* �޸ģ�
* ע�ͣ��˺���ÿ���ӵ���1�Σ�tΪ���ô���
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
                rfUartSendData(p);                              //������������
            }
        }
    }
}

/*********************************************************************************************
* ���ƣ�sensor_check()
* ���ܣ������Լ�麯�������趨��ѯʱ��
* ��������
* ���أ�������ѯ��ʱ�䣬��λms,��Χ:1-65535
* �޸ģ�
* ע�ͣ��˺���������Ҫ����������Ӧ�Ĵ�����
*********************************************************************************************/
unsigned short sensor_check()
{
    return 100;                                                 //����ֵ�����´ε���ʱ�䣬�˴�Ϊ100ms
}

/*********************************************************************************************
* ���ƣ�z_process_command_call()
* ���ܣ������ϲ�Ӧ�÷�������ָ��
* ������ptag: ָ���ʶ D0��D1�� A0 ...
*       pval: ָ��ֵ�� ��������ʾ��ȡ��
*       obuf: ָ�������ŵ�ַ
* ���أ�>0ָ������������ݳ��ȣ�0��û�з������ݣ�<0����֧��ָ�
* �޸ģ�
* ע�ͣ�
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
    if (memcmp(ptag, "CD1", 3) == 0)  				            //����⵽CD1ָ��
    {
        int v = atoi(pval);                                     //��ȡCD1����
        D1 &= ~v;                                               //����D1����
        sensor_control(D1);                                     //���µ�ŷ�ֱ�����״̬
    }
    if (memcmp(ptag, "OD1", 3) == 0)  				            //����⵽OD1ָ��
    {
        int v = atoi(pval);                                     //��ȡOD1����
        D1 |= v;                                                //����D1����
        sensor_control(D1);                                     //���µ�ŷ�ֱ�����״̬
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

