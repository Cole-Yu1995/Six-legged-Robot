#include "gesture.h"
#include "lcd.h"
#include "delay.h"
#include "intel_window.h"



//����ʶ��ģʽ��ʼ��
void GESTURE_Init(void)
{
}


//��������ʶ��ģʽ
void gesture_test(void)
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
	handleGesture();//��������
	
	for(t=0;t<4;t++)
	{
		USART_SendData(USART1, stop_send_buf[t]);//�򴮿�1��������
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	}
}


