#include "ble.h"
#include "lcd.h"
#include "uart4.h"
#include "usart.h"

void BLE_Init(void)
{  
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB,PE�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);						 //PB.5 �����
}

//��������ģʽ
u8 ble_test(void)
{
	OPEN_BLE=1;
	while(1)
	{ 
  	if (UART4_RX_CNT) 
		{	
			delay_ms(100);
      if ((UART4_RX_BUF[0]==0xa5) && (UART4_RX_BUF[1]==0x5a))  //��ͨ��������
			{
				if (UART4_RX_BUF[3]==0x83)
				{
					if (UART4_RX_BUF[8]==0x21)
					{
						UART4_RX_CNT=0;
						return 1;
					}
				}
			}
		  UART4_RX_CNT=0;
		}
	}
}

