#include "common.h"
#include "stdlib.h"
#include "view.h"
#include "uart4.h"

//ESP8266 WIFI STA����
//���ڲ���TCP/UDP����
//����ֵ:0,����
//    ����,�������

u8 atk_8266_wifista_test(void)
{
	u8 i,j=0;
	char online_buf[5]={0xDA,0x00,0x03,0xFF,0xFF};
	u8 netpro=1;	//����ģʽ
	//u8 IP_BUF[16]; 	//IP����
	u8 IP_BUF[16]="45.43.222.110"; 	//IP����
	u8 *p;
	u16 t=999;		//���ٵ�һ�λ�ȡ����״̬
	u16 rlen=0;
	u8 constate=0;	//����״̬
  
BEGIN1:
	p=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
	atk_8266_send_cmd("AT+CWMODE=1","OK",50);		//����WIFI STAģʽ
	atk_8266_send_cmd("AT+RST","OK",20);		//DHCP�������ر�(��APģʽ��Ч) 
	delay_ms(1000);         //��ʱ3S�ȴ������ɹ�
	delay_ms(1000);
	
	if (UART4_RX_CNT) 
	{	
		UART4_RX_CNT=0;
		if ((UART4_RX_BUF[0]==0xa5) && (UART4_RX_BUF[1]==0x5a))  //��ͨ��������
		{
			if ((UART4_RX_BUF[3]==0x83) && (UART4_RX_BUF[8]==0x04))
			{
				myfree(SRAMIN,p);		//�ͷ��ڴ� 
				usart_view(0x08);
				return 1;  
			}
		}
	}
	
	delay_ms(1000);
	delay_ms(1000);
	//�������ӵ���WIFI��������/���ܷ�ʽ/����,�⼸��������Ҫ�������Լ���·�������ý����޸�!! 
	sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//�������߲���:ssid,����
	while(atk_8266_send_cmd(p,"WIFI GOT IP",300))   //����Ŀ��·����,���һ��IP
	{
		if (UART4_RX_CNT) 
		{	
			UART4_RX_CNT=0;
      if ((UART4_RX_BUF[0]==0xa5) && (UART4_RX_BUF[1]==0x5a))  //��ͨ��������
			{
				if ((UART4_RX_BUF[3]==0x83) && (UART4_RX_BUF[8]==0x04))
				{
					myfree(SRAMIN,p);		//�ͷ��ڴ� 
					usart_view(0x08);
					return 1;  
				}
			}
		}
		delay_ms(10);
	}					

	delay_ms(1000);
	delay_ms(1000);
	netpro|=1;	//ѡ������ģʽ
	usart_run_view(0x05,2);
	usart_view(0x0b);
	usart_run_view(0x06,2);
	while (netpro&0X01)     //TCP Client    ͸��ģʽ����
	{
		if (UART4_RX_CNT) 
		{	
			delay_ms(100);
      if ((UART4_RX_BUF[0]==0xa5) && (UART4_RX_BUF[1]==0x5a))  //��ͨ��������
			{
				if (UART4_RX_BUF[3]==0x83)
				{
					if (UART4_RX_BUF[2]==0x06)
					{
						if (UART4_RX_BUF[8]==0x12)
						{
							myfree(SRAMIN,p);		//�ͷ��ڴ� 
							UART4_RX_CNT=0;
							usart_view(0x08);
							return 1;
						}
						else if (UART4_RX_BUF[8]==0x11)
						{
							UART4_RX_CNT=0;
							delay_ms(100);
							usart_run_view(0x06,1);
							atk_8266_send_cmd("AT+CIPMUX=0","OK",20);   //0�������ӣ�1��������
							sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",%s",IP_BUF,(u8*)PORTNUM);    //����Ŀ��TCP������
							while (atk_8266_send_cmd(p,"OK",200))
							{
								if (UART4_RX_CNT) 
								{	
									UART4_RX_CNT=0;
									if ((UART4_RX_BUF[0]==0xa5) && (UART4_RX_BUF[1]==0x5a))  //��ͨ��������
									{
										if ((UART4_RX_BUF[3]==0x83) && (UART4_RX_BUF[8]==0x12))
										{
											myfree(SRAMIN,p);		//�ͷ��ڴ� 
											usart_view(0x08);
											return 1;  
										}
									}
								}
							}
							atk_8266_send_cmd("AT+CIPMODE=1","OK",200);      //����ģʽΪ��͸��	
							delay_ms(100);
							usart_view(0x02);//���Ӵ���
							usart_run_view(0x06,2);
							goto BEGIN2;
						}
					}
					else 
					{
						UART4_RX_CNT=0;
						i=7;
						while (UART4_RX_BUF[i]!=0xff)
						{
              IP_BUF[i-7]=UART4_RX_BUF[i];
							i++;
						}					
					}
				}
			}
		  UART4_RX_CNT=0;
		}
    delay_ms(100);		
	}

BEGIN2:	
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
//			constate=atk_8266_consta_check();//�õ�����״̬
//			if(constate!='+')t=1000;		//״̬Ϊ��δ����,������������״̬
//			else t=0;                   //״̬Ϊ�Ѿ�������,10����ټ��
		}
		if(t==1000)//����10����û���յ��κ�����,��������ǲ��ǻ�����.
		{
			t=0;
			constate=atk_8266_consta_check();//�õ�����״̬
			if(constate=='+')	
			{
				usart_status_view(1); //����״̬�ɹ�
			}
			else 
			{
				usart_status_view(0); //����״̬ʧ��
      }				
			
			//����������
			atk_8266_quit_trans();
			atk_8266_send_cmd("AT+CIPSEND","OK",20);         //��ʼ͸��  
			for (j=0;j<5;j++) 
			{  
				//printf(online_buf);
				USART_SendData(USART3,online_buf[j]); 
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //ѭ������,ֱ��������� 
			}	
		}
		
		if (UART4_RX_CNT) 
		{	
			if (UART4_RX_BUF[8]==0x13)
			{
				myfree(SRAMIN,p);		//�ͷ��ڴ� 
				UART4_RX_CNT=0;
				usart_view(0x08);
				return 1;  
			}
		}
	}	
	myfree(SRAMIN,p);		//�ͷ��ڴ� 	
} 




























