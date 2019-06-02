#include "delay.h"
#include "uart4.h"


//���ջ����� 	
u8 UART4_RX_BUF[64];  	//���ջ���,���64���ֽ�.
//���յ������ݳ���
u8 UART4_RX_CNT=0;   		  
  
void UART4_IRQHandler(void)
{
	u8 res;	 
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)//���յ�����
	{	 	
	  res =USART_ReceiveData(UART4);//;��ȡ���յ�������
		UART4_RX_BUF[UART4_RX_CNT]=res;		//��¼���յ���ֵ
		UART4_RX_CNT++;						//������������1 
	}  											 
}						

//��ʼ��IO ����2
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������	  
void uart4_init(u32 bound)
{  
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//ʹ��GPIOCʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);//ʹ��UART4ʱ��
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOC, &GPIO_InitStructure);  
  
	//UART-RX-PC11  
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
 
	USART_InitStructure.USART_BaudRate = bound;  
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  
	USART_InitStructure.USART_Parity = USART_Parity_No ;  
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  

	USART_Init(UART4, &USART_InitStructure);   
	USART_Cmd(UART4, ENABLE);     
	USART_ClearFlag(UART4,USART_FLAG_TC); 
  
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn; //ʹ�ܴ���4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
 
  USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//�����ж�
   
  USART_Cmd(UART4, ENABLE);                    //ʹ�ܴ��� 
}


















