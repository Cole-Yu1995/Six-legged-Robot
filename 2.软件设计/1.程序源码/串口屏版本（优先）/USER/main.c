#include "sys.h"
#include "delay.h"  
#include "key.h"    	
#include "common.h"
#include "usart.h"  
#include "usart2.h"
#include "usart3.h"
#include "uart4.h"
#include "ble.h"
#include "light.h"
#include "voice.h"
#include "music.h"
#include "gesture.h"

//�˻�����ģʽ
u8 intelligent_test(void)
{
	u8 p=0;
	VOICE_Init();
	LIGHT_Init();
	while (1)
	{
		if (UART4_RX_CNT) 
		{
			delay_ms(100);
      if ((UART4_RX_BUF[0]==0xa5) && (UART4_RX_BUF[1]==0x5a))  //��ͨ��������
			{
				if (UART4_RX_BUF[3]==0x83)
				{
					UART4_RX_CNT=0;
					switch (UART4_RX_BUF[8])
					{
						case 0x31:return 1; break;
						case 0x32:voice_test(); OPEN_VOICE=0; break;//����ģʽһ������ʶ��ģʽ
						case 0x33:gesture_test(); break; //����ģʽ��������ʶ��ģʽ
						case 0x34:music_test(); break;//����ģʽ�������ֲ���ģʽ
//						case 0x35:printf("lightok");light_test(); break;//����ģʽ�ģ���ֹ����ģʽ
					}
				}
			}
		  UART4_RX_CNT=0;
		}
	}
}

//������
int main(void)
{	 	    
  u8 t;
	delay_init();	    	 //��ʱ������ʼ��	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600
	usart2_init(9600);		//��ʼ�����ڲ�����Ϊ9600
	usart3_init(115200);		//��ʼ������3 
	uart4_init(115200);		//��ʼ������4
	
	BLE_Init();         //������ʼ�� 

	while(1)
	{	
		if (UART4_RX_CNT) 
		{	
			delay_ms(100);
			UART4_RX_CNT=0;
      if ((UART4_RX_BUF[0]==0xa5) && (UART4_RX_BUF[1]==0x5a))  //��ͨ��������
			{ 
				//oder_len=UART4_RX_BUF[2];   //ָ����ֽڳ��ȣ�������10����
				if (UART4_RX_BUF[3]==0x83)
				{
					//data_len=UART4_RX_BUF[6]*2;  //���ݵ��ֳ��ȣ�������16����
					switch (UART4_RX_BUF[8])
					{
						case 0x01:         //WIFIģʽ
							atk_8266_test();
							break;
						case 0x02:         //����ģʽ
							if (ble_test()==1) OPEN_BLE=0;
							break;
						case 0x03:         //�˻�����ģʽ
							intelligent_test(); 
							break;
					}
				}
			}
		}
	}
}


















