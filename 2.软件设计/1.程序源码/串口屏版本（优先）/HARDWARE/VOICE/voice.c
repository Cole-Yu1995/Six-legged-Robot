#include "voice.h"
#include "sys.h"
#include "usart2.h"	
#include "uart4.h"	

//����ʶ��ģʽ��ʼ��
void VOICE_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 //ʹ��PB,PE�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED1->PE.5 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOE, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOE.5
	GPIO_ResetBits(GPIOE,GPIO_Pin_5);						 //PE.5 �����
}

//��������ʶ��ģʽ
u8 voice_test(void)
{
	char stop_send_buf[5]={0x55,0x55,0x02,0x07};
  int j=0;
	int t=0;
  OPEN_VOICE=1;
	while(1)
	{
		if(USART2_RX_STA&0x8000)
		{					   
			j=USART2_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			for(t=0;t<j;t++)
			{
				USART_SendData(USART1, USART2_RX_BUF[t]);//�򴮿�1��������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
			}
			USART2_RX_STA=0;
		}
		if (UART4_RX_CNT) 
		{	
			delay_ms(100);
      if ((UART4_RX_BUF[0]==0xa5) && (UART4_RX_BUF[1]==0x5a))  //��ͨ��������
			{
				if (UART4_RX_BUF[3]==0x83)
				{
					if (UART4_RX_BUF[8]==0x36)
					{
						UART4_RX_CNT=0;
						for(t=0;t<4;t++)
						{
							USART_SendData(USART1, stop_send_buf[t]);//�򴮿�1��������
							while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
						}
						return 1;
					}
				}
			}
		  UART4_RX_CNT=0;
		}
	}
}

