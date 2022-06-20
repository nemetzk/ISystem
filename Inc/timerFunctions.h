#ifndef __TIMERFUNCTIONSH
#define __TIMERFUNCTIONSH

#include <stdint.h>
#include "stm32l4xx_hal.h"


typedef struct
{
	uint16_t number;
	uint16_t set_value;
	uint16_t cur_value;
	uint8_t Elapsed;
	 void (*Callback)(void);
}myTimerType;






void initTimer(myTimerType *Timer_ptr);
void TimerProgramCyclic(void);
void setTimer(myTimerType *Timer_ptr);
unsigned int seeTimer(unsigned char index);
//unsigned char timerElapsed(unsigned char index);
void TimerekSetup(TIM_HandleTypeDef *htim);

#endif

