
#include "timerFunctions.h"
#include "stm32l4xx_hal.h"

TIM_HandleTypeDef htim6;
static unsigned char notimers = 0;

void TimerProgramCyclic(void)
{
	for ( tidx = 0 ; tidx <= notimers ; tidx++ )
	{
		if (timer[tidx]->cur_value == 1)
		{
			if (timer[tidx]->Callback != NULL)
				timer[tidx]->Callback();
		}
		if (timer[tidx]->cur_value > 0 )
		{
			timer[tidx]->cur_value-- ;
			timer[tidx]->Elapsed = 0x00;
		}
		else
		{
			timer[tidx]->Elapsed = 0xff;
		}

	}
}

void setTimer(myTimerType *Timer_ptr)
{
	Timer_ptr->cur_value=Timer_ptr->set_value;
}


unsigned int seeTimer(unsigned char index) {
	uint16_t xvar;
	xvar=timer[index]->cur_value;;
	return xvar;
}

/*
unsigned char timerElapsed(myTimerType *Timer_ptr) {
	return Timer_ptr->cur_value==0;
}
*/

void TimerekSetup(void){
	unsigned char timer_setup_idx;
	//timer = (myTimerType*)malloc(TIMEREK_SZAMA*sizeof(myTimerType));
	//for (timer_setup_idx=0;timer_setup_idx<TIMEREK_SZAMA;timer_setup_idx++) setTimer ( timer_setup_idx , 0 ) ;
	HAL_TIM_Base_Start_IT(&htim6);
}
/*
void TIM6_IRQHandler(void) { // Pass the control to HAL, which processes the IRQ
	HAL_TIM_IRQHandler(&htim6);
}*/

void initTimer(myTimerType *Timer_ptr) //void initTimer(myTimerType *Timer_ptr, void(*callBackFun)(void))
{
	/*
	 * kívül ezeket kell hozzárendelni - így kell deklarálni
	Timer_ptr->set_value=0;
	Timer_ptr->cur_value=0;
	Timer_ptr->Callback=callBackFun;
	*/
	Timer_ptr->number=++notimers;
	timer[notimers]=Timer_ptr;
	//seeTimer(1);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if(htim->Instance == TIM6){
		//HAL_GPIO_TogglePin(LD6_GPIO_Port, LD6_Pin);
		TimerProgramCyclic();
	}
}


