#ifndef __TIMERFUNCTIONSH
#define __TIMERFUNCTIONSH

#include <stdint.h>

#define TIMEREK_SZAMA           10
//  timer indexek
#define TESZT_TIMER             0
#define TIMER_ENC_DELAY			1
#define FELIRAT_TIMER           2
#define IS_MOVING_TIMER 		3
#define MEAS_SPD_TIMER 			4
#define  TESZT_TIME				5


typedef struct
{
	uint16_t number;
	uint16_t set_value;
	uint16_t cur_value;
	uint8_t Elapsed;
	 void (*Callback)(void);

}myTimerType;



unsigned int tidx ;
myTimerType *timer[TIMEREK_SZAMA+1];

void initTimer(myTimerType *Timer_ptr);
void TimerProgramCyclic(void);
void setTimer(myTimerType *Timer_ptr);
unsigned int seeTimer(unsigned char index);
//unsigned char timerElapsed(unsigned char index);
void TimerekSetup(void);

#endif

