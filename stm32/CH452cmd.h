/* 常量定义 */

/* CH451和CH452的常用命令码 */
#define CH452_NOP		    0x0000					// 空操作
#define CH452_RESET     0x0201					// 复位
#define CH452_LEVEL		  0x0100					// 加载光柱值,需另加7位数据
#define CH452_CLR_BIT	  0x0180					// 段位清0,需另加6位数据
#define CH452_SET_BIT	  0x01C0					// 段位置1,需另加6位数据
#define CH452_SLEEP		  0x0202					// 进入睡眠状态
#define CH452_LEFTMOV   0x0300		            // 设置移动方式-左移
#define CH452_LEFTCYC   0x0301		            // 设置移动方式-左循环
#define CH452_RIGHTMOV  0x0302		            // 设置移动方式-右移
#define CH452_RIGHTCYC  0x0303		            // 设置移动方式-右循环	
#define CH452_SELF_BCD	0x0380					// 自定义BCD码,需另加7位数据
#define CH452_SYSOFF    0x0400					// 关闭显示、关闭键盘
#define CH452_SYSON1    0x0401					// 开启显示
#define CH452_SYSON2    0x0403					// 开启显示、键盘
#define CH452_SYSON2W   0x0423					// 开启显示、键盘, 真正2线接口
#define CH452_NO_BCD    0x0500					// 设置默认显示方式,可另加3位扫描极限
#define CH452_BCD       0x0580					// 设置BCD译码方式,可另加3位扫描极限
#define CH452_TWINKLE   0x0600		            // 设置闪烁控制,需另加8位数据
#define CH452_GET_KEY	  0x0700					// 获取按键,返回按键代码
#define CH452_DIG0      0x0800					// 数码管位0显示,需另加8位数据
#define CH452_DIG1      0x0900		            // 数码管位1显示,需另加8位数据
#define CH452_DIG2      0x0a00		            // 数码管位2显示,需另加8位数据
#define CH452_DIG3      0x0b00		            // 数码管位3显示,需另加8位数据
#define CH452_DIG4      0x0c00		            // 数码管位4显示,需另加8位数据
#define CH452_DIG5      0x0d00					// 数码管位5显示,需另加8位数据
#define CH452_DIG6      0x0e00					// 数码管位6显示,需另加8位数据
#define CH452_DIG7      0x0f00		            // 数码管位7显示,需另加8位数据


/*  数码管对应的0到4位 BCD码  */
/*
 字符0： 0x00
 字符1： 0x01
 字符2： 0x02
 字符3： 0x03
 ......
 字符8： 0x08
 字符9： 0x09
 字符A： 0x0A
 字符B： 0x0B
 字符C： 0x0C
 字符D： 0x0D
 字符E： 0x0E
 字符F： 0x0F
 
 与上面的数码管位对应的命令相加，重新构成一个12位移位寄存器的值
 
*/


// BCD译码方式下的特殊字符
#define		CH452_BCD_SPACE		0x10            //空格
#define		CH452_BCD_PLUS		0x11            // + 号
#define		CH452_BCD_MINUS		0x12            // - 号
#define		CH452_BCD_EQU		0x13            // = 号
#define		CH452_BCD_LEFT		0x14            // [ 左括号
#define		CH452_BCD_RIGHT		0x15            // ] 右括号
#define		CH452_BCD_UNDER		0x16            // _ 下划线
#define		CH452_BCD_CH_H		0x17            // H 字母
#define		CH452_BCD_CH_L		0x18            // L 字母
#define		CH452_BCD_CH_P		0x19            // P 字母
#define		CH452_BCD_DOT		0x1A            // . 小数点
#define		CH452_BCD_SELF		0x1E            // 自定义
#define		CH452_BCD_TEST		0x88            
#define		CH452_BCD_DOT_X		0x80            //数字字符边上的小数点

// 有效按键代码
#define		CH452_KEY_MIN		0x40
#define		CH452_KEY_MAX		0x7F

// 2线接口的CH452定义
#define		CH452_I2C_ADDR0		0x40			// CH452的ADDR=0时的地址
#define		CH452_I2C_ADDR1		0x60			// CH452的ADDR=1时的地址,默认值
#define		CH452_I2C_MASK		0x3E			// CH452的2线接口高字节命令掩码

