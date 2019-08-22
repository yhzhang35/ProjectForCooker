#include"system.h"
#include"integer.h"
#include"bluetooth.h"
#include "cooker.h"
#include "motor.h"
#include "jiDianQi.h"
extern BYTE ReadBuffer[200];
extern UINT fnum;

/*****************************************************

function:实现对整个项目系统函数的实现，比如：system_init,定时等


*****************************************************/

//对定时器全局变量的定义
static unsigned int timer1_count=0;
static unsigned int timer1_n=0;
static unsigned char timer1_commend=0;
static unsigned char timer1_flag=0;

static unsigned int timer2_count=0;
static unsigned int timer2_n=0;
static unsigned char timer2_commend=0;
static unsigned char timer2_flag=0;

//系统初始化函数
void init_System()
{}
	
//执行相应函数
	void execute()
	{
		
		unsigned  char i=0,j=0;
		unsigned char num;
		unsigned short parameters[10],temp=0;  //定义参数数组
		
		printf("执行炒菜的各步骤\n");
		//循环查找F
		printf("%d,%s",fnum,ReadBuffer);
		while((i<=fnum)&&(ReadBuffer[i]!='E'))
		{
			while((ReadBuffer[i]!='F')&&(ReadBuffer[i]!='E')) ++i;
			//读取函数编号
				num=0;
				num=(ReadBuffer[++i]-'0')*10;
				num+=(ReadBuffer[++i]-'0');
			
			
			printf("找到的函数编号为%d \n",num);
			
			//指向‘(’
			i++;
			
			//读取参数
			j=0;
			while(ReadBuffer[i]!=')'&&(ReadBuffer[i]!='E'))
			{
				i++; //指向参数
				while((ReadBuffer[i]!=',')&&(ReadBuffer[i]!=')'))
				{
					temp*=10;
					temp+=(ReadBuffer[i++]-'0');
		
				}
				parameters[j++]=temp; //保存数据
				temp=0; //清零
			}
			//跳过‘）’
			if(ReadBuffer[i]==')')
				i++;
			
			switch(num)
			{
				//调用热锅函数 void induction_cooker_contro(float temperature,
											//unsigned char time,unsigned char start_degree)
				case 1: printf("输入的函数编号为1，参数别为%d,%d,%d\n",
												parameters[0],parameters[1],parameters[2]);
								induction_cooker_contro(parameters[0],
																				parameters[1],parameters[2],parameters[3]);
				break;
				
				//锅和搅拌轴转动 void motor_revolve(unsigned char motor_n,unsigned char direction,
                       //unsigned char speed_stage,float distance)
				case 2: printf("输入的函数编号为2，参数别为%d,%d,%d,%d\n",
					            parameters[0],parameters[1],parameters[2],parameters[3]);
								motor_revolve(parameters[0],parameters[1],
				        parameters[2],parameters[3],parameters[4],parameters[5]);
				break;
				
				//翻菜盒转动  void motor_contro(unsigned char motor_n,unsigned char direction,
         //可能这儿要和上面合并            //  unsigned char speed_stage,float distance)
				case 3: printf("输入的函数编号为3，参数别为,%d,%d,%d,%d\n",
					parameters[0],parameters[1],parameters[2],parameters[3]);  
								//motor_revolve(parameters[0],parameters[1],parameters[2],parameters[3]);
				break;
				
				//继电器 void setJiDianQi(unsigned char Fa_Name,unsigned char statement,unsigned int time)
				case 4:  printf("输入的函数编号为4，参数别为,%d,%d,%d\n",
												parameters[0],parameters[1],parameters[2]); 
								 turn_Ji(parameters[0],parameters[1],parameters[2],parameters[3]);
				break;
				default:printf("输入错误\n");
			}
				
		}
	}
	
	
	
	/********************************************************/
//用定时器实现延时 单位为毫秒
void timer1(unsigned long time,unsigned char commend)
{	
		while(timer1_flag!=0);
		//置标志位
		timer1_flag=1;
	
		//再使用
		TIM_Cmd(TIM6,ENABLE); //使能定时器	
		//配置命令
		timer1_count=time;
		timer1_commend=commend;
		
}

//用定时器实现延时
void timer2(unsigned long time,unsigned char commend)
{	
		while(timer2_flag!=0);
		//置标志位
		timer2_flag=1;
	
		//再使用
		TIM_Cmd(TIM7,ENABLE); //使能定时器	
		//配置命令
		timer2_count=time;
		timer2_commend=commend;
		
}


//每个计时器初始化
void timers_init()
{
	//定时器一(timer6)的初始化
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);//使能TIM4时钟
	
	TIM_TimeBaseInitStructure.TIM_Period=(1000-1);   //自动装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=71; //分频系数
	//TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	//TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //设置向上计数模式
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE); //开启定时器中断
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
	
	//中断分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;//定时器中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	
	
	//定时器二(timer7)的初始化
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);//使能TIM4时钟
	
	TIM_TimeBaseInitStructure.TIM_Period=(1000-1);   //自动装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=71; //分频系数
	//TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	//TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //设置向上计数模式
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE); //开启定时器中断
	TIM_ClearITPendingBit(TIM7,TIM_IT_Update);
	
	//中断分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;//定时器中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	
	
	
}



//定时器一的中断服务函数
void TIM6_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6,TIM_IT_Update))
	{
		timer1_n++;
		if(timer1_n==timer1_count)  //到时间
		{
			switch(timer1_commend)
			{
				case 1: 
					TIM_SetCompare3(TIM3,0);  //关闭电磁炉加热
				break;
				
				case 3:
					motor_stop(1);                //停止搅拌轴
					break;
				
				case 2:
					motor_stop(2);                  //停止锅转动
					break;
				case 4:turn_Ji(JiDianQi_1,DISABLE_JI,1,0);//关闭继电器1
					break;
				case 5:turn_Ji(JiDianQi_2,DISABLE_JI,1,0);
					break;
				case 6:turn_Ji(JiDianQi_3,DISABLE_JI,1,0);
					break;
			}
			//清零
			TIM_Cmd(TIM6,DISABLE);  //关闭定时器
			timer1_commend=0;
			timer1_n=0;
			timer1_count=0;
			timer1_flag=0;
		}
	}
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);	
}

void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7,TIM_IT_Update))
	{
		timer2_n++;
		if(timer2_n==timer2_count)  //到时间
		{
			switch(timer2_commend)
			{
				case 1: 
					TIM_SetCompare3(TIM3,0);  //关闭电磁炉加热
				break;
				
				case 3:motor_stop(1);;                //停止搅拌轴
					break;
				
				case 2:motor_stop(2);;                  //停止锅转动
					break;
				case 4:turn_Ji(JiDianQi_1,DISABLE_JI,1,0);                    //关闭继电器
					break;
				case 5:turn_Ji(JiDianQi_2,DISABLE_JI,1,0);
					break;
				case 6:turn_Ji(JiDianQi_3,DISABLE_JI,1,0);
					break;
			}
			//清零
			TIM_Cmd(TIM7,DISABLE);  //关闭定时器
			timer2_commend=0;
			timer2_n=0;
			timer2_count=0;
			timer2_flag=0;
		}
	}
	TIM_ClearITPendingBit(TIM7,TIM_IT_Update);	
}




