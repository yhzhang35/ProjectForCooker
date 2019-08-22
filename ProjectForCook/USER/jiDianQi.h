#ifndef __JIDIANQI_H_
#define __JIDIANQI_H_
#include "stm32f10x.h"

/***********继电器实现模块*************/

#define JiDianQi_Port GPIOB
#define JiDianQi_1    GPIO_Pin_0
#define JiDianQi_2    GPIO_Pin_1
#define JiDianQi_3    GPIO_Pin_2
#define JiDianQi_4    GPIO_Pin_3
#define JiDianQi_5    GPIO_Pin_4
#define JiDianQi_6    GPIO_Pin_5
#define JiDianQi_7    GPIO_Pin_6
#define JiDianQi_8    GPIO_Pin_7
#define JiDianQi_9    GPIO_Pin_8

#define ENABLE_JI 1
#define DISABLE_JI 0




void jiDianQi_Init();
void turn_Ji(unsigned short n,unsigned char status,
	unsigned char timer_choose,unsigned char time);


#endif