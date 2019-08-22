#include"motor.h"
#include "system.h"
#include "bluetooth.h"
#include "stdio.h"
#include "encoder.h"


unsigned short count=0;

//in1--1,in2--0,ena--1 //takeup[0:499]
void motor_rotate_zheng(unsigned char motor_choose,unsigned int takeUp)
{
	//置相关位
	if(motor_choose==1)
	{
		GPIO_SetBits(MOTOR1_IN1_PORT,MOTOR1_IN1_PIN);
		GPIO_ResetBits(MOTOR1_IN2_PORT,MOTOR1_IN2_PIN);
		TIM_SetCompare1(TIM3,takeUp);
	}
	else if(motor_choose==2)
	{
		GPIO_SetBits(MOTOR2_IN1_PORT,MOTOR2_IN1_PIN);
		GPIO_ResetBits(MOTOR2_IN2_PORT,MOTOR2_IN2_PIN);
		TIM_SetCompare2(TIM3,takeUp);
	}
	else
	{
		//给出报警
	}
	
	
}

//in1--0,in2--1,ena--1
void motor_rotate_fan(unsigned char motor_choose,unsigned int takeUp)
{
	//置相关位
	if(motor_choose==1)
	{
		GPIO_SetBits(MOTOR1_IN2_PORT,MOTOR1_IN2_PIN);
		GPIO_ResetBits(MOTOR1_IN1_PORT,MOTOR1_IN1_PIN);
		TIM_SetCompare1(TIM3,takeUp);
	}
	else if(motor_choose==2)
	{
		GPIO_SetBits(MOTOR2_IN2_PORT,MOTOR2_IN2_PIN);
		GPIO_ResetBits(MOTOR2_IN1_PORT,MOTOR2_IN1_PIN);
		TIM_SetCompare2(TIM3,takeUp);
	}
	else
	{
		//给出报警
	}


}


void GPIO_MOTOR_INT(u16 per,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
		/* 开启时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  //这儿是使能复用功能吗
	                                                    //这儿是使能重映射时钟
	//初始化PWM引脚
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Pin=MOTOR1_PWM_PIN|MOTOR2_PWM_PIN;
	GPIO_Init(MOTOR1_PWM_PORT,&GPIO_InitStructure);
	
	//初始化in1引脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=MOTOR1_IN1_PIN|MOTOR2_IN1_PIN;
	GPIO_Init(MOTOR1_IN1_PORT,&GPIO_InitStructure);
	
		//初始化in2引脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=MOTOR1_IN2_PIN|MOTOR2_IN2_PIN;
	GPIO_Init(MOTOR1_IN2_PORT,&GPIO_InitStructure);
	
	
	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);//改变指定管脚的映射	
	
	TIM_TimeBaseInitStructure.TIM_Period=per;   //自动装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //分频系数
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //设置向上计数模式
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);	
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High ;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC1Init(TIM3,&TIM_OCInitStructure); //输出比较通道1初始化
	TIM_OC2Init(TIM3,&TIM_OCInitStructure); //输出比较通道2初始化
	
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable); //使能TIMx在 CCR1 上的预装载寄存器
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable); //使能TIMx在 CCR2 上的预装载寄存器
	TIM_ARRPreloadConfig(TIM3,ENABLE);//使能预装载寄存器
	
	TIM_Cmd(TIM3,ENABLE); //使能定时器
	TIM_SetCompare1(TIM3,0);
	TIM_SetCompare2(TIM3,0);
	
}
	
void motor_stop(unsigned char motor_choose)
{
		//置相关位
	if(motor_choose==1)
	{
		GPIO_ResetBits(MOTOR1_IN2_PORT,MOTOR1_IN2_PIN);
		GPIO_ResetBits(MOTOR1_IN1_PORT,MOTOR1_IN1_PIN);
	}
	else	if(motor_choose==2)
			  {
						GPIO_ResetBits(MOTOR2_IN2_PORT,MOTOR2_IN2_PIN);
						GPIO_ResetBits(MOTOR2_IN1_PORT,MOTOR2_IN1_PIN);
			  }
				else
				{
					//提示出错了
				}
	
}

void motor_flue(unsigned char motor_choose)
{
	//置相关位
	if(motor_choose==1)
	{
		GPIO_SetBits(MOTOR1_IN2_PORT,MOTOR1_IN2_PIN);
		GPIO_SetBits(MOTOR1_IN1_PORT,MOTOR1_IN1_PIN);
	}
	else	if(motor_choose==2)
				{
					GPIO_SetBits(MOTOR2_IN2_PORT,MOTOR2_IN2_PIN);
					GPIO_SetBits(MOTOR2_IN1_PORT,MOTOR2_IN1_PIN);
				}
				else
				{
					//提示出错了
				}
}
void delay_ms(unsigned int delaytime)
{}
	
	
//定义旋转接口  
/**
	
	@function:实现对电机的精确控制
	@para 电机选择，运动方向，挡位，角位移，定时器选择，定时长度
	@注：distance的精度为0.001
*/
void motor_revolve(unsigned char motor_choose,
	unsigned char direction,unsigned short speed_stage,
		float distance,unsigned timer_choose,unsigned int time)
		{
		
		if(time==0)
		{
			//设定电机旋转位移
			count=distance*2400;
		
			//对应到相应的编码器中去
			if(motor_choose==1)
			{
				TIM_SetAutoreload(TIM2,count);
				if(direction==DIRECTION_ZHENG)  //向上计数
					TIM_SetCounter(TIM2,0);
				else
				{
					TIM_SetCounter(TIM2,count-1);  //向下计数
					printf("初始设置的counter:%d\n",Read_Encoder1());
				}
			}
			else
				if(motor_choose==2)
				{
					TIM_SetAutoreload(TIM4,count);
					if(direction==DIRECTION_ZHENG)
						TIM_SetCounter(TIM4,0);
					else
						TIM_SetCounter(TIM4,count);
				}
				else
				{
					//提示输入参数错误
				
				}
		}
		else
		{
			if(motor_choose==1)
			//通过延时来停止电机旋转
			switch(timer_choose)
			{
				case 1:timer1(time*100,COMMEND_MOTOR_1);
				break;
				
				case 2:timer2(time*100,COMMEND_MOTOR_1);
				break;
				
				case 3:;
				break;
				
				//提示错误;
				default:;
					
				
			}
			if(motor_choose==2)
			{
				switch(timer_choose)
				{
					case 1:timer1(time*100,COMMEND_MOTOR_2);
					break;
				
					case 2:timer2(time*100,COMMEND_MOTOR_2);;
					break;
				
					case 3:;
					break;
				
					//提示错误;
					default:;
					}
				}
				else
				{
					//提示错误
				}
					
			}
		
			//设定方向和挡位
		if(direction==DIRECTION_ZHENG)
		{
			motor_rotate_zheng(motor_choose,speed_stage);
		}
		else
		{
			motor_rotate_fan(motor_choose,speed_stage);
		}
	
}
	
