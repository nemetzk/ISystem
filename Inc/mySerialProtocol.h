/*
 * mySerialProtocol.h
 *
 *  Created on: May 6, 2022
 *      Author: nemet
 */

#ifndef INC_MYSERIALPROTOCOL_H_
#define INC_MYSERIALPROTOCOL_H_

#include "timerFunctions.h"

#define NO_STARTCHAR 	0
#define NO_STATION	 	1
#define NO_COMMAND 		2
#define NO_ADDRESS 		3
#define NO_VALUE		4
#define NO_ENDCHAR		5

#define ST_WF_STARTCHAR 	1
#define ST_WF_STATION	 	2
#define ST_WF_COMMAND 		3
#define ST_WF_ADDRESS 		4
#define ST_WF_VALUE			5
#define ST_WF_ENDCHAR		6


typedef struct mspFuncth
{
	uint8_t protocol_st;
	uint8_t RxBuffer[25];
	uint8_t oneCharBuffer;
	uint16_t elapsedTime;
	uint8_t no_char;
	struct UART_HandleTypeDef *Uart;
	myTimerType serialTimeOutTimer;
}mspFunct;

typedef struct currentInFrameth
{
	uint8_t Station;
	char Command;
	uint8_t Address;
	uint8_t Value;
}currentInFramet;

typedef struct protocolth
{
	char startChar;
	char endChar;
	char readChar;
	char writeChar;
	char readInput;
	char writeOutput;
	char readOutput;



}protocolt;


typedef struct mySerialProtocolth
{
	uint8_t number;
	struct mspFuncth func;
	struct protocolth protocol;
	struct currentInFrameth currentInFrame;
}mySerialProtocolt;



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

#endif /* INC_MYSERIALPROTOCOL_H_ */
