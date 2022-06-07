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
	// ************ protocol *************

	 msp->protocol.startChar = '&';
	 msp->protocol.endChar = '!';
	 msp->protocol.readChar = 'r';
	 msp->protocol.writeChar = 'w';

	 //******

	HAL_UART_Receive_IT(msp->func.Uart, &(msp->func.oneCharBuffer), 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	uint8_t eidx;
	uint8_t i;
	struct mySerialProtocolth *msp;

	for (eidx=1;eidx<=no_mySerialProtocols;eidx++)
		if ( huart == mySerialProtocols[eidx]->func.Uart)
		  {
			switch (protocol_st)
			{
				case ST_INIT:

					protocol_st = ST_WF_STARTCHAR;
				break;

				case ST_RESET:
					mySerialProtocols[eidx]->currentInFrame = nullFrame;
					protocol_st = ST_WF_STARTCHAR;
					break;

				case ST_WF_STARTCHAR:
					if (mySerialProtocols[eidx]->func.oneCharBuffer == mySerialProtocols[eidx]->protocol.startChar)
					{
						mySerialProtocols[eidx]->func.no_char++;
						protocol_st = ST_WF_STATION;
					}
					break;

				case ST_WF_STATION:
						mySerialProtocols[eidx]->currentInFrame.Station =mySerialProtocols[eidx]->func.oneCharBuffer;
						mySerialProtocols[eidx]->func.no_char++;
						protocol_st = ST_WF_COMMAND;
					break;

				case ST_WF_COMMAND:
					mySerialProtocols[eidx]->currentInFrame.Command =mySerialProtocols[eidx]->func.oneCharBuffer;
					mySerialProtocols[eidx]->func.no_char++;
					protocol_st = ST_WF_ADDRESS;
					break;

				case ST_WF_ADDRESS:
					mySerialProtocols[eidx]->currentInFrame.Address =mySerialProtocols[eidx]->func.oneCharBuffer;
					mySerialProtocols[eidx]->func.no_char++;
					protocol_st =  ST_WF_VALUE;
					break;

				case ST_WF_VALUE:
					mySerialProtocols[eidx]->currentInFrame.Value =mySerialProtocols[eidx]->func.oneCharBuffer;
					mySerialProtocols[eidx]->func.no_char++;
					protocol_st =  ST_WF_VALUE;
					break;

				case ST_WF_ENDCHAR:
					if (mySerialProtocols[eidx]->func.oneCharBuffer == mySerialProtocols[eidx]->protocol.endChar)
					mySerialProtocols[eidx]->currentInFrame.Value =mySerialProtocols[eidx]->func.oneCharBuffer;
					mySerialProtocols[eidx]->func.no_char++;
					protocol_st =  ST_WF_VALUE;
					break;
			}
			HAL_UART_Receive_IT(huart, &(mySerialProtocols[eidx]->func.oneCharBuffer), 1);
			/**
			 *   Feladatok:
			 *   	- timer ami felügyeli a két karakter közti időt
			 *   	- ha az idő letelt ráküldi a reset folyamatra, elfelejti az eddig begyűjtött karaktereket és újra várni kezdi az elsőt
			 */


			/*


			sbuses[eidx]->func.SBUS_rxBuffer[sbuses[eidx]->func.no_char]=sbuses[eidx]->func.oneCharBuffer;
			parseSbusMessage(sbuses[eidx],sbuses[eidx]->func.SBUS_rxBuffer);
			HAL_UART_Receive_IT(huart, &(sbuses[eidx]->func.oneCharBuffer), 1);
			setTimer(&(sbuses[eidx]->func.sbusTimer));
			*/
		  }
}
