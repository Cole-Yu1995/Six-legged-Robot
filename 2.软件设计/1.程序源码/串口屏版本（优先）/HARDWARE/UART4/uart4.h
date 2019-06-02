#ifndef __UART4_H
#define __UART4_H	 
#include "sys.h"  
   
extern u8 UART4_RX_BUF[64]; 		//���ջ���,���64���ֽ�
extern u8 UART4_RX_CNT;   			//���յ������ݳ���

//ģʽ����
//����봮���жϽ��գ��벻Ҫע�����º궨��
#define EN_UART4_RX 	1			//0,������;1,����.




void Uart4_Init(u32 bound);
void UART4_Send_Data(u8 *buf,u8 len);
void UART4_Receive_Data(u8 *buf,u8 *len);
#endif













