#include "sys.h"
#include "delay.h"  
#include "usart.h"   
#include "lcd.h"
#include "key.h"  
#include "usmart.h"    
#include "malloc.h" 
#include "w25qxx.h"    
#include "sdio_sdcard.h"
#include "ff.h"  
#include "exfuns.h"    
#include "fontupd.h"
#include "text.h"	
#include "usmart.h"	
#include "touch.h"		
#include "usart3.h"
#include "common.h" 
#include "string.h"
#include "usart2.h"
#include "window.h"
#include "ble.h"
 
int main(void)
{	 
	u8 channel=0; 	    

	delay_init();	    	 //��ʱ������ʼ��	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600
	usart2_init(9600);		//��ʼ�����ڲ�����Ϊ9600
	usart3_init(115200);		//��ʼ������3 
	LCD_Init();			   		//��ʼ��LCD   
	W25QXX_Init();				//��ʼ��W25Q128
	tp_dev.init();				//��ʼ��������
 	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	exfuns_init();				//Ϊfatfs��ر��������ڴ�  
 	f_mount(fs[0],"0:",1); 		//����SD�� 
 	f_mount(fs[1],"1:",1); 		//����FLASH.
	font_init();			//����ֿ��Ƿ�OK
	
  WINDOW_Init(0,180);          //����������ʾ����
	BLE_Init();         //������ʼ�� 
	
	while (1)
	{
		channel=window_get_keynum(0,180);
		switch (channel)
		{
			case 1:delay_ms(300);atk_8266_test();	break;	//����ģʽһ��WIFIģʽ
			case 2:delay_ms(300);ble_test(); OPEN_BLE=0;break; //����ģʽ��������ģʽ
	    case 3:delay_ms(300);intelligent_test(); break;//����ģʽ������������ģʽ
		}
	}
}


















