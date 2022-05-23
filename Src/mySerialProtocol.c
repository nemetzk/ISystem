/*
 * mySerialProtocol.c
 *
 *  Created on: May 6, 2022
 *      Author: nemet
 */
#include "main.h"
#include "mySerialProtocol.h"

void initMySerialProtocol(struct mySerialProtocolth *msp)
{
	uint8_t i;
	msp->number=++no_mySerialProtocols;
	mySerialProtocols[no_mySerialProtocols]=msp;
	HAL_UART_Receive_IT(msp->Uart, &(msp->func.oneCharBuffer), 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	uint8_t eidx;
	struct mySerialProtocolth *msp;

	for (eidx=1;eidx<=no_sbuses;eidx++)
		if ( huart == mySerialProtocols[eidx]->Uart)
		  {

			if (sbuses[eidx]->func.sbusTimer.cur_value > 15)
			{
				sbuses[eidx]->func.no_char =0;
			}
			else
			{
				sbuses[eidx]->func.no_char++;
			}
			sbuses[eidx]->func.SBUS_rxBuffer[sbuses[eidx]->func.no_char]=sbuses[eidx]->func.oneCharBuffer;
			parseSbusMessage(sbuses[eidx],sbuses[eidx]->func.SBUS_rxBuffer);
			HAL_UART_Receive_IT(huart, &(sbuses[eidx]->func.oneCharBuffer), 1);
			setTimer(&(sbuses[eidx]->func.sbusTimer));
		  }
}
