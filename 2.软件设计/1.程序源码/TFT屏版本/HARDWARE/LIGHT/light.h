#ifndef __LIGHT_H
#define __LIGHT_H	 
#include "sys.h"

#define GET_LOW_BYTE(A) (u8)((A))
#define GET_HIGH_BYTE(A) (u8)((A) >> 8)

#define LIGHT0  GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_0)//��ȡǰ�󴫸���
#define LIGHT1  GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_1)//��ȡǰ�Ҵ�����
#define LIGHT2  GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_2)//��ȡ���󴫸��� 
#define LIGHT3  GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_3)//��ȡ���Ҵ�����

void LIGHT_Init(void);
void light_test(void);

#endif
