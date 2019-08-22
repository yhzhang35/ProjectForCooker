#ifndef __ENCODER_H_
#define __ENCODER_H_
#include"stm32f10x.h"
#include"led.h"
#include"motor.h"

#define ENCODER_TIM_PERIOD (0x0258<<2)

//³õÊ¼»¯±àÂëÆ÷
void Encode_Encoder_Init(); //PA0(TIM2 CH1),PA1(TIM2 CH2)
short Read_Encoder1();
short Read_Encoder2();
void TIM2_IRQHandler(void);

#endif