#include "led.h"
#include "pwm.h"
#include "stm32f10x.h"
#include "encoder.h"
#include "bluetooth.h"
#include "spi_fat_flash.h"
#include "ff.h"
#include "stdio.h"
#include "system.h"
#include "can.h"
#include "cooker.h"
#include "jiDianQi.h"


//main文件中的函数申明
void file_System_Init();
void anolyses_Commend();


//定义反向溢出开始计数标志标志
unsigned char fan_go_first=0;

//定义蓝牙所需要的变量
unsigned char arry[8]={0};
unsigned char arry_i=0;
unsigned char check_flag=0;
unsigned char save_flag=0;
unsigned char no_or=0;
unsigned char dat[100];
unsigned char finish_flag=0;
unsigned char made_flag=0;

//定义文件系统所需要的全局变量
FILINFO file_info;
FATFS fs;													/* FatFs文件系统对象 */
FIL fnew;													/* 文件对象 */
FRESULT res_flash;                /* 文件操作结果 */
UINT fnum;            					  /* 文件成功读写数量 */
BYTE ReadBuffer[200]={0};        /* 读缓冲区 */
//BYTE WriteBuffer[] =              /* 写缓冲区*/
//"欢迎使用秉火STM32开发板 xie 今天是个好日子，新建文件系统测试文件\r\n"; 

int main(void)
{
	//定义变量
	unsigned char res;
	unsigned char tbuf[8],char_buf[8];
	unsigned char rbuf[8];
	unsigned char j=0;
	
	//定义温度值
	unsigned char temper=0;
	
	/* 初始化LED */
	//LED_Init();	
	
	//电磁炉初始化
	Cooker_Init(500,72-1);
	
	//电机初始化
	GPIO_MOTOR_INT(500,72-1);
	
	/* 初始化调试串口，一般为串口1 */
	USART1_Bluetooth_Init(115200);	
	
	//调用文件系统初始化
	file_System_Init();
	
	//can初始化
	CAN_Mode_Init(CAN_SJW_2tq,CAN_BS2_3tq,
	CAN_BS1_5tq,4,CAN_Mode_Normal);//波特率为1M
	
	//初始化继电器
	jiDianQi_Init();
	
	//编码器初始化
	Encode_Encoder_Init();
	
	//系统定时器初始化
	timers_init();
	
	//SPI_MAX6675初始化
	MAX6675_init();
	
	/******************************************************/
	#if 0
	//测试温度
	while(1)
	{
	
		temper=Get_temperature();
		printf("转换为整形，结果为：%d\n",temper);
	}
	
	#endif
	
	//////////////////////////////////////////////////////////
	/**********************************************************/
	//以下为测试代码
	#if 1
	//蓝牙和文件系统几乎调试成功，只是数据传输长了，会出现错误
	
	//测试继电器 //完成继电器测试
	turn_Ji(JiDianQi_1,ENABLE_JI,1,100);
	//测试电磁炉  //完成挡位的测试  时间保证为0.1秒
	induction_cooker_contro(200,1,200,110);
	//测试电机一  //完成挡位的测试
	motor_revolve(1,0,250,2,1,100);  //电机一，方向，档位，圈，定时器1，时间40s
	//测试电机二
	
	while(1);
	#endif
	
	#if 0
	/*************************回锅肉**********************/
	//1.热锅 180c 20s
	induction_cooker_contro(200,1,20,110);  //200度，timer1,2s,挡位(0-400)
  //2. 加油 料盒一
	turn_Ji(JiDianQi_1,ENABLE_JI,1,30);  //继电器选择，状态，timer,时间
	induction_cooker_contro(200,2,60,110);
	//3.加肉（快速） 料盒二
	turn_Ji(JiDianQi_2,ENABLE_JI,2,20);
	//induction_cooker_contro(200,2,100,110);
	//4.正反中炒 220c 50s
	induction_cooker_contro(200,2,500,110);
	
	motor_revolve(1,1,400,0,1,500);    //电机，方向，档位，圈数，timer，50s
	
	//5.加青椒（料盒三）快速
	turn_Ji(JiDianQi_3,ENABLE_JI,1,100);
	//6.正反快炒 220c 70s
	induction_cooker_contro(200,1,700,110);
	motor_revolve(1,0,400,0,2,700);  //电机，方向，档位，圈数，timer，70s
	//7.end
	while(1);
	
	#endif
	////////////////////////////////////////////////////////////
	/**********************************************/
	# if 0
	//以下为实际代码
	while(1)
	{
		file_System_Init();//挂载spi-flash
		
		/*等待命令并处理，如果是写命令者向文件中写入，如果是读命令
		将想要的数据读出*/
		anolyses_Commend();
		
		//通过CAN与从机通信，选择炒菜机
		for(j=0;j<8;j++)
			{
				tbuf[j]=j;
				char_buf[j]=tbuf[j]+0x30;
			}
			res=CAN_Send_Msg(tbuf,8);
			if(res)
			{
				printf("Send Failed!\r\n");
			}
			else
			{
				printf("发送数据：%s\r\n",char_buf);
			}
		res=CAN_Receive_Msg(rbuf);
		if(res)
		{
			for(j=0;j<res;j++)
			{
				char_buf[j]=rbuf[j]+0x30;
			}
			printf("接收数据：%s\r\n",char_buf);
		}
		
		//通过分析can总线获得的数据，选择炒菜机假如选择本机
		
		
		//如果查找到炒菜的菜谱
		if(made_flag==1)
		{
			//处理数据
			printf("正在做菜\n");
			execute();
			made_flag=0;  //这个标志应该放在can通信后置位，现在是放在读数据后面
		}
			
	}
	#endif
	
}
	
