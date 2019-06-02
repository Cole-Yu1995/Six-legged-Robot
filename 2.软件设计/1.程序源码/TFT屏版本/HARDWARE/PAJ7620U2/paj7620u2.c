#include "paj7620u2.h"
#include "paj7620u2_cfg.h"
#include "intel_window.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h"

//ѡ��PAJ7620U2 BANK����
void paj7620u2_selectBank(bank_e bank)
{
	switch(bank)
	{
		case BANK0: GS_Write_Byte(PAJ_REGITER_BANK_SEL,PAJ_BANK0);break;//BANK0�Ĵ�������
		case BANK1: GS_Write_Byte(PAJ_REGITER_BANK_SEL,PAJ_BANK1);break;//BANK1�Ĵ�������
	}	
}

//PAJ7620U2����
u8 paj7620u2_wakeup(void)
{ 
	u8 data=0x0a;
	GS_WakeUp();//����PAJ7620U2
	delay_ms(5);//����ʱ��>400us
	GS_WakeUp();//����PAJ7620U2
	delay_ms(5);//����ʱ��>400us
	paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
	data = GS_Read_Byte(0x00);//��ȡ״̬
	if(data!=0x20) return 0; //����ʧ��
	
	return 1;
}

//PAJ7620U2��ʼ��
//����ֵ��0:ʧ�� 1:�ɹ�
u8 paj7620u2_init(void)
{
	u8 i;
	u8 status;
	
	GS_i2c_init();//IIC��ʼ��
    status = paj7620u2_wakeup();//����PAJ7620U2
	if(!status) return 0;
	paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
	for(i=0;i<INIT_SIZE;i++)
	{
		GS_Write_Byte(init_Array[i][0],init_Array[i][1]);//��ʼ��PAJ7620U2
	}
    paj7620u2_selectBank(BANK0);//�л���BANK0�Ĵ�������
	
	return 1;
}

//����ʶ�����
void handleGesture(void)
{

	u8 time1;
	u8 i=0;
  u8 status;
	u8 data[2]={0x00};
	u16 gesture_data1;
	char gesture_send_buf[7]={0x55,0x55,0x05,0x06,0x00,0x01,0x00};
	
AGAIN1:	
	while(!paj7620u2_init());//PAJ7620U2��������ʼ�� 	
	paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
	for(i=0;i<GESTURE_SIZE;i++)
	{
		GS_Write_Byte(gesture_arry[i][0],gesture_arry[i][1]);//����ʶ��ģʽ��ʼ��
	}
	paj7620u2_selectBank(BANK0);//�л���BANK0�Ĵ�������

	time1=0;
	while(1)
	{
    status = GS_Read_nByte(PAJ_GET_INT_FLAG1,2,&data[0]);//��ȡ����״̬			
		if(!status)
		{   
			gesture_data1 =(u16)data[1]<<8 | data[0];
			if(gesture_data1) 
			{
				switch(gesture_data1)
				{
					case GES_UP: 
						   gesture_send_buf[4]=0x01; break; //����
					case GES_DOWM: 
						   gesture_send_buf[4]=0x02; break; //����
					case GES_LEFT: 
						   gesture_send_buf[4]=0x07; break; //����
					case GES_RIGHT: 
						   gesture_send_buf[4]=0x08; break; //����
					case GES_FORWARD: 
						   gesture_send_buf[4]=0x14; break; //��ǰ
					case GES_BACKWARD:  
							 gesture_send_buf[4]=0x19; break; //���
//					case GES_CLOCKWISE:
//						   gesture_send_buf[4]=0x01; break; //˳ʱ��
//					case GES_COUNT_CLOCKWISE: 
//						   gesture_send_buf[4]=0x01; break; //��ʱ��
					case GES_WAVE: 
						   gesture_send_buf[4]=0x0A; break; //�Ӷ�
					default: 
						   gesture_send_buf[4]=0x00; break;
				}	
        for(i=0;i<7;i++)
				{
					USART_SendData(USART1, gesture_send_buf[i]);//�򴮿�1��������
					while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
				}				
			}	
		}
		if (intel_window_back())
		{
			GS_Write_Byte(PAJ_SET_INT_FLAG1,0X00);//�ر�����ʶ���ж����
			GS_Write_Byte(PAJ_SET_INT_FLAG2,0X00);
			break;
		}	
		delay_ms(50);	 
    time1++;
    if (time1==100) goto AGAIN1;			
	}
}


void music_handleGesture() 
{
	u8 t,time2,i=0;
	u8 status;
	u8 data[2]={0x00};
	u16 gesture_data2;
	static char musicing=0xc8;
	static char music_send_buf[7]={0x55,0x55,0x05,0x06,0x00,0x01,0x00};
	char stop_send_buf[5]={0x55,0x55,0x02,0x07};

AGAIN2:	
	while(!paj7620u2_init());//PAJ7620U2��������ʼ�� 		
	paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
	for(i=0;i<GESTURE_SIZE;i++)
	{
		GS_Write_Byte(gesture_arry[i][0],gesture_arry[i][1]);//����ʶ��ģʽ��ʼ��
	}
	paj7620u2_selectBank(BANK0);//�л���BANK0�Ĵ�������
	
	time2=0;
	while(1)
	{
		if (intel_window_back())
		{
			for(t=0;t<4;t++)
			{
				USART_SendData(USART1, stop_send_buf[t]);//�򴮿�1��������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
			}
			GS_Write_Byte(PAJ_SET_INT_FLAG1,0X00);//�ر�����ʶ���ж����
			GS_Write_Byte(PAJ_SET_INT_FLAG2,0X00);
			break;
		}	
    status = GS_Read_nByte(PAJ_GET_INT_FLAG1,2,&data[0]);//��ȡ����״̬			
		if(!status)
		{  			
			gesture_data2 =(u16)data[1]<<8 | data[0];
			if(gesture_data2) 
			{
				switch(gesture_data2)
				{
					case GES_UP: 
							 musicing++;
							 if (musicing>220) musicing=0xc8;
							 music_send_buf[4]=musicing;
							 for(t=0;t<7;t++)
							 {
							   USART_SendData(USART1, music_send_buf[t]);//�򴮿�1��������
							 	 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
						   }
							 break;
					case GES_DOWM: 
						   musicing--;
							 if (musicing<200) musicing=0xDC;
							 music_send_buf[4]=musicing;
							 for(t=0;t<7;t++)
							 {
							   USART_SendData(USART1, music_send_buf[t]);//�򴮿�1��������
								 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
							 }
						   break;
					case GES_LEFT: 
						   for(t=0;t<4;t++)
							 {
							   USART_SendData(USART1, stop_send_buf[t]);//�򴮿�1��������
								 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
							 }
						break;
					case GES_RIGHT: 
						   for(t=0;t<7;t++)
							 {
							 	 USART_SendData(USART1, music_send_buf[t]);//�򴮿�1��������
								 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
							 }
						   break;
					default:break;
				}				
			}	
		}
		delay_ms(50);	   
		time2++;
    if (time2==100) goto AGAIN2;	
	}
}
