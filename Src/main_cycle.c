/*
 * main_cycle.c
 *
 *  Created on: Jun 27, 2022
 *      Author: nemet
 */
#include "timerFunctions.h"
#include "main.h"
myTimerType  test_timer;


void timerCallback(void)
{
	HAL_GPIO_TogglePin(D3_GPIO_Port, D3_Pin);
	setTimer(&test_timer);
}

void main_cycle_setup (void)
{
	test_timer.set_value=30;
	test_timer.Callback=timerCallback;
	initTimer(&test_timer);
	test_timer.Enabled=2;
	setTimer(&test_timer);
}
