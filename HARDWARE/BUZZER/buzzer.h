#ifndef __BUZZER_H
#define __BUZZER_H

#include "sys.h"

void buzzer_init(void);
void buzzer_set(u8 select);
#define CIASD3 PBout(12)	  		//·äÃùÆ÷
#endif
