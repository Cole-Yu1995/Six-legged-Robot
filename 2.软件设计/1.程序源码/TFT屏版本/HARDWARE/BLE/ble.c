#include "ble.h"
#include "lcd.h"

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
void ble_test(void)
{
	LCD_Clear(BLACK);
  POINT_COLOR=BROWN;	
	Show_Str(25,150,240,24, "�̳���������ģʽ",24,1);	
	POINT_COLOR=GRAY;	
	LCD_DrawRectangle(0,180+92,180+240,180+140);
	Show_Str(70,320-40,240,24, "������ҳ",24,1);	
	
	OPEN_BLE=1;
	
	while(1)
	{
    if (window_back()) 	break;
	}
}

