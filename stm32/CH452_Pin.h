#include "stm32f2xx.h"
#include "CH452cmd.h"

/*
*********************************************************************************
* 1.采用三线制 SCL 和 SCL LOAD 输出模式
* 2.不使用CD452的键盘输出
* 3. SCL PC10 与 SDA  PC12 为共用接口，LOAD1~LOAD5 分别对应PE2 PE3 PE4 PA12 PA11
* 4. 经过详细阅读比较，并非采用SPI与通信，只是接线类似， 另外一种通信方式兼容的IIC通信 
*    但与客户需求不同，无法满足条件
* 5. SCL SDA LOAD 对于单片机来说 都是输出端口
*********************************************************************************
*/

#define CH452_SCL_Pin    GPIO_Pin_10
#define CH452_SCL_Port   GPIOC       

#define CH452_SDA_Pin	 GPIO_Pin_12
#define CH452_SDA_Port   GPIOC

#define CH452_LOAD_U1_Pin    GPIO_Pin_2
#define CH452_LOAD_U2_Pin    GPIO_Pin_3
#define CH452_LOAD_U3_Pin    GPIO_Pin_4
#define CH452_LOAD1_Port   GPIOE 

#define CH452_LOAD_U4_Pin    GPIO_Pin_12
#define CH452_LOAD_U5_Pin    GPIO_Pin_11
#define CH452_LOAD2_Port   GPIOA 

/* 3线接口的位操作 */
#define		CH452_DCLK_SET		{GPIO_WriteBit(CH452_SCL_Port, CH452_SCL_Pin, Bit_SET);}
#define		CH452_DCLK_CLR		{GPIO_WriteBit(CH452_SCL_Port, CH452_SCL_Pin, Bit_RESET);}
	
#define		CH452_DIN_SET		{GPIO_WriteBit(CH452_SCL_Port, CH452_SDA_Pin, Bit_SET);}
#define		CH452_DIN_CLR		{GPIO_WriteBit(CH452_SCL_Port, CH452_SDA_Pin, Bit_RESET);}


#define		CH452_LOAD1_SET		{GPIO_WriteBit(CH452_LOAD1_Port, CH452_LOAD_U1_Pin, Bit_SET);}
#define		CH452_LOAD1_CLR		{GPIO_WriteBit(CH452_LOAD1_Port, CH452_LOAD_U1_Pin, Bit_RESET);}

#define		CH452_LOAD2_SET		{GPIO_WriteBit(CH452_LOAD1_Port, CH452_LOAD_U2_Pin, Bit_SET);}
#define		CH452_LOAD2_CLR		{GPIO_WriteBit(CH452_LOAD1_Port, CH452_LOAD_U2_Pin, Bit_RESET);}

#define		CH452_LOAD3_SET		{GPIO_WriteBit(CH452_LOAD1_Port, CH452_LOAD_U3_Pin, Bit_SET);}
#define		CH452_LOAD3_CLR		{GPIO_WriteBit(CH452_LOAD1_Port, CH452_LOAD_U3_Pin, Bit_RESET);}

#define		CH452_LOAD4_SET		{GPIO_WriteBit(CH452_LOAD2_Port, CH452_LOAD_U4_Pin, Bit_SET);}
#define		CH452_LOAD4_CLR		{GPIO_WriteBit(CH452_LOAD2_Port, CH452_LOAD_U4_Pin, Bit_RESET);}

#define		CH452_LOAD5_SET		{GPIO_WriteBit(CH452_LOAD2_Port, CH452_LOAD_U5_Pin, Bit_SET);}
#define		CH452_LOAD5_CLR		{GPIO_WriteBit(CH452_LOAD2_Port, CH452_LOAD_U5_Pin, Bit_RESET);}




