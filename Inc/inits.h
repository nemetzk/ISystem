/*
 * inits.h
 *
 *  Created on: May 6, 2022
 *      Author: nemet
 */

#ifndef INC_INITS_H_
#define INC_INITS_H_

#include "Flash_L4.h"
#include "dataStorage.h"
#include "timerFunctions.h"
#include "mySerialProtocol.h"

struct dataStorageth settingsData;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

#endif /* INC_INITS_H_ */
