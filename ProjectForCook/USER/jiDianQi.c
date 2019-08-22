#include "jiDianQi.h"
#include "system.h"

void jiDianQi_Init()
{
	//定义初始化结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	//配置GPIO
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//拉低平
	GPIO_ResetBits(GPIOB,GPIO_Pin_All);
}

//n可取JiDianQi_x  
//status可取ENBALE_JI or DISABLE_JI
void turn_Ji(unsigned short n,unsigned char status,
	unsigned char timer_choose,unsigned char time)
{
	//置相应位
	if(status==ENABLE_JI)
	{
		if(timer_choose==1)
		{
			//延时相应时间
			timer1(time*100,4);
		
			GPIO_SetBits(JiDianQi_Port,n);
		}
		if(timer_choose==2)
		{
			//延时相应时间
			timer2(time*100,4);
		
			GPIO_SetBits(JiDianQi_Port,n);
		}
		else
		{
			//提示错误
		}
	}
	else
	{
		GPIO_ResetBits(JiDianQi_Port,n);
	}
	
	
}
