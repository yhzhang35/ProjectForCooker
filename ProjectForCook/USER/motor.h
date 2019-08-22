#ifndef __MOTOR_H_
#define __MOTOR_H_

#include"stm32f10x.h"

//挡位的定义
#define SPEED_STAGE_1 100
#define SPEED_STAGE_2 200
#define SPEED_STAGE_3 300
#define SPEED_STAGE_4 400
#define SPEED_STAGE_5 499

//方向的定义
#define DIRECTION_ZHENG 1
#define DIRECTION_FAN   0

//电机一的宏定义
#define MOTOR1_IN1_PORT GPIOC
#define MOTOR1_IN1_PIN GPIO_Pin_0
#define MOTOR1_IN1_CLK  RCC_APB2Periph_GPIOC

#define MOTOR1_IN2_PORT GPIOC
#define MOTOR1_IN2_PIN 	GPIO_Pin_1
#define MOTOR1_IN2_CLK  RCC_APB2Periph_GPIOC

#define MOTOR1_PWM_PORT GPIOC
#define MOTOR1_PWM_PIN 	GPIO_Pin_6
#define MOTOR1_PWM_CLK  RCC_APB2Periph_GPIOC


//电机二的宏定义
//电机一的宏定义
#define MOTOR2_IN1_PORT GPIOC
#define MOTOR2_IN1_PIN GPIO_Pin_2
#define MOTOR2_IN1_CLK  RCC_APB2Periph_GPIOC

#define MOTOR2_IN2_PORT GPIOC
#define MOTOR2_IN2_PIN 	GPIO_Pin_3
#define MOTOR2_IN2_CLK  RCC_APB2Periph_GPIOC

#define MOTOR2_PWM_PORT GPIOC
#define MOTOR2_PWM_PIN 	GPIO_Pin_7
#define MOTOR2_PWM_CLK  RCC_APB2Periph_GPIOC



void motor_rotate_fan(unsigned char motor_choose,unsigned int takeUp);
void motor_rotate_zheng(unsigned char motor_choose,unsigned int takeUp);
void GPIO_MOTOR_INT(u16 per,u16 psc);
void delay();
void motor_stop(unsigned char motor_choose);
void motor_flue(unsigned char motor_choose);
void delay_ms(unsigned int delaytime);
//***************************************
void motor_revolve(unsigned char motor_choose,
	unsigned char direction,unsigned short speed_stage,
		float distance,unsigned timer_choose,unsigned int time);
#endif