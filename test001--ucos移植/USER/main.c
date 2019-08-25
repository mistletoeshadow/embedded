#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "includes.h"  //ucosII下的头文件

 
 
/************************************************
 ALIENTEK战舰STM32开发板实验1
ucosII test实验 
 修改：mistletoe65   2019/8/21 
 作者：正点原子 @ALIENTEK
************************************************/


//START 任务
//设置任务优先级
#define START_TASK_PRIO     10  //开始任务的优先级最低
//设置任务栈的大小
#define START_STK_SIZE      128
//任务堆栈
OS_STK START_TASK_STK[START_STK_SIZE];
void start_task(void *pdata);


//LED0任务
//设置任务优先级
#define LED0_TASK_PRIO			7
//设置任务堆栈大小
#define LED0_STK_SIZE       128
//任务堆栈
OS_STK LED0_TASK_STK[LED0_STK_SIZE];
//任务函数
void led0_task(void *pdata);


//LED1任务
//设置任务优先级
#define LED1_TASK_PRIO				6
//设置任务堆栈大小
#define LED1_STK_SIZE				128
//任务堆栈
OS_STK LED1_TASK_STK[LED1_STK_SIZE];
//任务函数
void led1_task(void *pdata);


//浮点测试任务
//设置任务优先级
#define FLOAT_TASK_PRIO 			5
//设置任务堆栈大小
#define FLOAT_STK_SIZE				128
//任务堆栈
__align(8) OS_STK FLOAT_TASK_STK[FLOAT_STK_SIZE];   //进行修改测试_align()作用
//任务函数
void float_task(void *pdata);


//开始函数
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr=0;
	pdata=pdata;
	OSStatInit();  //开始统计任务
	
	OS_ENTER_CRITICAL();//进入临界区（关闭中断）
	OSTaskCreate(led0_task,(void*)0,(OS_STK*)&LED0_TASK_STK[LED0_STK_SIZE-1],LED0_TASK_PRIO);
	OSTaskCreate(led1_task,(void*)0,(OS_STK*)&LED1_TASK_STK[LED1_STK_SIZE-1],LED1_TASK_PRIO);
	OSTaskCreate(float_task,(void*)0,(OS_STK*)&FLOAT_TASK_STK[LED0_STK_SIZE-1],FLOAT_TASK_PRIO);
	OSTaskSuspend(START_TASK_PRIO);		//挂起开始任务
	OS_EXIT_CRITICAL();   //退出临界区（开启中断）
}

//LEDO任务
void led0_task(void *pdata)
{
	while(1)
	{
		LED0=0;
		delay_ms(100);
		LED0=1;
		delay_ms(500);
	}
}

//任务二
void led1_task(void *pdata)
{
	while(1)
	{
		LED1=0;
		delay_ms(100);
		LED1=1;
		delay_ms(500);
	}
}

//任务三
void float_task(void *pdata)
{
	OS_CPU_SR cpu_sr=0;
	static float float_num=0.01;
	while(1)
	{
		float_num+=0.01f;
		OS_ENTER_CRITICAL(); //进入临界区（关闭中断）
		printf("float_num的值为 %.4f\r\n",float_num);
		OS_EXIT_CRITICAL();//退出临界区（开启中断）
		delay_ms(500);
	}
}

 int main(void)
 {	
	delay_init();	    //延时函数初始化	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断分组配置
	LED_Init();		  	//初始化与LED连接的硬件接口
	uart_init(115200);
	
	 OSInit();//UCOS初始化
	 OSTaskCreate(start_task,(void*)0,(OS_STK*)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO);//创建开始任务
	 OSStart();//任务开始，其他任务挂在开始任务下面
 }


 
