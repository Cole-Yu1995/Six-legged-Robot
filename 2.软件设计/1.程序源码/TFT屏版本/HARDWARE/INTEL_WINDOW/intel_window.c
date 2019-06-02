#include "intel_window.h"
//#include "window.h"

//���ؼ��̽��棨�ߴ�Ϊ240*140��
//x,y:������ʼ���꣨320*240�ֱ��ʵ�ʱ��x����Ϊ0��
void INTEL_WINDOW_Init(u16 x,u16 y)
{
	LCD_Clear(BLACK); 
	POINT_COLOR=GRAY;
	Show_Str(50,50,240,24, "�˻�����ģʽ",24,1);
	POINT_COLOR=BROWN;	
	LCD_DrawRectangle(x,y,x+240,y+46);						   	 
	LCD_DrawRectangle(x,y+46,x+240,y+92);
	LCD_DrawRectangle(x,y+92,x+240,y+140);
	LCD_DrawRectangle(x,y+140,x+240,y+140+42);
 	Show_Str(x+50,y+10,200,24, "����ʶ��ģʽ",24,1);	 			
	Show_Str(x+50,y+55,200,24, "����ʶ��ģʽ",24,1);	
	Show_Str(x+50,y+100,200,24, "���ֲ���ģʽ",24,1);	
	Show_Str(x+50,y+145,200,24, "��ֹ����ģʽ",24,1);	
	POINT_COLOR=GRAY;	
	LCD_DrawRectangle(0,180+92,180+240,180+140);
	Show_Str(70,320-40,240,24, "������ҳ",24,1);	
}

//����״̬����
//x,y:��������
//key:��ֵ��0~3��
//sta:״̬��0���ɿ���1�����£�
void intel_window_key_staset(u16 x,u16 y,u8 keyx,u8 sta)
{		  
	u16 i=keyx,j=keyx;
	if(sta)LCD_Fill(x+1,y+i*46+1,x+239,y+i*46+45,BROWN);
	else LCD_Fill(x+1,y+i*46+1,x+239,y+i*46+45,BLACK); 
  if (j==0) Show_Str(x+50,y+10,200,24, "����ʶ��ģʽ",24,1);	
  else if (j==1) Show_Str(x+50,y+55,200,24, "����ʶ��ģʽ",24,1);
	else if (j==2) Show_Str(x+50,y+100,200,24, "���ֲ���ģʽ",24,1);	
	else if (j==3) Show_Str(x+50,y+145,200,24, "����Ѳ��ģʽ",24,1);	
}

//�õ�������������
//x,y:��������
//����ֵ��������ֵ��1~3��Ч��0,��Ч��
u8 intel_window_get_keynum(u16 x,u16 y)
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
		}
		else if (tp_dev.x[0]<(x+240)&&tp_dev.x[0]>(x)&&tp_dev.y[0]<(y+140+42)&&tp_dev.y[0]>(y+140))
		{
			key=4;
		};
		
		if(key)
		{	   
			intel_window_key_staset(x,y,key-1,0);
			intel_window_key_staset(x,y,key-1,1);
		} 
	}
	return key; 
}

u8 intel_window_back(void)
{
	tp_dev.scan(0);
  if(tp_dev.sta&TP_PRES_DOWN)			//������������
	{	
		if (tp_dev.x[0]<240&&tp_dev.x[0]>0&&tp_dev.y[0]<320&&tp_dev.y[0]>272)
		{
			LCD_Fill(0,180+2*46+1,0+239,180+2*46+45,BROWN);
			Show_Str(65,320-40,240,24, "������һҳ",24,1);	
			delay_ms(300);
      INTEL_WINDOW_Init(0,90);
			return 1;
		};
	}
	return 0;
}
