#ifndef __SYSTEM_H_
#define __SYSTEM_H_

#define COMMEND_COOKER 0
#define COMMEND_MOTOR_1 3
#define COMMEND_MOTOR_2 2


//炒菜执行相应函数
	void execute();
	
//系统初始化函数
void init_System();
void timer1(unsigned long time,unsigned char commend);
void timer2(unsigned long time,unsigned char commend);
void timers_init();

#endif