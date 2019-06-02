#include "common.h"
#include "stdlib.h"
#include "window.h"

//ESP8266 WIFI STA����
//���ڲ���TCP/UDP����
//����ֵ:0,����
//    ����,�������

u8 atk_8266_wifista_test(void)
{
	u8 j=0;
	char online_buf[5]={0xDA,0x00,0x03,0xFF,0xFF};
	u8 netpro=1;	//����ģʽ
	u8 key;
	u8 timex=0; 
	//u8 ipbuf[16]; 	//IP����
	u8 ipbuf[16]="45.43.222.110"; 	//IP����
	u8 *p;
	u16 t=999;		//���ٵ�һ�λ�ȡ����״̬
	u8 res=0;
	u16 rlen=0;
	u8 constate=0;	//����״̬
	p=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
	atk_8266_send_cmd("AT+CWMODE=1","OK",50);		//����WIFI STAģʽ
	atk_8266_send_cmd("AT+RST","OK",20);		//DHCP�������ر�(��APģʽ��Ч) 
	delay_ms(1000);         //��ʱ3S�ȴ������ɹ�
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	//�������ӵ���WIFI��������/���ܷ�ʽ/����,�⼸��������Ҫ�������Լ���·�������ý����޸�!! 
	sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//�������߲���:ssid,����
	while(atk_8266_send_cmd(p,"WIFI GOT IP",300));					//����Ŀ��·����,���һ��IP
PRESTA:
	delay_ms(1000);
	delay_ms(1000);
	netpro|=1;	//ѡ������ģʽ
	if(netpro&0X01)     //TCP Client    ͸��ģʽ����
	{
		LCD_Clear(BLACK);
		POINT_COLOR=BROWN;
		Show_Str_Mid(0,30,"ESP8266 WIFI-STA ����",16,240); 
		Show_Str(30,50,200,16,"��������ESP8266ģ��,���Ե�...",12,1);
		if(atk_8266_ip_set("WIFI-STA Զ��IP����",(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],(u8*)portnum,ipbuf)) //IP����
		{
			delay_ms(300);
			WINDOW_Init(0,180);
			delay_ms(300);
			return 1;	
		}
		atk_8266_send_cmd("AT+CIPMUX=0","OK",20);   //0�������ӣ�1��������
		sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",%s",ipbuf,(u8*)portnum);    //����Ŀ��TCP������
		while (atk_8266_send_cmd(p,"OK",200))
		{
			LCD_Clear(BLACK);
			POINT_COLOR=BROWN;
			Show_Str_Mid(0,40,"ESP8266 ����TCP Serverʧ��",12,240); //����ʧ��	
			POINT_COLOR=GRAY;
			LCD_DrawRectangle(0,180+92,180+240,180+140);
			Show_Str(70,320-40,240,24, "������ҳ",24,1);   
			for (j=0;j<20;j++) 
      {
        if (window_back()) 
				{
				  atk_8266_quit_trans();//�˳�͸��
		      atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  //�ر�͸��ģʽ	
					return 1; 
				}
				delay_ms(200);
			}
    }			

		atk_8266_send_cmd("AT+CIPMODE=1","OK",200);      //����ģʽΪ��͸��			
	}
	
	LCD_Clear(BLACK);
	POINT_COLOR=BROWN;
	Show_Str(0,30,240,12, "��Ʒ�����ڵ�Ƭ������������˿���ϵͳ���",12,1);
	Show_Str(0,50,200,12, "ѧУ�����ǽ���ѧ                     ",12,1);
	Show_Str(0,70,200,12, "ѧԺ���������е����ѧԺ               ",12,1);
	Show_Str(0,90,200,12, "�༶��14�������������ܻ�2��            ",12,1);
	Show_Str(0,110,200,12,"ѧ�����֣�������                          ",12,1);
	Show_Str(0,130,200,12,"ָ����ʦ�������                          ",12,1);
   
	LCD_Fill(0,150,239,80+12,BLACK);
	POINT_COLOR=BROWN;
	atk_8266_get_wanip(ipbuf);//������ģʽ,��ȡWAN IP
	sprintf((char*)p,"IP��ַ:%s �˿�:%s",ipbuf,(u8*)portnum);
	Show_Str(0,150,200,12,p,12,1);				//��ʾIP��ַ�Ͷ˿�	
	Show_Str(0,170,200,12,"״̬:",12,1); 		//����״̬
	Show_Str(90,170,200,12,"ģʽ:",12,1); 		//����״̬
	Show_Str(120,170,200,12,(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],12,1); 		//����״̬
	POINT_COLOR=GRAY;
	LCD_DrawRectangle(0,180+92,180+240,180+140);
	Show_Str(70,320-40,240,24, "������ҳ",24,1);	
	
	USART3_RX_STA=0;
	while(1)
	{
		t++;
		delay_ms(10);
		if(USART3_RX_STA&0X8000)		//���յ�һ��������
		{ 
			rlen=USART3_RX_STA&0X7FFF;	//�õ����ν��յ������ݳ���
			USART3_RX_BUF[rlen]=0;		//��ӽ����� 
			
		  DataTransfer((char*)USART3_RX_BUF,rlen-2);//�����յ������ݣ�����ת����16����ָ��
			USART3_RX_STA=0;
			//constate=atk_8266_consta_check();//�õ�����״̬
			if(constate!='+')t=1000;		//״̬Ϊ��δ����,������������״̬
			else t=0;                   //״̬Ϊ�Ѿ�������,10����ټ��
		}
		if(t==1000)//����10����û���յ��κ�����,��������ǲ��ǻ�����.
		{
			t=0;
			LCD_Fill(30,170,80,182,BLACK);
			constate=atk_8266_consta_check();//�õ�����״̬
			if(constate=='+')	
			{
				POINT_COLOR=BROWN;
				Show_Str(30,170,200,12,"���ӳɹ�",12,1);  //����״̬
			}
			else 
			{
				POINT_COLOR=GRAY;
				Show_Str(30,170,200,12,"����ʧ��",12,1); 
				delay_ms(200);
				atk_8266_quit_trans();//�˳�͸��
				atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  //�ر�͸��ģʽ	
				WINDOW_Init(0,180);
				delay_ms(200);
				myfree(SRAMIN,p);		//�ͷ��ڴ� 
				return 1; 
      }				
			//����������
			atk_8266_quit_trans();
			atk_8266_send_cmd("AT+CIPSEND","OK",20);         //��ʼ͸��  
			for (j=0;j<5;j++) 
			{  
				USART_SendData(USART3,online_buf[j]); 
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //ѭ������,ֱ��������� 
			}	
		}
		for (j=0;j<10;j++) 
		  if (window_back()) 
				{
					myfree(SRAMIN,p);		//�ͷ��ڴ� 
					return 1; 
				}
	}
	myfree(SRAMIN,p);		//�ͷ��ڴ� 
	return res;		
} 




























