用到的函数接口
******************************************************************************************
//电磁炉控制函数
void induction_cooker_contro(float temperature,unsigned char time,unsigned char start_degree)

******************************************************************************************
//定时函数，用定时器实现
void timing(unsigned char time)
******************************************************************************************
//电机控制函数
void motor_contro(unsigned char motor_n,unsigned char direction,
                       unsigned char speed_stage,float distance)
1.已经实现正传圈数的控制，反转没有调好
******************************************************************************************			
//电磁阀控制函数
void dianCiFa(unsigned char Fa_Name,unsigned char statement)
******************************************************************************************
//电磁炉状态获取函数
void getStatementOfCooker(struct stateStructure)
******************************************************************************************
//系统初始化函数
void init_System()
******************************************************************************************
//蓝牙传输函数
unsigned char send_By_Bluetooth(unsigned char str[])
unsigned char *str read_By_Bluetooth()
******************************************************************************************
//can传输函数
******************************************************************************************
unsigned char send_By_Can(unsigned char str[])
unsigned char *str read_By_Can()

******************************************************************************************

//flash存储函数
byte flash_read()
byte flash_write()

**********************************************



/*************************************************************************************



读写flash的测试文件


***************************************************************************************/
FATFS fs;													/* FatFs文件系统对象 */
FIL fnew;													/* 文件对象 */
FRESULT res_flash;                /* 文件操作结果 */
UINT fnum;            					  /* 文件成功读写数量 */
BYTE ReadBuffer[1024]={0};        /* 读缓冲区 */
BYTE WriteBuffer[] =              /* 写缓冲区*/
"欢迎使用秉火STM32开发板 xie 今天是个好日子，新建文件系统测试文件\r\n"; 

int main(void)
{
	/* 初始化LED */
	//LED_GPIO_Config();	
	//LED_BLUE;
	
	/* 初始化调试串口，一般为串口1 */
	USART1_Bluetooth_Init(9600);	
  printf("****** 这是一个SPI FLASH 文件系统实验 ******\r\n");
  
	//在外部SPI Flash挂载文件系统，文件系统挂载时会对SPI设备初始化
	//初始化函数调用流程如下
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
  
/*----------------------- 文件系统测试：写测试 -------------------*/
	/* 打开文件，每次都以新建的形式打开，属性为可写 */
//	printf("\r\n****** 即将进行文件写入测试... ******\r\n");	
//	res_flash = f_open(&fnew, "1:FatFs读写测试文件.txt",FA_CREATE_ALWAYS | FA_WRITE );
//	if ( res_flash == FR_OK )
//	{
//		printf("》打开/创建FatFs读写测试文件.txt文件成功，向文件写入数据。\r\n");
//    /* 将指定存储区内容写入到文件内 */
//		res_flash=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
//    if(res_flash==FR_OK)
//    {
//      printf("》文件写入成功，写入字节数据：%d\n",fnum);
//      printf("》向文件写入的数据为：\r\n%s\r\n",WriteBuffer);
//    }
//    else
//    {
//      printf("！！文件写入失败：(%d)\n",res_flash);
//    }    
//		/* 不再读写，关闭文件 */
//    f_close(&fnew);
//	}
//	else
//	{	
//		//LED_RED;
//		printf("！！打开/创建文件失败。\r\n");
//	}
//	
/*------------------- 文件系统测试：读测试 --------------------------*/
	printf("****** 即将进行文件读取测试... ******\r\n");
	res_flash = f_open(&fnew, "1:FatFs读写测试文件.txt",FA_OPEN_EXISTING | FA_READ); 	 
	if(res_flash == FR_OK)
	{
		//LED_GREEN;
		printf("》打开文件成功。\r\n");
		res_flash = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
    if(res_flash==FR_OK)
    {
      printf("》文件读取成功,读到字节数据：%d\r\n",fnum);
      printf("》读取得的文件------数据为：\r\n%s \r\n", ReadBuffer);	
    }
    else
    {
      printf("！！文件读取失败：(%d)\n",res_flash);
    }		
	}
	else
	{
		//LED_RED;
		printf("！！打开文件失败。\r\n");
	}
	/* 不再读写，关闭文件 */
	f_close(&fnew);	
  
	/* 不再使用文件系统，取消挂载文件系统 */
	f_mount(NULL,"1:",1);
  
  /* 操作完成，停机 */
	while(1)
	{
	}
}
*************************************************



/*******************************************************************************************

电机旋转（编码器的测试函数）

******************************************************************************************/

//	//编码器初始化，里面包括了使能定时器
//	Encode_Encoder_Init();
//	
//	//pwm初始化，里面包括了使能定时器
//	GPIO_MOTOR_INT(500,72-1);
//	
//	//旋转
//	revolve(DIRECTION_ZHENG,SPEED_STAGE_1,3.0);
//	
//	//等待
//	while(1);

******************************************************************************



/********************************资源分配*********************************/
蓝牙和串口同用一个ustar1    PA9  PA10
LED指示灯                   PC0-PC7
编码器         TIM2  PA0,PA1
锅升温控制pwm   TIM3 Pwm_PC8 IN1_PC2 IN2_PC3
锅旋转pwm       TIM3 Pwm_PC7 
搅拌轴pwm   TIM3 Pwm_PC6 IN1_PC0 IN2_PC1
外部定时器   TIM6,TIM7
spi PG13 PB13 14 15
翻菜 pwm
继电器  （液体泵7个：液体料） 推菜   抬菜机 洗锅 洗漏斗 洗底板 给锅加水 加汤 用PB来控制









