#include "voice.h"
#include "lcd.h"
#include "intel_window.h"
#include "sys.h"
#include "usart2.h"	

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
void voice_test(void)
{
	char stop_send_buf[5]={0x55,0x55,0x02,0x07};
  int j=0;
	int t=0;
	LCD_Clear(BLACK);
  POINT_COLOR=BROWN;	
	Show_Str(50,150,240,24, "����ʶ��ģʽ",24,1);	
	POINT_COLOR=GRAY;	
	LCD_DrawRectangle(0,180+92,180+240,180+140);
	Show_Str(65,320-40,240,24, "������һҳ",24,1);	
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
			//Show_Str(20,180,240,24, "OK",24,1);
		}
    if (intel_window_back())
    {
			for(t=0;t<4;t++)
			{
				USART_SendData(USART1, stop_send_buf[t]);//�򴮿�1��������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
			}
			break;
		}
	}
}

