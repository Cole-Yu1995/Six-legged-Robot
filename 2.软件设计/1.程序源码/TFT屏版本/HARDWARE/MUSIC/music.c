#include "music.h"
#include "lcd.h"
#include "intel_window.h"
#include "sys.h"



//����ʶ��ģʽ��ʼ��
void MUSIC_Init(void)
{
//  GPIO_InitTypeDef  GPIO_InitStructure;

//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOFʱ��

//  //GPIOF9,F10��ʼ������
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
//  GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��
//	
//	GPIO_ResetBits(GPIOF,GPIO_Pin_10);//GPIOF10���õ�
}


//�������ֲ���ģʽ
void music_test(void)
{
	u8 i;
	u8 flag=0;
	LCD_Clear(BLACK);
  POINT_COLOR=BROWN;	
	Show_Str(50,150,240,24, "���ֲ���ģʽ",24,1);	
	POINT_COLOR=GRAY;	
	LCD_DrawRectangle(0,180+92,180+240,180+140);
	Show_Str(65,320-40,240,24, "������һҳ",24,1);	
	
	while(!paj7620u2_init());//PAJ7620U2��������ʼ�� 
	
	music_handleGesture();//��������

}

