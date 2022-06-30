/*
 * alarm.c
 *
 *  Created on: Jun 30, 2022
 *      Author: nemet
 */

#include "main.h"



void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
  /* Turn LED2 on: Alarm generation */
	HAL_GPIO_WritePin(D3_GPIO_Port, D3_Pin, GPIO_PIN_SET);

	//setAlarm(sAlarm.AlarmTime.Hours,(++sAlarm.AlarmTime.Minutes));
}