/*********************************************************************
 文件系统的初始化函数
********************************************************************/
void file_System_Init()
{
	  printf("****** 这是一个SPI FLASH 文件系统实验 ******\r\n");
  
	//在外部SPI Flash挂载文件系统，文件系统挂载时会对SPI设备初始化
	//f_mount()->find_volume()->disk_initialize->SPI_FLASH_Init()
	res_flash = f_mount(&fs,"1:",1);
	
/*----------------------- 格式化测试 -----------------*/  
	/* 如果没有文件系统就格式化创建创建文件系统 */
	if(res_flash == FR_NO_FILESYSTEM)
	{
		printf("》FLASH还没有文件系统，即将进行格式化...\r\n");
    /* 格式化 */
		res_flash=f_mkfs("1:",0,0);							
		
		if(res_flash == FR_OK)
		{
			printf("》FLASH已成功格式化文件系统。\r\n");
      /* 格式化后，先取消挂载 */
			res_flash = f_mount(NULL,"1:",1);			
      /* 重新挂载	*/			
			res_flash = f_mount(&fs,"1:",1);
		}
		else
		{
			//LED_RED;
			printf("《《格式化失败。》》\r\n");
			while(1);
		}
	}
  else if(res_flash!=FR_OK)
  {
    printf("！！外部Flash挂载文件系统失败。(%d)\r\n",res_flash);
    printf("！！可能原因：SPI Flash初始化不成功。\r\n");
		while(1);
  }
  else
  {
    printf("》文件系统挂载成功，可以进行读写测试\r\n");
  }
}
	
	
	
	
void anolyses_Commend()
{
	unsigned long address=0;
	unsigned char length=0;
	/*----------------------- 文件系统测试：写测试 -------------------*/
	
	/* 打开文件，每次都以新建的形式打开，属性为可写 */
	while(finish_flag==0);
	printf("收到的协议长度为：%d \n",arry_i);
	if(made_flag==0)
	{
		printf("\r\n****** 即将进行文件写入测试... ******\r\n");	
		//第一次是创建，第二次是添加
		res_flash = f_open(&fnew, "1:智能菜谱.txt",FA_CREATE_ALWAYS | FA_WRITE );
		//res_flash = f_open(&fnew, "1:智能菜谱.txt",  FA_WRITE );
		if ( res_flash == FR_OK )
		{
			printf("》打开/创建FatFs读写测试文件.txt文件成功，向文件写入数据。\r\n");
			/* 将指定存储区内容写入到文件内 */
			//res_flash=f_write(&fnew,dat,arry_i,&fnum);
			
			//读取文件长度
			f_stat("1:智能菜谱.txt",&file_info);
			f_lseek(&fnew,file_info.fsize);
			f_puts(dat,&fnew);
			if(res_flash==FR_OK)
			{
				printf("》文件写入成功，写入字节数据：%d\n",fnum);
				printf("》向文件写入的数据为：\r\n%s\r\n",dat);
			}
			else
			{
				printf("！！文件写入失败：(%d)\n",res_flash);
			}   
	
			/* 不再读写，关闭文件 */
			f_close(&fnew);
			
		}
	
		else
		{	
			printf("！！打开/创建文件失败。\r\n");
		}
}
	
/*------------------- 文件系统测试：读测试 --------------------------*/
	if(made_flag==1)
	{
		printf("****** 即将进行文件读取测试... ******\r\n");
		res_flash = f_open(&fnew, "1:智能菜谱.txt",FA_OPEN_EXISTING | FA_READ); 	 
		if(res_flash == FR_OK)
		{
			printf("》打开文件成功。\r\n");
			
			//定位到文件相应的位置**********************
			address=0;
			address|=arry[4]<<16;
			address|=arry[5]<<8;
			address|=arry[6]<<0;
			
			//读取数据的长度
			length=arry[7];
			
			f_lseek(&fnew,address);
			res_flash = f_read(&fnew, ReadBuffer, length, &fnum); 
			if(res_flash==FR_OK)
			{
				printf("》文件读取成功,读到字节数据：%d\r\n",fnum);
				printf("》读取得的文件------数据为：\r\n%s \r\n", ReadBuffer);	
				
				//置位炒菜标志位
				
			}
			else
			{
				printf("！！文件读取失败：(%d)\n",res_flash);
			}		
			/* 不再读写，关闭文件 */
			f_close(&fnew);	
		}
		else
		{
			printf("！！打开文件失败。\r\n");
		}
		
	}
  
	/* 不再使用文件系统，取消挂载文件系统 */
	f_mount(NULL,"1:",1);
  
	//将蓝牙读取完成标志置零
	finish_flag=0;
}

