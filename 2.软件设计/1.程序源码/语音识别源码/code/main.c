/***************************�����Ƶ���****************************
**  �������ƣ�YS-V0.7����ʶ��ģ����������
**	CPU: STC11L08XE
**	����22.1184MHZ
**	�����ʣ�9600 bit/S
**	���ײ�Ʒ��Ϣ��YS-V0.7����ʶ�𿪷���
**                http://yuesheng001.taobao.com
**  ���ߣ�zdings
**  ��ϵ��751956552@qq.com
**  �޸����ڣ�2013.9.13
**  ˵��������ģʽ�� ��ÿ��ʶ��ʱ����Ҫ˵��С�ܡ�������� �����ܹ�������һ����ʶ��
/***************************�����Ƶ���******************************/
#include "config.h"
/************************************************************************************/
//	nAsrStatus ������main�������б�ʾ�������е�״̬������LD3320оƬ�ڲ���״̬�Ĵ���
//	LD_ASR_NONE:		��ʾû������ASRʶ��
//	LD_ASR_RUNING��		��ʾLD3320������ASRʶ����
//	LD_ASR_FOUNDOK:		��ʾһ��ʶ�����̽�������һ��ʶ����
//	LD_ASR_FOUNDZERO:	��ʾһ��ʶ�����̽�����û��ʶ����
//	LD_ASR_ERROR:		��ʾһ��ʶ��������LD3320оƬ�ڲ����ֲ���ȷ��״̬
/***********************************************************************************/
uint8 idata nAsrStatus=0;	
void MCU_init(); 
void ProcessInt0(); //ʶ������
void delay(unsigned long uldata);
void User_handle(uint8 dat);//�û�ִ�в�������
void Delay200ms();
void Led_test(void);//��Ƭ������ָʾ
uint8_t G0_flag=DISABLE;//���б�־��ENABLE:���С�DISABLE:��ֹ���� 
sbit LED=P4^2;//�ź�ָʾ��

/***********************************************************
* ��    �ƣ� void  main(void)
* ��    �ܣ� ������	�������
* ��ڲ�����  
* ���ڲ�����
* ˵    ���� 					 
* ���÷����� 
**********************************************************/ 
void  main(void)
{
	uint8 idata nAsrRes;
	uint8 i=0;
	Led_test();
	MCU_init();
	LD_Reset();
	UartIni(); /*���ڳ�ʼ��*/
	nAsrStatus = LD_ASR_NONE;		//	��ʼ״̬��û������ASR
	
	while(1)
	{
		switch(nAsrStatus)
		{
			case LD_ASR_RUNING:
			case LD_ASR_ERROR:		
				break;
			case LD_ASR_NONE:
			{
				nAsrStatus=LD_ASR_RUNING;
				if (RunASR()==0)	/*	����һ��ASRʶ�����̣�ASR��ʼ����ASR��ӹؼ��������ASR����*/
				{
					nAsrStatus = LD_ASR_ERROR;
				}
				break;
			}
			case LD_ASR_FOUNDOK: /*	һ��ASRʶ�����̽�����ȥȡASRʶ����*/
			{				
				nAsrRes = LD_GetResult();		/*��ȡ���*/
				User_handle(nAsrRes);//�û�ִ�к��� 
				nAsrStatus = LD_ASR_NONE;
				break;
			}
			case LD_ASR_FOUNDZERO:
			default:
			{
				nAsrStatus = LD_ASR_NONE;
				break;
			}
		}// switch	 			
	}// while

}
/***********************************************************
* ��    �ƣ� 	 LED�Ʋ���
* ��    �ܣ� ��Ƭ���Ƿ���ָʾ
* ��ڲ����� �� 
* ���ڲ�������
* ˵    ���� 					 
**********************************************************/
void Led_test(void)
{
	LED=~ LED;
	Delay200ms();
	LED=~ LED;
	Delay200ms();
	LED=~ LED;
	Delay200ms();
	LED=~ LED;
	Delay200ms();
	LED=~ LED;
	Delay200ms();
	LED=~ LED;
}
/***********************************************************
* ��    �ƣ� void MCU_init()
* ��    �ܣ� ��Ƭ����ʼ��
* ��ڲ�����  
* ���ڲ�����
* ˵    ���� 					 
* ���÷����� 
**********************************************************/ 
void MCU_init()
{
	P0 = 0xff;
	P1 = 0xff;
	P2 = 0xff;
	P3 = 0xff;
	P4 = 0xff;
	

	LD_MODE = 0;		//	����MD�ܽ�Ϊ�ͣ�����ģʽ��д
	IE0=1;
	EX0=1;
	EA=1;
}
/***********************************************************
* ��    �ƣ�	��ʱ����
* ��    �ܣ�
* ��ڲ�����  
* ���ڲ�����
* ˵    ���� 					 
* ���÷����� 
**********************************************************/ 
void Delay200us()		//@22.1184MHz
{
	unsigned char i, j;
	_nop_();
	_nop_();
	i = 5;
	j = 73;
	do
	{
		while (--j);
	} while (--i);
}

void  delay(unsigned long uldata)
{
	unsigned int j  =  0;
	unsigned int g  =  0;
	while(uldata--)
	Delay200us();
}

