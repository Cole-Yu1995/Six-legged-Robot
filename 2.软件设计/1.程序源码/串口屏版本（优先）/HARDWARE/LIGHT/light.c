#include "light.h"
#include "lcd.h"
#include "delay.h"

//����Ѳ��ģʽ��ʼ��
void LIGHT_Init(void)
{    	 
 	GPIO_InitTypeDef GPIO_InitStructure;  //��ʼ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);//ʹ��PORTFʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;//PE0~2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //���óɸ�������
 	GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��GPIOE0,1,2
}

/********************��������ú���*************************/
void PrintCom(u8 *DAT,u8 len)
{
	u8 t;
	for(t=0;t<len;t++)
	{
	 USART_SendData(USART1, DAT[t]);//�򴮿�1��������
	 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	}
}

void stopActionGroup()
{
	u8 buf[6];
	buf[0] = 0x55 ;
	buf[1] = 0x55 ;
	buf[2] = 2 ;
	buf[3] = 0x07 ;
	PrintCom(buf, 4) ;
}

void runActionGroup(u8 numofAction, int Times)
{
	static u8 buf[9];
	buf[0] = 0x55 ;
	buf[1] = 0x55 ;
	buf[2] = 5 ;
	buf[3] = 0x06 ;
	buf[4] = (u8)numofAction ;		
	buf[5] = GET_LOW_BYTE(Times) ;
	buf[6] = GET_HIGH_BYTE((int)Times) ;
	PrintCom(buf, 7) ;
}

void light_handle()
{
	static u8  step = 0;   //��̬���������ڼ�¼����
  static u8  lastStep = 0; //��̬���������ڼ�¼���һ���Ĳ���
	static u8  actiongroup_flag=0;

	switch (step)
	{
		case 0:        //����0
      if (!actiongroup_flag)              //���û�ж�����������
			{  
        if ((LIGHT0) && (LIGHT1))              //���ǰ��ǰ�Һ��⴫������û������
				{ 
          runActionGroup(61,1);    //����ǰ���ĵ�һ��
          step = 1;                            //����ת�Ƶ�����1
          delay_ms(300);   
        }
				else                     //���ǰ���ǰ������һ���ź��⴫����û�м�⵽����
				{                               
          lastStep = 0;                        //����踳ֵΪ0
          step = 3;                            //ת�Ƶ�����3
          delay_ms(200);             //��ʱ200����
        }
      }
      break;   //����switch���
    case 1:    //����1
      if (!actiongroup_flag) {   //���û�ж�����������
        if ((LIGHT0) && (LIGHT1)) {  //���ǰ��ǰ�Һ��⴫������û������
          runActionGroup(81, 1);    //����ǰ���ĵڶ���
          step = 0;                            //ת�Ƶ�����0
          delay_ms(300);             //��ʱ300����
        } else {                               //���ǰ���ǰ������һ���ź��⴫����û�м�⵽����
          lastStep = 1;                        //����踳ֵΪ1
          step = 3;                            //ת�Ƶ�����3
          delay_ms(200);              //��ʱ200����
        }
      }
      break;
    case 3:  //����3
      stopActionGroup();      //����ֹͣ����������
      step = 4;                          //ת�Ƶ�����4
      delay_ms(200);             //��ʱ200����
      break;                             //����switch���
    case 4:  //����4
      if (lastStep == 0) {                //��������Ϊ0
        runActionGroup(71, 1); //����81�Ŷ����飬81Ϊǰ��1��71�Ŷ�����ķ�����
      }
      if (lastStep == 1) {                //��������Ϊ1
        runActionGroup(51, 1); //����61�Ŷ����飬61Ϊǰ��2��51�Ŷ�����ķ�����
      }
      delay_ms(300);      //��ʱ300����
      step = 5;                   //ת�Ƶ�����5
      break;                      //����switch���
    case 5:
      if (!actiongroup_flag) {  //���û�ж�����������
        runActionGroup(2, 1); //����2�Ŷ�����2�Σ�2��Ϊ���˶���
      }
      delay_ms(300);     //��ʾ600������ִ��
      step = 6;                  //ת�Ƶ�����6
      break;                     //����switch���
    case 6:
      if (!actiongroup_flag) { //���û�ж�����������
        if (lastStep == 0) {       //��������Ϊ0
          runActionGroup(4, 7); //����4�Ŷ�����7�Σ�4�Ŷ�����Ϊ��ת����,�ɸ��������޸�
        }
        if (lastStep == 1) {      //��������Ϊ1
          runActionGroup(3, 7);  //����3�Ŷ�����7�Σ� 3�Ŷ�����Ϊ��ת����
        }
        lastStep = 0; //����踴λ0
        step = 0;     //����ת�ƻ�0
      }
      delay_ms(300);  //��ʱ300����
      break;          //����switch���
	}
}

//��������Ѳ��ģʽ
void light_test(void)
{
	LCD_Clear(BLACK);
  POINT_COLOR=BROWN;	
	Show_Str(50,150,240,24, "��ֹ����ģʽ",24,1);	
	POINT_COLOR=GRAY;	
	LCD_DrawRectangle(0,180+92,180+240,180+140);
	Show_Str(70,320-40,240,24, "������һҳ",24,1);		
	while(1)
	{
   	light_handle();
    if (intel_window_back()) 	break;
	}
}

