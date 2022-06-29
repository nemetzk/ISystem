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
myTimerType  test_timer;

mySerialProtocolt *mySerialProtocols[MAX_NO_MYSERIALPROTOCOLS+1];



void myPrintf(struct UART_HandleTypeDef *huart,char myMsg[30])
{
	HAL_UART_Transmit(huart, (uint8_t*)myMsg, strlen(myMsg), HAL_MAX_DELAY);
}

void timerCallback(void)
{
	HAL_GPIO_TogglePin(D3_GPIO_Port, D3_Pin);
	//setTimer(&test_timer);
}

void resetComm(struct mySerialProtocolth *msp)
{
	myPrintf(msp->func.Uart,"Unknown command!\n");
	msp->func.protocol_st= ST_WF_STARTCHAR;
	HAL_UART_Receive_IT(msp->func.Uart, &(msp->func.oneCharBuffer), 1);
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
	msp->func.serialTimeOutTimer.Enabled = 0;
	msp->func.serialTimeOutTimer.set_value=100;
	msp->func.protocol_st= ST_WF_STARTCHAR;


	test_timer.set_value=80;
	test_timer.Callback=timerCallback;
	initTimer(&test_timer);

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
	char msg[50]; //Ebbe a tömbbe többet akartunk tölteni mint amennyi fér így mindig kifagytunk !!!
	sprintf(msg, "command:%c   address:%c  value:%c \r\n",msp->currentInFrame.Command ,msp->currentInFrame.Address, msp->currentInFrame.Value);
	myPrintf(msp->func.Uart,msg);
	switch (msp->currentInFrame.Address)
	{
	case '1':
		if (msp->currentInFrame.Value == '1')
		{
			HAL_GPIO_WritePin(D3_GPIO_Port, D3_Pin, GPIO_PIN_SET);
			myPrintf(msp->func.Uart,"D3_ON\n");

		}

		if (msp->currentInFrame.Value == '0')
		{
			HAL_GPIO_WritePin(D3_GPIO_Port, D3_Pin, GPIO_PIN_RESET);
			myPrintf(msp->func.Uart,"D3_OFF\n");
		}

	break;


	case '2':
		if (msp->currentInFrame.Value == '1')
		{
			HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, GPIO_PIN_SET);
			HAL_UART_Transmit(msp->func.Uart, "D4_ON", 6, 1000);
		}

		if (msp->currentInFrame.Value == '0')
		{
			HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, GPIO_PIN_RESET);
			HAL_UART_Transmit(msp->func.Uart, "D4_OFF", 6, 1000);
		}

	break;

	case '3':
		if (msp->currentInFrame.Value == '1')
		{
			HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, GPIO_PIN_SET);
			HAL_UART_Transmit(msp->func.Uart, "D5_ON", 6, 1000);
		}

		if (msp->currentInFrame.Value == '0')
		{
			HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, GPIO_PIN_RESET);
			HAL_UART_Transmit(msp->func.Uart, "D5_OFF", 6, 1000);
		}
	break;

	case '4':
		if (msp->currentInFrame.Value == '1')
		{
			HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, GPIO_PIN_SET);
			HAL_UART_Transmit(msp->func.Uart, "D6_ON", 6, 1000);
		}

		if (msp->currentInFrame.Value == '0')
		{
			HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, GPIO_PIN_RESET);
			HAL_UART_Transmit(msp->func.Uart, "D6_OFF", 6, 1000);
		}
	break;


	}
}

void parseFrame (struct mySerialProtocolth *msp)
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

}//VOID

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	uint8_t eidx;
	uint8_t i;
	char msg[20];

	struct mySerialProtocolth *msp;
	for (eidx=1;eidx<=no_mySerialProtocols;eidx++)
		if ( huart == mySerialProtocols[eidx]->func.Uart)
		  {
			switch (mySerialProtocols[eidx]->func.protocol_st)
			{
				case ST_WF_STARTCHAR:
					mySerialProtocols[eidx]->currentInFrame = nullFrame;
					if (mySerialProtocols[eidx]->func.oneCharBuffer == mySerialProtocols[eidx]->protocol.startChar)
					{
						mySerialProtocols[eidx]->func.protocol_st= ST_WF_COMMAND;
					}
					setTimer(&mySerialProtocols[eidx]->func.serialTimeOutTimer);
					break;

				case ST_WF_COMMAND:
					mySerialProtocols[eidx]->currentInFrame.Command =mySerialProtocols[eidx]->func.oneCharBuffer;
					setTimer(&mySerialProtocols[eidx]->func.serialTimeOutTimer);
					mySerialProtocols[eidx]->func.protocol_st= ST_WF_ADDRESS;
					break;

				case ST_WF_ADDRESS:
					mySerialProtocols[eidx]->currentInFrame.Address =mySerialProtocols[eidx]->func.oneCharBuffer;
					setTimer(&mySerialProtocols[eidx]->func.serialTimeOutTimer);
					mySerialProtocols[eidx]->func.protocol_st=  ST_WF_VALUE;
					break;

				case ST_WF_VALUE:
					mySerialProtocols[eidx]->currentInFrame.Value =mySerialProtocols[eidx]->func.oneCharBuffer;
					setTimer(&mySerialProtocols[eidx]->func.serialTimeOutTimer);
					mySerialProtocols[eidx]->func.protocol_st=  ST_WF_ENDCHAR;
					break;

				case ST_WF_ENDCHAR:
					if (mySerialProtocols[eidx]->func.oneCharBuffer == mySerialProtocols[eidx]->protocol.endChar)
					{
						myPrintf(mySerialProtocols[eidx]->func.Uart,"COMMAND ACK!\r\n");
						parseFrame(mySerialProtocols[eidx]);
						mySerialProtocols[eidx]->func.protocol_st=  ST_WF_STARTCHAR;
					}
					break;
			}//switch
			sprintf(msg,"%c",mySerialProtocols[eidx]->func.oneCharBuffer);
			myPrintf(mySerialProtocols[eidx]->func.Uart,msg);
			HAL_UART_Receive_IT(huart, &(mySerialProtocols[eidx]->func.oneCharBuffer), 1);

		  }//if
}
