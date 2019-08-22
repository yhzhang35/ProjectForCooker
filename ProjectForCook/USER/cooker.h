#ifndef __COOKER_H_
#define __COOKER_H_
#include "stm32f10x.h"

#define 	MAX6675_CS	 		GPIO_Pin_4
#define 	MAX6675_CSL()		GPIO_ResetBits(GPIOA,MAX6675_CS);
#define 	MAX6675_CSH()		GPIO_SetBits(GPIOA,MAX6675_CS);

void Cooker_Init(u16 per,u16 psc);
void induction_cooker_contro(float temperature,unsigned char timer_choose,
	unsigned int time,unsigned short start_degree);
void MAX6675_init();
unsigned char MAX6675_ReadByte(void);
unsigned int Get_temperature();

#endif __COOKER_H_