void Delay200ms()		//@22.1184MHz
{
	unsigned char i, j, k;

	i = 17;
	j = 208;
	k = 27;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

/***********************************************************
* ��    �ƣ� �жϴ�����
* ��    �ܣ�
* ��ڲ�����  
* ���ڲ�����
* ˵    ���� 					 
* ���÷����� 
**********************************************************/ 
void ExtInt0Handler(void) interrupt 0  
{ 	
	ProcessInt0();				
}

/********************��������ú���*************************/
void stopActionGroup()
{
	volatile  uint8_t buf[6];
	buf[0] = 0x55 ;
	buf[1] = 0x55 ;
	buf[2] = 2 ;
	buf[3] = 0x07 ;
	buf[4] = 0X0D;
	buf[5] = 0x0A;	
	PrintCom(buf, 6) ;
}
void runActionGroup(uint8_t numofAction, uint16_t Times)
{
	static volatile  uint8_t buf[9];
	buf[0] = 0x55 ;
	buf[1] = 0x55 ;
	buf[2] = 5 ;
	buf[3] = 0x06 ;
	buf[4] = (uint8_t)numofAction ;		
	buf[5] = GET_LOW_BYTE(Times) ;
	buf[6] = GET_HIGH_BYTE((uint16_t)Times) ;
  buf[7] = 0x0d;
	buf[8] = 0x0a;
	PrintCom(buf, 9) ;
	//PrintCom(buf,7) ;
	LED = !LED;
	Delay200ms();
	Delay200ms();
	Delay200ms();
}

/***********************************************************
* ��    �ƣ��û�ִ�к��� 
* ��    �ܣ�ʶ��ɹ���ִ�ж������ڴ˽����޸� 
* ��ڲ����� �� 
* ���ڲ�������
* ˵    ���� 					 
**********************************************************/
void 	User_handle(uint8 dat)
{
	 int	i=0;
	 if(0==dat)
	 {
			G0_flag=ENABLE;
		LED=0;
		PrintCom("ok\r\n",4); /*text.....*/
	 }
	 else if(ENABLE==G0_flag)
	 {	
			G0_flag=DISABLE;
			LED=1;
		  switch(dat)		   /*�Խ��ִ����ز���,�ͻ���ɾ��Printcom �����������滻Ϊ������Ҫ���ƵĴ���*/
			{
				case CODE_STOP:			/*���ͣ��*/
						 stopActionGroup();
						 break;
				case CODE_FORWARD:	 /*���ǰ����*/
						 runActionGroup(33,1);
						 for (i=0;i<10;i++) Delay200ms();
						 runActionGroup(1,3);
						 break;					
				case CODE_BACKWARD:		/*������ˡ�*/				
						 runActionGroup(34,1);
						 for (i=0;i<10;i++) Delay200ms();
						 runActionGroup(2,3);	
						 break;
				case CODE_LEFTROTATE:		/*�����ת��*/				
						 runActionGroup(35,1);
						 for (i=0;i<10;i++) Delay200ms();
						 runActionGroup(3,2);
						 break;
				case CODE_RIGHTROTATE:		/*�����ת��*/				
						 runActionGroup(36,1);
						 for (i=0;i<10;i++) Delay200ms();
						 runActionGroup(4,2);
						 break;
				case CODE_LEFT:		/*������ߡ�*/				
						 runActionGroup(37,1);
						 for (i=0;i<10;i++) Delay200ms();
						 runActionGroup(7,3);
						 break;
				case CODE_RIGHT:		/*������ߡ�*/				
						 runActionGroup(38,1);
						 for (i=0;i<10;i++) Delay200ms();
						 runActionGroup(8,3);
						 break;
				case CODE_DANCE:			/*��������衱*/
						 runActionGroup(43,1);
						 for (i=0;i<12;i++) Delay200ms();
						 runActionGroup(9,2);
						 break;
				case CODE_CANSING:	 /*����ᳪ����*/
						 runActionGroup(44,1);
						 break;
				case CODE_APPLESING:		/*�����Сƻ����*/				
						 runActionGroup(45,1);  
						 for (i=0;i<13;i++) Delay200ms();
						 runActionGroup(92,1);
						 break;
				case CODE_COMEBACKSING:		/*�������������*/				
						 runActionGroup(46,1); 
						 for (i=0;i<20;i++) Delay200ms();
						 runActionGroup(91,1);
						 break;	
				case CODE_TENYEARSSING:		/*�����ʮ�ꡱ*/				
						 runActionGroup(47,1); 
						 for (i=0;i<16;i++) Delay200ms();
						 runActionGroup(97,1);
						 break;		
				case CODE_CHINASING:		/*�����һ���Ұ����й���*/				
						 runActionGroup(48,1); 
						 for (i=0;i<20;i++) Delay200ms();
						 runActionGroup(98,1);
						 break;			
				case CODE_INTUODUCE:		/*������ҽ��ܡ�*/				
						 runActionGroup(49,1);
						 break;	
				case CODE_STANDUP:		/*���������*/				
						 runActionGroup(99,1);
						 break;	
				case CODE_NAME:		/*������ʲô���֡�*/				
						 runActionGroup(38,1);
						 break;		
				case CODE_CANDANCE:		/*�����������*/				
						 runActionGroup(42,1);
						 break;	
				case CODE_WEATHER:		/*���������ô����*/				
						 runActionGroup(39,1);
						 break;
				case CODE_SING:		/*�����һ�׸衱*/				
						 runActionGroup(98,1);
						 break;		
				default:								
						 runActionGroup(37,1);
						 break;
			}	
		}	
		else 	
		{
			PrintCom("��˵��һ������\r\n",16); /*text.....*/	
		}
}	 
