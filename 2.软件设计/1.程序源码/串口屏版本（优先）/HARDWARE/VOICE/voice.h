#ifndef __VOICE_H
#define __VOICE_H	 
#include "sys.h"

#define OPEN_VOICE PEout(5)// PF10

void VOICE_Init(void);
u8 voice_test(void);

#endif
