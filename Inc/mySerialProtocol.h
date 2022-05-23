/*
 * mySerialProtocol.h
 *
 *  Created on: May 6, 2022
 *      Author: nemet
 */

#ifndef INC_MYSERIALPROTOCOL_H_
#define INC_MYSERIALPROTOCOL_H_

typedef struct mspFuncth
{
	uint8_t SBUS_rxBuffer[25];
	uint8_t oneCharBuffer;
	uint16_t elapsedTime;
	uint8_t no_char;
	myTimerType sbusTimer;
}mspFunct;

typedef struct mySerialProtocolth
{
	uint8_t number;
	struct UART_HandleTypeDef *Uart;
	struct mspFuncth func;
}mySerialProtocolt;


#define MAX_NO_MYSERIALPROTOCOLS 2
uint8_t no_mySerialProtocols;
sbust *mySerialProtocols[MAX_NO_MYSERIALPROTOCOLS+1];

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

#endif /* INC_MYSERIALPROTOCOL_H_ */
