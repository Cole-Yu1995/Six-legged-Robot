#include "window.h"

//���ؼ��̽��棨�ߴ�Ϊ240*140��
//x,y:������ʼ���꣨320*240�ֱ��ʵ�ʱ��x����Ϊ0��
void WINDOW_Init(u16 x,u16 y)
{
	LCD_Clear(BLACK);
	POINT_COLOR=BROWN;
	Show_Str(0,30,240,12, "��Ʒ�����ڵ�Ƭ������������˿���ϵͳ���",12,1);
	Show_Str(0,50,200,12, "ѧУ�����ǽ���ѧ                     ",12,1);
	Show_Str(0,70,200,12, "ѧԺ���������е����ѧԺ               ",12,1);
	Show_Str(0,90,200,12, "�༶��14�������������ܻ�2��            ",12,1);
	Show_Str(0,110,200,12,"ѧ��������������                          ",12,1);
	Show_Str(0,130,200,12,"ָ����ʦ�������                            ",12,1);
	POINT_COLOR=GRAY;
	Show_Str(45,160,200,16,"��ͬ��������ģʽѡ��             ",16,1);
	LCD_DrawRectangle(x,y,x+240,y+46);						   	 
	LCD_DrawRectangle(x,y+46,x+240,y+92);
	LCD_DrawRectangle(x,y+92,x+240,y+140);
 	Show_Str(x+50,y+10,200,24, "Զ��WIFIģʽ",24,1);	 			
	Show_Str(x+50,y+55,200,24, "�̳�����ģʽ",24,1);	
	Show_Str(x+50,y+100,200,24, "�˻�����ģʽ",24,1);	
}

//����״̬����
//x,y:��������
//key:��ֵ��0~2��
//sta:״̬��0���ɿ���1�����£�
void window_key_staset(u16 x,u16 y,u8 keyx,u8 sta)
{		  
	u16 i=keyx,j=keyx;
	if(sta)LCD_Fill(x+1,y+i*46+1,x+239,y+i*46+45,BROWN);
	else LCD_Fill(x+1,y+i*46+1,x+239,y+i*46+45,BLACK); 
  if (j==0) Show_Str(x+50,y+10,200,24, "Զ��WIFIģʽ",24,1);	
  else if (j==1) Show_Str(x+50,y+55,200,24, "�̳�����ģʽ",24,1);
	else if (j==2) Show_Str(x+50,y+100,200,24, "�˻�����ģʽ",24,1);	
}

//�õ�������������
//x,y:��������
//����ֵ��������ֵ��1~3��Ч��0,��Ч��
u8 window_get_keynum(u16 x,u16 y)
{
//	static u8 key_x=0;//0,û���κΰ������£�1~3�Ű�������
	u8 key=0;
	tp_dev.scan(0); 		 
	if(tp_dev.sta&TP_PRES_DOWN)			//������������
	{	
		if(tp_dev.x[0]<(x+240)&&tp_dev.x[0]>(x)&&tp_dev.y[0]<(y+46)&&tp_dev.y[0]>(y))
		{	
			key=1;	 	   
		}
		else if (tp_dev.x[0]<(x+240)&&tp_dev.x[0]>(x)&&tp_dev.y[0]<(y+92)&&tp_dev.y[0]>(y+46))
		{
			key=2;
		}
		else if (tp_dev.x[0]<(x+240)&&tp_dev.x[0]>(x)&&tp_dev.y[0]<(y+140)&&tp_dev.y[0]>(y+92))
		{
			key=3;
		};
		
		if(key)
		{	   
			window_key_staset(x,y,key-1,0);
			window_key_staset(x,y,key-1,1);
		} 
	}
	return key; 
}

u8 window_back(void)
{
	tp_dev.scan(0);
  if(tp_dev.sta&TP_PRES_DOWN)			//������������
	{	
		if (tp_dev.x[0]<240&&tp_dev.x[0]>0&&tp_dev.y[0]<320&&tp_dev.y[0]>272)
		{
			LCD_Fill(0,180+2*46+1,0+239,180+2*46+45,BROWN);
			Show_Str(70,320-40,240,24, "������ҳ",24,1);	
			delay_ms(300);
      WINDOW_Init(0,180);
			return 1;
		};
	}
	return 0;
}
