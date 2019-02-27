#include "stm32f2xx.h"
#include "stdio.h"
#include "CH452cmd.h"
#include "CH452_Pin.h"
#include "CH452W3.h"
#include "rtthread.h"
/*
*********************************************************
* 函数说明：往CH452A的12位异步寄存器加载数据
* cmd命令参数：参照CH452cmd.h  文件  定义一个无符号整型变量来存储一个12字节的命令 
*********************************************************
*/
void CH452_Write_Cmd(unsigned short cmd)
{
	   unsigned char i;
	   for(i=0;i<12;i++)
				{
					 if(cmd&(1<<i)) 
								{CH452_DIN_SET;}
					 else
								{CH452_DIN_CLR;}     //输出位数据 		
								
					 CH452_DCLK_CLR;									
					 CH452_DCLK_SET;       //上升沿有效
				}
}
/*
*********************************************************
* 函数说明：SCL SDA LOAD 输出模式
*********************************************************
*/
void Pin_init_CH452(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOE, ENABLE);
 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //输出模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType =  GPIO_OType_PP; //推挽输出
  GPIO_InitStructure.GPIO_Pin = CH452_SCL_Pin|CH452_SDA_Pin;
  GPIO_Init( CH452_SCL_Port , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType =  GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Pin = CH452_LOAD_U1_Pin|CH452_LOAD_U2_Pin|CH452_LOAD_U3_Pin;
  GPIO_Init( CH452_LOAD1_Port , &GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType =  GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Pin = CH452_LOAD_U4_Pin|CH452_LOAD_U5_Pin;
  GPIO_Init( CH452_LOAD2_Port , &GPIO_InitStructure );
	
	CH452_LOAD1_SET;
	CH452_LOAD2_SET;
	CH452_LOAD3_SET;
	CH452_LOAD4_SET;
	CH452_LOAD5_SET;
	CH452_DCLK_SET;
	CH452_DIN_SET;
	
	CH452_Write_Init(1,CH452_RESET);
	CH452_Write_Init(2,CH452_RESET);
	CH452_Write_Init(3,CH452_RESET);
	CH452_Write_Init(4,CH452_RESET);
	CH452_Write_Init(5,CH452_RESET);

	
	CH452_Write_Init(1,CH452_SYSON1);
	CH452_Write_Init(1,CH452_BCD);
  CH452_Write_Init(1,CH452_SLEEP);	

	
	CH452_Write_Init(2,CH452_SYSON1);
	CH452_Write_Init(2,CH452_BCD); 
	CH452_Write_Init(2,CH452_SLEEP);	
	
	CH452_Write_Init(3,CH452_SYSON1);
	CH452_Write_Init(3,CH452_BCD); 
	CH452_Write_Init(3,CH452_SLEEP);	
	
	CH452_Write_Init(4,CH452_SYSON1);
	CH452_Write_Init(4,CH452_BCD); 
	CH452_Write_Init(4,CH452_SLEEP);	
	
	CH452_Write_Init(5,CH452_SYSON1);
	CH452_Write_Init(5,CH452_BCD); 
	CH452_Write_Init(5,CH452_SLEEP);	
}

/*
*********************************************************
* 函数说明：控制器往CH452写初始化命令
* m变量 ： 选择控制which one CH452，变量只能取1~5对应LOAD1~LOAD5，这里暂不对i变量取值做判断
* cmd命令参数：参照CH452cmd.h  文件  定义一个无符号整型变量来存储一个12字节的命令 
* 注 ：在调用CH452_Write_data（）命令函数后，需要对 最后一次进行指令操作的CH452A传感器进行一次空指令操作
       bug原因： CH452A会调用最后一次操作指令，加载一个空指令操作，对应用程序无影响
*********************************************************
*/
void CH452_Write_Init(unsigned char m,unsigned short cmd)
{	
	switch(m)
	{
			case 1:
        CH452_Write_Cmd(cmd);
				CH452_LOAD1_CLR;
			  CH452_LOAD1_SET; //加载数据，LOAD1上升沿，
								          //LOAD 用于加载串行数据	
				break;
			
			case 2:
        CH452_Write_Cmd(cmd);
				CH452_LOAD2_CLR;
				CH452_LOAD2_SET;  //加载数据，LOAD2上升沿			
				break;
			
			case 3:
        CH452_Write_Cmd(cmd);
				CH452_LOAD3_CLR;
				CH452_LOAD3_SET;  //加载数据，LOAD3上升沿
				break;
			
			case 4:
        CH452_Write_Cmd(cmd);
				CH452_LOAD4_CLR; 
				CH452_LOAD4_SET;  //加载数据，LOAD4上升沿
				break;
			
			case 5:
        CH452_Write_Cmd(cmd);
				CH452_LOAD5_CLR;
				CH452_LOAD5_SET;  //加载数据，LOAD5上升沿		
				break;
			
			default:
				break;
	}
}




/*
*********************************************************
* 函数说明：数码管 闪烁
* m变量 ：变量只能取1~10对应数码管1到10编号，这里暂不对i变量取值做判断
*********************************************************
*/
void CH452_Write_Glint(unsigned char m)
{
		unsigned short cmd;
		switch(m)
				{ 
					case 1:
						cmd = CH452_TWINKLE+0x07;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET; 
						rt_thread_delay(8);
						CH452_Write_Init(5,CH452_NOP);
						break;
						
					case 2:					
						cmd = CH452_TWINKLE+0x38;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
						rt_thread_delay(8);
						CH452_Write_Init(5,CH452_NOP);
						break;
					
					case 3:
						cmd = CH452_TWINKLE+0x07;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
						rt_thread_delay(8);
						CH452_Write_Init(4,CH452_NOP);
						break;
					
					case 4:
						cmd = CH452_TWINKLE+0x38;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
						rt_thread_delay(8);
						CH452_Write_Init(4,CH452_NOP);
						break;

					case 5:					
						cmd = CH452_TWINKLE+0x07;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
						rt_thread_delay(8);
						CH452_Write_Init(2,CH452_NOP);
						break;	
					
					case 6:
						cmd = CH452_TWINKLE+0x38;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
						rt_thread_delay(8);
						CH452_Write_Init(2,CH452_NOP);
						break;
					
					case 7:
						cmd = CH452_TWINKLE+0x07;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;	
            rt_thread_delay(8);		
						CH452_Write_Init(3,CH452_NOP);
						break;
					
					case 8:
						cmd = CH452_TWINKLE+0x38;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(3,CH452_NOP);
						break;
					
					case 9:
						cmd = CH452_TWINKLE+0x07;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
						rt_thread_delay(8);
						CH452_Write_Init(1,CH452_NOP);
						break;
					
					case 10:
						cmd = CH452_TWINKLE+0x38;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(1,CH452_NOP);
						break;
					
					default:
						break;
					 
				}
}



/*
*********************************************************
* 函数说明：数码管停止 闪烁
* m变量 ：变量只能取1~10对应数码管1到10编号，这里暂不对i变量取值做判断
*********************************************************
*/
void CH452_Write_NO_Glint(unsigned char m)
{
		unsigned short cmd;
		switch(m)
				{ 
					case 1:
						cmd = CH452_TWINKLE+0x00;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET; 
						rt_thread_delay(8);
						CH452_Write_Init(5,CH452_NOP);
						break;
						
					case 2:					
						cmd = CH452_TWINKLE+0x00;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
						rt_thread_delay(8);
						CH452_Write_Init(5,CH452_NOP);
						break;
					
					case 3:
						cmd = CH452_TWINKLE+0x00;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
						rt_thread_delay(8);
						CH452_Write_Init(4,CH452_NOP);
						break;
					
					case 4:
						cmd = CH452_TWINKLE+0x00;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
						rt_thread_delay(8);
						CH452_Write_Init(4,CH452_NOP);
						break;

					case 5:					
						cmd = CH452_TWINKLE+0x00;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
						rt_thread_delay(8);
						CH452_Write_Init(2,CH452_NOP);
						break;	
					
					case 6:
						cmd = CH452_TWINKLE+0x00;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
						rt_thread_delay(8);
						CH452_Write_Init(2,CH452_NOP);
						break;
					
					case 7:
						cmd = CH452_TWINKLE+0x00;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;	
            rt_thread_delay(8);		
						CH452_Write_Init(3,CH452_NOP);
						break;
					
					case 8:
						cmd = CH452_TWINKLE+0x00;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(3,CH452_NOP);
						break;
					
					case 9:
						cmd = CH452_TWINKLE+0x00;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
						rt_thread_delay(8);
						CH452_Write_Init(1,CH452_NOP);
						break;
					
					case 10:
						cmd = CH452_TWINKLE+0x00;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(1,CH452_NOP);
						break;
					
					default:
						break;
					 
				}
}

/*
*********************************************************
* 函数说明：余额不足信息显示 “___“   闪烁
* m变量 ：变量只能取1~10对应数码管1到10编号，这里暂不对i变量取值做判断
*********************************************************
*/
void CH452_Write_UnderLine(unsigned char m)
{
	  unsigned short cmd;
		switch(m)
				{ 
					case 1:
						cmd = 0x0800+CH452_BCD_UNDER;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0900+CH452_BCD_UNDER;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
						rt_thread_delay(8);
					
						cmd = 0x0a00+CH452_BCD_UNDER;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
						rt_thread_delay(8);
						CH452_Write_Init(5,CH452_NOP);
						break;
						
					case 2:
						cmd = 0x0b00+CH452_BCD_UNDER;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0c00+CH452_BCD_UNDER;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
						rt_thread_delay(8);
					
						cmd = 0x0d00+CH452_BCD_UNDER;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
						rt_thread_delay(8);
						CH452_Write_Init(5,CH452_NOP);
						break;
					
					case 3:
						cmd = 0x0800+CH452_BCD_UNDER;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0900+CH452_BCD_UNDER;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
						rt_thread_delay(8);
					
						cmd = 0x0a00+CH452_BCD_UNDER;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
						rt_thread_delay(8);
						CH452_Write_Init(4,CH452_NOP);
						break;
					
					case 4:
						cmd = 0x0b00+CH452_BCD_UNDER;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0c00+CH452_BCD_UNDER;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
						rt_thread_delay(8);
					
						cmd = 0x0d00+CH452_BCD_UNDER;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
						rt_thread_delay(8);
						CH452_Write_Init(4,CH452_NOP);
						break;

					case 5:
						cmd = 0x0800+CH452_BCD_UNDER;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0900+CH452_BCD_UNDER;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
						rt_thread_delay(8);
					
						cmd = 0x0a00+CH452_BCD_UNDER;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
						rt_thread_delay(8);
						CH452_Write_Init(2,CH452_NOP);
						break;	
					
					case 6:
						cmd = 0x0b00+CH452_BCD_UNDER;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0c00+CH452_BCD_UNDER;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
						rt_thread_delay(8);
					
						cmd = 0x0d00+CH452_BCD_UNDER;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
						rt_thread_delay(8);
						CH452_Write_Init(2,CH452_NOP);
						break;
					
					case 7:
						cmd = 0x0800+CH452_BCD_UNDER;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
						rt_thread_delay(8);
					
						cmd = 0x0900+CH452_BCD_UNDER;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0a00+CH452_BCD_UNDER;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;	
            rt_thread_delay(8);
						CH452_Write_Init(3,CH452_NOP);
						break;
					
					case 8:
						cmd = 0x0b00+CH452_BCD_UNDER;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET; 
					  rt_thread_delay(8);
					
						cmd = 0x0c00+CH452_BCD_UNDER;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
						rt_thread_delay(8);
					
						cmd = 0x0d00+CH452_BCD_UNDER;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
					  rt_thread_delay(8);
					  CH452_Write_Init(3,CH452_NOP);
						break;
					
					case 9:
						cmd = 0x0800+CH452_BCD_UNDER;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET; 
					  rt_thread_delay(8);
					
						cmd = 0x0900+CH452_BCD_UNDER;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0a00+CH452_BCD_UNDER;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
						rt_thread_delay(8);
						CH452_Write_Init(1,CH452_NOP);
						break;
					
					case 10:
						cmd = 0x0b00+CH452_BCD_UNDER;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET; 
					  rt_thread_delay(8);
					
						cmd = 0x0c00+CH452_BCD_UNDER;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0d00+CH452_BCD_UNDER;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(1,CH452_NOP);
						break;
					
					default:
						break;
					 
				}

}
/*
*********************************************************
* 函数说明：错误信息显示 “EEE“  不闪烁
* m变量 ：变量只能取1~10对应数码管1到10编号，这里暂不对i变量取值做判断
*********************************************************
*/

void CH452_Write_ERROR(unsigned char m)
{
	  unsigned short cmd;
switch(m)
				{ 
					case 1:
						cmd = 0x0800+0x0E;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0900+0x0E;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
						rt_thread_delay(8);
					
						cmd = 0x0a00+0x0E;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
						rt_thread_delay(8);
						CH452_Write_Init(5,CH452_NOP);
						break;
						
					case 2:
						cmd = 0x0b00+0x0E;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0c00+0x0E;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
						rt_thread_delay(8);
					
						cmd = 0x0d00+0x0E;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
						rt_thread_delay(8);
						CH452_Write_Init(5,CH452_NOP);
						break;
					
					case 3:
						cmd = 0x0800+0x0E;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0900+0x0E;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
						rt_thread_delay(8);
					
						cmd = 0x0a00+0x0E;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
						rt_thread_delay(8);
						CH452_Write_Init(4,CH452_NOP);
						break;
					
					case 4:
						cmd = 0x0b00+0x0E;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0c00+0x0E;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
						rt_thread_delay(8);
					
						cmd = 0x0d00+0x0E;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
						rt_thread_delay(8);
						CH452_Write_Init(4,CH452_NOP);
						break;

					case 5:
						cmd = 0x0800+0x0E;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0900+0x0E;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
						rt_thread_delay(8);
					
						cmd = 0x0a00+0x0E;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
						rt_thread_delay(8);
						CH452_Write_Init(2,CH452_NOP);
						break;	
					
					case 6:
						cmd = 0x0b00+0x0E;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0c00+0x0E;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
						rt_thread_delay(8);
					
						cmd = 0x0d00+0x0E;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
						rt_thread_delay(8);
						CH452_Write_Init(2,CH452_NOP);
						break;
					
					case 7:
						cmd = 0x0800+0x0E;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
						rt_thread_delay(8);
					
						cmd = 0x0900+0x0E;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0a00+0x0E;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;	
            rt_thread_delay(8);
						CH452_Write_Init(3,CH452_NOP);
						break;
					
					case 8:
						cmd = 0x0b00+0x0E;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET; 
					  rt_thread_delay(8);
					
						cmd = 0x0c00+0x0E;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
						rt_thread_delay(8);
					
						cmd = 0x0d00+0x0E;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(3,CH452_NOP);
						break;
					
					case 9:
						cmd = 0x0800+0x0E;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET; 
					  rt_thread_delay(8);
					
						cmd = 0x0900+0x0E;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0a00+0x0E;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
						rt_thread_delay(8);
						CH452_Write_Init(1,CH452_NOP);
						break;
					
					case 10:
						cmd = 0x0b00+0x0E;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET; 
					  rt_thread_delay(8);
					
						cmd = 0x0c00+0x0E;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0d00+0x0E;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(1,CH452_NOP);
						break;
					
					default:
						break;
					 
				}

}
/*
*********************************************************
* 函数说明：初始化调用该函数显示路数
* m变量 ：变量只能取1~10对应数码管1到10编号，这里暂不对i变量取值做判断
*********************************************************
*/
void CH452_Write_Way(unsigned char m)
{
	  unsigned short cmd;	
		switch(m)
				{ 	
					case 1:
						cmd = 0x0800+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0900+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
						rt_thread_delay(8);
					
						cmd = 0x0a00+0x01;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
						rt_thread_delay(8);
					  CH452_Write_Init(5,CH452_NOP);
						break;
						
					case 2:
						cmd = 0x0b00+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0c00+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
						rt_thread_delay(8);
					
						cmd = 0x0d00+0x02;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
						rt_thread_delay(8);
					  CH452_Write_Init(5,CH452_NOP);
						break;
					
					case 3:
						cmd = 0x0800+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0900+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
						rt_thread_delay(8);
					
						cmd = 0x0a00+0x03;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
						rt_thread_delay(8);
					  CH452_Write_Init(4,CH452_NOP);
						break;
					
					case 4:
						cmd = 0x0b00+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0c00+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
						rt_thread_delay(8);
					
						cmd = 0x0d00+0x04;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
						rt_thread_delay(8);
					  CH452_Write_Init(4,CH452_NOP);
						break;

					case 5:
						cmd = 0x0800+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0900+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
						rt_thread_delay(8);
					
						cmd = 0x0a00+0x05;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
						rt_thread_delay(8);
					  CH452_Write_Init(2,CH452_NOP);
						break;	
					
					case 6:
						cmd = 0x0b00+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0c00+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
						rt_thread_delay(8);
					
						cmd = 0x0d00+0x06;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
						rt_thread_delay(8);
					  CH452_Write_Init(2,CH452_NOP);
						break;
					
					case 7:
						cmd = 0x0800+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
						rt_thread_delay(8);
					
						cmd = 0x0900+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0a00+0x07;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;	
            rt_thread_delay(8);	
						CH452_Write_Init(3,CH452_NOP);
						break;
					
					case 8:
						cmd = 0x0b00+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET; 
					  rt_thread_delay(8);
					
						cmd = 0x0c00+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
						rt_thread_delay(8);
					
						cmd = 0x0d00+0x08;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(3,CH452_NOP);
						break;
					
					case 9:
						cmd = 0x0800+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET; 
					  rt_thread_delay(8);
					
						cmd = 0x0900+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0a00+0x09;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
						rt_thread_delay(8);
						CH452_Write_Init(1,CH452_NOP);
						break;
					
					case 10:
						cmd = 0x0b00+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET; 
					  rt_thread_delay(8);
					
						cmd = 0x0c00+0x01;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0d00+0x00;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(1,CH452_NOP);
						break;
					
					default:
						break;			 
				}
}


/*
*********************************************************
* 函数说明：控制器 往CH452写命令
* m变量 ：变量只能取1~10对应数码管1到10编号，这里暂不对i变量取值做判断
* date：直接金钱数字信息，可能是小数，需要进行处理判断 
*********************************************************
*/
void CH452_Write_Float_Data(unsigned char m,float date)
{   
	  unsigned short cmd;	
		if(date >1000)
		{
				date = date/10; 	
		}		
	  if(100 <= date )
		{
			unsigned int data = date;  					
				switch(m)
				{ 	
					case 1:
						cmd = 0x0800+data/100;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0900+data%100/10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
						rt_thread_delay(8);
					
						cmd = 0x0a00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
						rt_thread_delay(8);
					  CH452_Write_Init(5,CH452_NOP);
						break;
						
					case 2:
						cmd = 0x0b00+data/100;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0c00+data%100/10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
						rt_thread_delay(8);
					
						cmd = 0x0d00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
						rt_thread_delay(8);
					  CH452_Write_Init(5,CH452_NOP);
						break;
					
					case 3:
						cmd = 0x0800+data/100;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0900+data%100/10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
						rt_thread_delay(8);
					
						cmd = 0x0a00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
						rt_thread_delay(8);
					  CH452_Write_Init(4,CH452_NOP);
						break;
					
					case 4:
						cmd = 0x0b00+data/100;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0c00+data%100/10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
						rt_thread_delay(8);
					
						cmd = 0x0d00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
						rt_thread_delay(8);
					  CH452_Write_Init(4,CH452_NOP);
						break;

					case 5:
						cmd = 0x0800+data/100;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0900+data%100/10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
						rt_thread_delay(8);
					
						cmd = 0x0a00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
						rt_thread_delay(8);
					  CH452_Write_Init(2,CH452_NOP);
						break;	
					
					case 6:
						cmd = 0x0b00+data/100;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0c00+data%100/10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
						rt_thread_delay(8);
					
						cmd = 0x0d00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
						rt_thread_delay(8);
					  CH452_Write_Init(2,CH452_NOP);
						break;
					
					case 7:
						cmd = 0x0800+data/100;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
						rt_thread_delay(8);
					
						cmd = 0x0900+data%100/10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0a00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;	
            rt_thread_delay(8);	
						CH452_Write_Init(3,CH452_NOP);
						break;
					
					case 8:
						cmd = 0x0b00+data/100;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET; 
					  rt_thread_delay(8);
					
						cmd = 0x0c00+data%100/10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
						rt_thread_delay(8);
					
						cmd = 0x0d00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(3,CH452_NOP);
						break;
					
					case 9:
						cmd = 0x0800+data/100;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET; 
					  rt_thread_delay(8);
					
						cmd = 0x0900+data%100/10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0a00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
						rt_thread_delay(8);
						CH452_Write_Init(1,CH452_NOP);
						break;
					
					case 10:
						cmd = 0x0b00+data/100;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET; 
					  rt_thread_delay(8);
					
						cmd = 0x0c00+data%100/10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0d00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(1,CH452_NOP);
						break;
					
					default:
						break;			 
				}
	  }
	   else if( date < 10)
		 {
			 	unsigned int data = date*10;
			  switch(m)
				{ 
					case 1:
						cmd = 0x0800+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET; 
					  rt_thread_delay(8);
					
						cmd = 0x0900+data/10+CH452_BCD_DOT_X;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
					  rt_thread_delay(8);
					  				
						cmd = 0x0a00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(5,CH452_NOP);
						break;
						
					case 2:		
						cmd = 0x0b00+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0c00+data/10+CH452_BCD_DOT_X;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0d00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(5,CH452_NOP);
						break;
					
					case 3:
						cmd = 0x0800+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0900+data/10+CH452_BCD_DOT_X;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0a00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(4,CH452_NOP);
						break;
					
					case 4:
						cmd = 0x0b00+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0c00+data/10+CH452_BCD_DOT_X;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0d00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(4,CH452_NOP);
						break;

					case 5:
						cmd = 0x0800+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0900+data/10+CH452_BCD_DOT_X;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0a00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(2,CH452_NOP);
						break;	
					
					case 6:
						cmd = 0x0b00+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0c00+data/10+CH452_BCD_DOT_X;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0d00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(2,CH452_NOP);
						break;
					
					case 7:
						cmd = 0x0800+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0900+data/10+CH452_BCD_DOT_X;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0a00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;	
            rt_thread_delay(8);		
						CH452_Write_Init(3,CH452_NOP);
						break;
					
					case 8:
						cmd = 0x0b00+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0c00+data/10+CH452_BCD_DOT_X;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0d00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(3,CH452_NOP);
						break;
					
					case 9:
						cmd = 0x0800+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0900+data/10+CH452_BCD_DOT_X;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0a00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
            rt_thread_delay(8);	
						CH452_Write_Init(1,CH452_NOP);
						break;
					
					case 10:
						cmd = 0x0b00+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0c00+data/10+CH452_BCD_DOT_X;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0d00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(1,CH452_NOP);
						break;
					
					default:
						break;					 
				}	   
		 }
		 else
		 {
			 	unsigned int data = date * 10;
			  switch(m)
				{ 
					case 1:
						cmd = 0x0800+data/100;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
						rt_thread_delay(8);
					
						cmd = 0x0900+data%100/10+CH452_BCD_DOT_X;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
					  rt_thread_delay(8);
					  				
						cmd = 0x0a00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(5,CH452_NOP);
						break;
						
					case 2:
						cmd = 0x0b00+data/100;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET; 
					  rt_thread_delay(8);
					
						cmd = 0x0c00+data%100/10+CH452_BCD_DOT_X;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0d00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(5,CH452_NOP);
						break;
					
					case 3:
						cmd = 0x0800+data/100;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET; 
					  rt_thread_delay(8);
					
						cmd = 0x0900+data%100/10+CH452_BCD_DOT_X;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0a00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(4,CH452_NOP);
						break;
					
					case 4:
						cmd = 0x0b00+data/100;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET; 
					  rt_thread_delay(8);
					
						cmd = 0x0c00+data%100/10+CH452_BCD_DOT_X;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0d00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(4,CH452_NOP);
						break;

					case 5:
						cmd = 0x0800+data/100;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
						rt_thread_delay(8);
					
						cmd = 0x0900+data%100/10+CH452_BCD_DOT_X;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0a00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(2,CH452_NOP);
						break;	
					
					case 6:
						cmd = 0x0b00+data/100;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET; 
					  rt_thread_delay(8);
					
						cmd = 0x0c00+data%100/10+CH452_BCD_DOT_X;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0d00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(2,CH452_NOP);
						break;
					
					case 7:
						cmd = 0x0800+data/100;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET; 
					  rt_thread_delay(8);
					
						cmd = 0x0900+data%100/10+CH452_BCD_DOT_X;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0a00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;	
            rt_thread_delay(8);		
						CH452_Write_Init(3,CH452_NOP);
						break;
					
					case 8:
						cmd = 0x0b00+data/100;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET; 
					  rt_thread_delay(8);
					
						cmd = 0x0c00+data%100/10+CH452_BCD_DOT_X;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0d00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(3,CH452_NOP);
						break;
					
					case 9:
						cmd = 0x0800+data/100;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET; 
					  rt_thread_delay(8);
					
						cmd = 0x0900+data%100/10+CH452_BCD_DOT_X;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0a00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
            rt_thread_delay(8);	
						CH452_Write_Init(1,CH452_NOP);
						break;
					
					case 10:
						cmd = 0x0b00+data/100;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
            rt_thread_delay(8);					
					
						cmd = 0x0c00+data%100/10+CH452_BCD_DOT_X;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0d00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(1,CH452_NOP);
						break;
					
					default:
						break;					 
				}	    
		 }
}

/*
*********************************************************
* 函数说明：控制器 往CH452写命令
* m变量 ：变量只能取1~10对应数码管1到10编号
* date：直接时间数字信息 
*********************************************************
*/

void CH452_Write_Int_Data(unsigned char m,int date)
{
	unsigned short cmd;
	if(date > 1000)
	{
			date=date/10;
	}	
	if(100 <= date)
	{	
				unsigned int data = date;
				switch(m)
				{ 
					case 1:
						cmd = 0x0800+data/100;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0900+data%100/10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
						rt_thread_delay(8);
					
						cmd = 0x0a00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
						rt_thread_delay(8);
					  CH452_Write_Init(5,CH452_NOP);
						break;
						
					case 2:
						cmd = 0x0b00+data/100;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0c00+data%100/10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
						rt_thread_delay(8);
					
						cmd = 0x0d00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
						rt_thread_delay(8);
					  CH452_Write_Init(5,CH452_NOP);
						break;
					
					case 3:
						cmd = 0x0800+data/100;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0900+data%100/10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
						rt_thread_delay(8);
					
						cmd = 0x0a00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
						rt_thread_delay(8);
					  CH452_Write_Init(4,CH452_NOP);
						break;
					
					case 4:
						cmd = 0x0b00+data/100;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0c00+data%100/10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
						rt_thread_delay(8);
					
						cmd = 0x0d00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
						rt_thread_delay(8);
					  CH452_Write_Init(4,CH452_NOP);
						break;

					case 5:
						cmd = 0x0800+data/100;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0900+data%100/10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
						rt_thread_delay(8);
					
						cmd = 0x0a00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
						rt_thread_delay(8);
					  CH452_Write_Init(2,CH452_NOP);
						break;	
					
					case 6:
						cmd = 0x0b00+data/100;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0c00+data%100/10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
						rt_thread_delay(8);
					
						cmd = 0x0d00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
						rt_thread_delay(8);
					  CH452_Write_Init(2,CH452_NOP);
						break;
					
					case 7:
						cmd = 0x0800+data/100;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
						rt_thread_delay(8);
					
						cmd = 0x0900+data%100/10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0a00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;	
            rt_thread_delay(8);	
						CH452_Write_Init(3,CH452_NOP);
						break;
					
					case 8:
						cmd = 0x0b00+data/100;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET; 
					  rt_thread_delay(8);
					
						cmd = 0x0c00+data%100/10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
						rt_thread_delay(8);
					
						cmd = 0x0d00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(3,CH452_NOP);
						break;
					
					case 9:
						cmd = 0x0800+data/100;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET; 
					  rt_thread_delay(8);
					
						cmd = 0x0900+data%100/10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0a00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
						rt_thread_delay(8);
						CH452_Write_Init(1,CH452_NOP);
						break;
					
					case 10:
						cmd = 0x0b00+data/100;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET; 
					  rt_thread_delay(8);
					
						cmd = 0x0c00+data%100/10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0d00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(1,CH452_NOP);
						break;
					
					default:
						break;			 
				}
	}
	else if(date < 10)
	{
				unsigned int data = date;
			  switch(m)
				{ 
					case 1:
						cmd = 0x0800+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET; 
					  rt_thread_delay(8);
					
						cmd = 0x0900+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
					  rt_thread_delay(8);
					  				
						cmd = 0x0a00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(5,CH452_NOP);
						break;
						
					case 2:		
						cmd = 0x0b00+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0c00+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0d00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(5,CH452_NOP);
						break;
					
					case 3:
						cmd = 0x0800+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0900+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0a00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(4,CH452_NOP);
						break;
					
					case 4:
						cmd = 0x0b00+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0c00+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0d00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(4,CH452_NOP);
						break;

					case 5:
						cmd = 0x0800+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0900+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0a00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(2,CH452_NOP);
						break;	
					
					case 6:
						cmd = 0x0b00+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0c00+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0d00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(2,CH452_NOP);
						break;
					
					case 7:
						cmd = 0x0800+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0900+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0a00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;	
            rt_thread_delay(8);		
						CH452_Write_Init(3,CH452_NOP);
						break;
					
					case 8:
						cmd = 0x0b00+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0c00+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0d00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(3,CH452_NOP);
						break;
					
					case 9:
						cmd = 0x0800+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0900+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0a00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
            rt_thread_delay(8);	
						CH452_Write_Init(1,CH452_NOP);
						break;
					
					case 10:
						cmd = 0x0b00+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0c00+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0d00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(1,CH452_NOP);
						break;
					
					default:
						break;					 
				}	 
	
	}
	else
	{
			unsigned int data = date;
			 switch(m)
				{ 
					case 1:
						cmd = 0x0800+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET; 
					  rt_thread_delay(8);						
					
						cmd = 0x0900+data/10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
					  rt_thread_delay(8);
					  				
						cmd = 0x0a00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(5,CH452_NOP);
						break;
						
					case 2:	
						cmd = 0x0b00+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0c00+data/10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0d00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD5_CLR;
					  rt_thread_delay(3);
						CH452_LOAD5_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(5,CH452_NOP);
						break;
					
					case 3:
						cmd = 0x0800+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0900+data/10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0a00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(4,CH452_NOP);
						break;
					
					case 4:
						cmd = 0x0b00+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0c00+data/10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0d00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD4_CLR;
					  rt_thread_delay(3);
						CH452_LOAD4_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(4,CH452_NOP);
						break;

					case 5:
						cmd = 0x0800+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0900+data/10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0a00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(2,CH452_NOP);
						break;	
					
					case 6:
						cmd = 0x0b00+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0c00+data/10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0d00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD2_CLR;
					  rt_thread_delay(3);
						CH452_LOAD2_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(2,CH452_NOP);
						break;
					
					case 7:
						cmd = 0x0800+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0900+data/10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0a00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;	
            rt_thread_delay(8);		
						CH452_Write_Init(3,CH452_NOP);
						break;
					
					case 8:
						cmd = 0x0800+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0c00+data/10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0d00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD3_CLR;
					  rt_thread_delay(3);
						CH452_LOAD3_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(3,CH452_NOP);
						break;
					
					case 9:
						cmd = 0x0800+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0900+data/10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0a00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
            rt_thread_delay(8);	
						CH452_Write_Init(1,CH452_NOP);
						break;
					
					case 10:
						cmd = 0x0b00+CH452_BCD_SPACE;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET; 
						rt_thread_delay(8);
					
						cmd = 0x0c00+data/10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
					  rt_thread_delay(8);
					
						cmd = 0x0d00+data%10;
						CH452_Write_Cmd(cmd);
						CH452_LOAD1_CLR;
					  rt_thread_delay(3);
						CH452_LOAD1_SET;
					  rt_thread_delay(8);
						CH452_Write_Init(1,CH452_NOP);
						break;
					
					default:
						break;					 
				}	    
	  }

}

