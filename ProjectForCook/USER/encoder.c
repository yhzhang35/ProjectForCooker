#include"encoder.h"
#include "bluetooth.h"
extern unsigned char fan_go_first;
extern unsigned short count;
//初始化编码器
void Encode_Encoder_Init() //PA0(TIM2 CH1),PA1(TIM2 CH2)
{
	 //编码器一的初始化
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		
	//使能时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//配置GPIO
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//配置时基
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);  //时基初始化
    TIM_TimeBaseStructure.TIM_Prescaler=0x0;  //配置分频
    TIM_TimeBaseStructure.TIM_Period=ENCODER_TIM_PERIOD; //自动重装载值
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //时钟分频，不用管
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  //向上计数
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);  

    TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//使用编码器模式3
		//输入结构体
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter=10;
    TIM_ICInit(TIM2,&TIM_ICInitStructure);
		
		//清楚中断标志位
    TIM_ClearFlag(TIM2,TIM_FLAG_Update);
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);

    TIM_SetCounter(TIM2,0);
    TIM_Cmd(TIM2,ENABLE);
		
		//配置中断
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
		
		NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		
		//编码器二的初始化********************************
		
		//使能时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
		//重映射时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 
	
		//配置GPIO
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOD,&GPIO_InitStructure);
		
		//配置重映射
		GPIO_PinRemapConfig(GPIO_Remap_TIM4,ENABLE);//改变指定管脚的映射	
	
		//配置时基
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);  //时基初始化
    TIM_TimeBaseStructure.TIM_Prescaler=0x0;  //配置分频
    TIM_TimeBaseStructure.TIM_Period=ENCODER_TIM_PERIOD; //自动重装载值
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //时钟分频，不用管
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  //向上计数
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);  

    TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//使用编码器模式3
		//输入结构体
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter=10;
    TIM_ICInit(TIM4,&TIM_ICInitStructure);
		
		//清楚中断标志位
    TIM_ClearFlag(TIM4,TIM_FLAG_Update);
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);

    TIM_SetCounter(TIM4,0);
    TIM_Cmd(TIM4,ENABLE);
		
		//配置中断
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
		
		NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
		NVIC_Init(&NVIC_InitStructure);
}


short Read_Encoder1()
{
    short Encoder_TIM;    
        Encoder_TIM= (short)TIM2 -> CNT;  
        //TIM2 -> CNT=0;    //这儿可能是出现抖动的原因
        return Encoder_TIM;
}

short Read_Encoder2()
{
    short Encoder_TIM;    
        Encoder_TIM= (short)TIM4 -> CNT;  
        //TIM4 -> CNT=0; 
        return Encoder_TIM;
}
//编码器一的服务函数
void TIM2_IRQHandler(void)
{  
		short n;
    if(TIM2->SR&0X0001)//溢出中断
    {    //通过此处验证，当向下溢出时，计数器被置为自动重装载寄存器的值
				n=Read_Encoder1();
				printf("编码器1结果为：%d\n",n);
				motor_stop(1);
//				if(n==0)
//					motor_stop(1);
//				else
//				{
//					if(fan_go_first==1)
//					{
//						motor_stop(1);
//						fan_go_first=0;
//					}
//					else
//						{
//							delay(1000000);
//							fan_go_first=1;
//						}
//				}
    }                  
    TIM2->SR&=~(1<<0);//清除中断标志位        
}

//编码器二的中断服务函数
void TIM4_IRQHandler(void)
{  
		short n;
    if(TIM4->SR&0X0001)//溢出中断
    {    //通过此处验证，当向下溢出时，计数器被置为自动重装载寄存器的值
				n=Read_Encoder2();
				printf("编码器2结果为：%d\n",n);
				motor_stop(2);
			
    }                  
    TIM4->SR&=~(1<<0);//清除中断标志位        
}
 
static void delay(unsigned long i)
{
	while(--i);
}
