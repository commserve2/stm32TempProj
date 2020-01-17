/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
//#include "stm32_eval.h"
#include <stdio.h>

#ifdef USE_STM32100B_EVAL
 #include "stm32100b_eval_lcd.h"
#elif defined USE_STM3210B_EVAL
 #include "stm3210b_eval_lcd.h"
#elif defined USE_STM3210E_EVAL
 #include "stm3210e_eval_lcd.h" 
#elif defined USE_STM3210C_EVAL
 #include "stm3210c_eval_lcd.h"
#elif defined USE_STM32100E_EVAL
 #include "stm32100e_eval_lcd.h"
#endif

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifdef USE_STM32100B_EVAL
  #define MESSAGE1   "STM32 MD Value Line " 
  #define MESSAGE2   " Device running on  " 
  #define MESSAGE3   "  STM32100B-EVAL    " 
#elif defined (USE_STM3210B_EVAL)
  #define MESSAGE1   "STM32 Medium Density" 
  #define MESSAGE2   " Device running on  " 
  #define MESSAGE3   "   STM3210B-EVAL    " 
#elif defined (STM32F10X_XL) && defined (USE_STM3210E_EVAL)
  #define MESSAGE1   "  STM32 XL Density  " 
  #define MESSAGE2   " Device running on  " 
  #define MESSAGE3   "   STM3210E-EVAL    "
#elif defined (USE_STM3210E_EVAL)
  #define MESSAGE1   " STM32 High Density " 
  #define MESSAGE2   " Device running on  " 
  #define MESSAGE3   "   STM3210E-EVAL    " 
#elif defined (USE_STM3210C_EVAL)
  #define MESSAGE1   " STM32 Connectivity " 
  #define MESSAGE2   " Line Device running" 
  #define MESSAGE3   " on STM3210C-EVAL   "
#elif defined (USE_STM32100E_EVAL)
  #define MESSAGE1   "STM32 HD Value Line " 
  #define MESSAGE2   " Device running on  " 
  #define MESSAGE3   "  STM32100E-EVAL    "   
#endif

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
 USART_InitTypeDef USART_InitStructure;

/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

#define PA1 (GPIOA->BSRR)
#define PA0 (GPIOA->BRR)

#define GPIOA_ODR GPIOA_BASE+0x0C
#define GPIOA_IDR GPIOA_BASE+0x08
#define GPIOB_ODR GPIOB_BASE+0x0C
#define GPIOB_IDR GPIOB_BASE+0x08
#define GPIOC_ODR GPIOC_BASE+0x0C
#define GPIOC_IDR GPIOC_BASE+0x08
#define GPIOD_ODR GPIOD_BASE+0x0C
#define GPIOD_IDR GPIOD_BASE+0x08
#define GPIOE_ODR GPIOE_BASE+0x0C
#define GPIOE_IDR GPIOE_BASE+0x08

#define BitBind(Addr, BitNum)  *((unsigned int*)((Addr&0xF0000000 | 0x2000000) + ((Addr&0xFFFFF) << 5) + (BitNum << 2)))

#define PAout(n) BitBind(GPIOA_ODR,n)
#define PAin(n) BitBind(GPIOA_IDR,n)
#define PBout(n) BitBind(GPIOB_ODR,n)
#define PBin(n) BitBind(GPIOB_IDR,n)
#define PCout(n) BitBind(GPIOC_ODR,n)
#define PCin(n) BitBind(GPIOC_IDR,n)
#define PDout(n) BitBind(GPIOD_ODR,n)
#define PDin(n) BitBind(GPIOD_IDR,n)
#define PEout(n) BitBind(GPIOE_ODR,n)
#define PEin(n) BitBind(GPIOE_IDR,n)

/* Private functions ---------------------------------------------------------*/
void RRC_Configuration(void)
{
	// 使用外部RC晶振
	RCC_DeInit();  //初始化为缺省值
	RCC_HSEConfig(RCC_HSE_ON); // 使能外部的高速时钟
	while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET); // 等待外部高速时钟使能就绪
	
	//FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); // Enable Prefetch Buffer
	//FLASH_SetLatency(FLASH_Latency_2); // Flash 2 wait state
	
	RCC_HCLKConfig(RCC_SYSCLK_Div1); // HCLK = SYSCLK
	RCC_PCLK2Config(RCC_HCLK_Div1);  // PCLK2 = HCLK
	RCC_PCLK1Config(RCC_HCLK_Div2);  // PCLK1 = HCLK/2
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); // PLLCLK = 8MHz * 9 = 72MHz
	RCC_PLLCmd(ENABLE); // Enable PLLCLK
	while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET); // Wait util PLLCLK is ready
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); // Select PLL as system clock
	while (RCC_GetSYSCLKSource() != 0x08); // Wait util PLL is used as system clock
	
	// 打开相应的外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 使能APB2外设的GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // 使能APB2外设的GPIOC的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Configure USARTx_TX as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Configure USARTx_RX as inpute floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void USART_Configuration(uint32_t baudRate)
{
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = baudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
}
/**
  * @brief  重定向输出字符到 USART 设备, 实现C库的printf
  *         
  * @param  ch: 要输出的字符
  * @param  f:  printf 输出流的文件指针,这里不使用
  * @retval ch
  */
int fputc(int ch, FILE* f)
{
	USART_SendData(USART1, ch);
	while (RESET == USART_GetFlagStatus(USART1, USART_FLAG_TC));
	return ch;
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	//uint16_t buard = 9600;
	//uint16_t M, F;
	//float div;
	uint8_t output = 'A';
	RRC_Configuration();
	GPIO_Configuration();
	USART_Configuration(19600);
	//  // USART1 模块配置: UE位使能, M位定义字长、停止位的位数，TE位， BBR寄存器选择要求的波特率
	//  USART1->CR1 |= (1 << 13);
	//	USART1->CR1 &= ~(1 << 12);
	//	USART1->CR2 &= ~(3 << 12);
	//	USART1->CR1 |= (1 << 3);
	//  // 波特率的计算 9600 = 108 M / (16 * U),  U= 703.125
	//	//USART1->BRR = (0x1D4) << 4 | 0x0C;
	//	div = (float)108000000/(buard*16);
	//	M = div;
	//	F = (div-M) * 16;
	//	USART1->BRR = (M << 4 | F);
  // 发送一个字符 A 到 USART1 的TXD
  // USART1->DR = 'A';
	
	// 发送20个字符 到USART1 的TXD
	for (uint8_t i = 0; i < 20; i++)
	{
		USART_SendData(USART1, (output+i));
		while (RESET == USART_GetFlagStatus(USART1, USART_FLAG_TC));
	}
	printf("\n www.baidu.com");
}

///**
//  * @brief  Retargets the C library printf function to the USART.
//  * @param  None
//  * @retval None
//  */
//PUTCHAR_PROTOTYPE
//{
//  /* Place your implementation of fputc here */
//  /* e.g. write a character to the USART */
//  USART_SendData(EVAL_COM1, (uint8_t) ch);

//  /* Loop until the end of transmission */
//  while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
//  {}

//  return ch;
//}

//#ifdef  USE_FULL_ASSERT

///**
//  * @brief  Reports the name of the source file and the source line number
//  *         where the assert_param error has occurred.
//  * @param  file: pointer to the source file name
//  * @param  line: assert_param error line source number
//  * @retval None
//  */
//void assert_failed(uint8_t* file, uint32_t line)
//{ 
//  /* User can add his own implementation to report the file name and line number,
//     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

//  /* Infinite loop */
//  while (1)
//  {
//  }
//}
//#endif

/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
