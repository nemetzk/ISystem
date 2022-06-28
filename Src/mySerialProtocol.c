/*
 * mySerialProtocol.c
 *
 *  Created on: May 6, 2022
 *      Author: nemet
 */
#include "main.h"
#include "mySerialProtocol.h"
#include "parameters.h"

currentInFramet nullFrame = {};

#define MFRAME msp->currentInFrame
#define MAX_NO_MYSERIALPROTOCOLS 2

uint8_t no_mySerialProtocols;

mySerialProtocolt *mySerialProtocols[MAX_NO_MYSERIALPROTOCOLS+1];


void resetComm(struct mySerialProtocolth *msp)
{
	msp->func.no_char = 0;
	msp->func.protocol_st= ST_WF_STARTCHAR;

}

void initMySerialProtocol(struct mySerialProtocolth *msp)
{
	uint8_t i;
	msp->number=++no_mySerialProtocols;
	mySerialProtocols[no_mySerialProtocols]=msp;
	// ************ protocol *************

	 msp->protocol.startChar = '&';
	 msp->protocol.endChar = '!';
	 /*
	 msp->protocol.readChar = 'r';
	 msp->protocol.writeChar = 'w';
	 msp->protocol.readInput = 'x';
	 msp->protocol.readOutput = 'y';
	 msp->protocol.writeOutput = 'Y';
*/
	 //******
	 msp->func.serialTimeOutTimer.Callback = resetComm;
	 msp->func.serialTimeOutTimer.ownerPtr = msp;
	initTimer(&msp->func.serialTimeOutTimer);
	msp->func.serialTimeOutTimer.set_value=100;

	HAL_UART_Receive_IT(msp->func.Uart, &(msp->func.oneCharBuffer), 1);
}


void rCommand (struct mySerialProtocolth *msp)
{
}

void wCommand (struct mySerialProtocolth *msp)
{
}

void xCommand (struct mySerialProtocolth *msp)
{
}

void yCommand (struct mySerialProtocolth *msp)
{
}

void YCommand (struct mySerialProtocolth *msp)
{
	switch (msp->currentInFrame.Address)
	{
	case '1':
		if (msp->currentInFrame.Value == 1)
		{
			HAL_GPIO_WritePin(D3_GPIO_Port, D3_Pin, GPIO_PIN_SET);
		}

		if (msp->currentInFrame.Value == 0)
				{
					HAL_GPIO_WritePin(D3_GPIO_Port, D3_Pin, GPIO_PIN_SET);
				}

	break;


	case '2':
		if (msp->currentInFrame.Value == 1)
		{
			HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, GPIO_PIN_SET);
		}

		if (msp->currentInFrame.Value == 0)
		{
			HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, GPIO_PIN_SET);
		}

	break;

	case '3':
		if (msp->currentInFrame.Value == 1)
		{
			HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, GPIO_PIN_SET);
		}

		if (msp->currentInFrame.Value == 0)
		{
			HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, GPIO_PIN_SET);
		}
	break;

	case '4':
		if (msp->currentInFrame.Value == 1)
		{
			HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, GPIO_PIN_SET);
		}

		if (msp->currentInFrame.Value == 0)
		{
			HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, GPIO_PIN_SET);
		}
	break;

	case '5':
	break;

	}
}

void parseFrame (struct mySerialProtocolth *msp)
{
 if (MFRAME.Address == MYADDRESS)
 {
 switch (MFRAME.Command)
 {
 case 'r':
	 rCommand(msp);
 break;
 case 'w':
	 wCommand(msp);
 break;
 case 'x':
	 xCommand(msp);
 break;
 case 'y':
	 yCommand(msp);
 break;
 case 'Y':
	 YCommand(msp);
 break;



 }//SWITCH
 }//IF
}//VOID

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	uint8_t eidx;
	uint8_t i;
	struct mySerialProtocolth *msp;



	for (eidx=1;eidx<=no_mySerialProtocols;eidx++)
		if ( huart == mySerialProtocols[eidx]->func.Uart)
		  {
			mySerialProtocols[eidx]->func.RxBuffer[mySerialProtocols[eidx]->func.no_char] = mySerialProtocols[eidx]->func.protocol_st;
			switch (mySerialProtocols[eidx]->func.protocol_st)
			{
				case ST_INIT:

					mySerialProtocols[eidx]->func.protocol_st = ST_WF_STARTCHAR;
				break;

				case ST_RESET:

					mySerialProtocols[eidx]->func.protocol_st= ST_WF_STARTCHAR;
					break;

				case ST_WF_STARTCHAR:
					mySerialProtocols[eidx]->currentInFrame = nullFrame;
					if (mySerialProtocols[eidx]->func.oneCharBuffer == mySerialProtocols[eidx]->protocol.startChar)
					{
						mySerialProtocols[eidx]->func.no_char++;
						mySerialProtocols[eidx]->func.protocol_st= ST_WF_STATION;
					}
					setTimer(&mySerialProtocols[eidx]->func.serialTimeOutTimer);
					break;

				case ST_WF_STATION:
						mySerialProtocols[eidx]->currentInFrame.Station =mySerialProtocols[eidx]->func.oneCharBuffer;
						mySerialProtocols[eidx]->func.no_char++;
						setTimer(&mySerialProtocols[eidx]->func.serialTimeOutTimer);
						mySerialProtocols[eidx]->func.protocol_st = ST_WF_COMMAND;
					break;

				case ST_WF_COMMAND:
					mySerialProtocols[eidx]->currentInFrame.Command =mySerialProtocols[eidx]->func.oneCharBuffer;
					mySerialProtocols[eidx]->func.no_char++;
					setTimer(&mySerialProtocols[eidx]->func.serialTimeOutTimer);
					mySerialProtocols[eidx]->func.protocol_st= ST_WF_ADDRESS;
					break;

				case ST_WF_ADDRESS:
					mySerialProtocols[eidx]->currentInFrame.Address =mySerialProtocols[eidx]->func.oneCharBuffer;
					mySerialProtocols[eidx]->func.no_char++;
					setTimer(&mySerialProtocols[eidx]->func.serialTimeOutTimer);
					mySerialProtocols[eidx]->func.protocol_st=  ST_WF_VALUE;
					break;

				case ST_WF_VALUE:
					mySerialProtocols[eidx]->currentInFrame.Value =mySerialProtocols[eidx]->func.oneCharBuffer;
					mySerialProtocols[eidx]->func.no_char++;
					setTimer(&mySerialProtocols[eidx]->func.serialTimeOutTimer);
					mySerialProtocols[eidx]->func.protocol_st=  ST_WF_ENDCHAR;
					break;

				case ST_WF_ENDCHAR:
					if (mySerialProtocols[eidx]->func.oneCharBuffer == mySerialProtocols[eidx]->protocol.endChar)
					//mySerialProtocols[eidx]->currentInFrame.Value =mySerialProtocols[eidx]->func.oneCharBuffer;
					{
						HAL_UART_Transmit(mySerialProtocols[eidx]->func.Uart, mySerialProtocols[eidx]->func.RxBuffer, 10, 1000);
						parseFrame(mySerialProtocols[eidx]);
						mySerialProtocols[eidx]->func.no_char=0;
						mySerialProtocols[eidx]->func.protocol_st=  ST_WF_STARTCHAR;
					}
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
