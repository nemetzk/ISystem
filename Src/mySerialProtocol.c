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

int binary_converter(char binary[], int length)
{
	int decimal = 0;
	int position = 0;
	int index = length - 1;
	while (index >= 0)
	{
		decimal = decimal + (binary[index] - 48) * pow(10, position);
		index--;
		position++;
	}
	return decimal;
}


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

void tCommand (struct mySerialProtocolth *msp)
{
	char msg[50]; //Ebbe a tömbbe többet akartunk tölteni mint amennyi fér így mindig kifagytunk !!!
	HAL_RTC_GetTime(msp->func.hrtc,  &msp->func.sTime, RTC_FORMAT_BCD);
	HAL_RTC_GetDate(msp->func.hrtc,  &msp->func.sDate, RTC_FORMAT_BCD);
	sprintf (msg,"current time: %i:%i:%i\r\n", msp->func.sTime.Hours, msp->func.sTime.Minutes,msp->func.sTime.Seconds);
    myPrintf(msp->func.Uart,msg);
}

void TCommand (struct mySerialProtocolth *msp)
{
	char msg[50]; //Ebbe a tömbbe többet akartunk tölteni mint amennyi fér így mindig kifagytunk !!!
	int address = 0;
	int value = 0;
	HAL_RTC_GetTime(msp->func.hrtc,  &msp->func.sTime, RTC_FORMAT_BCD);
	HAL_RTC_GetDate(msp->func.hrtc,  &msp->func.sDate, RTC_FORMAT_BCD);
	myPrintf(msp->func.Uart,msg);
	address = binary_converter(msp->currentInFrame.Address,3);
	value = binary_converter(msp->currentInFrame.Value,3);
	sprintf(msg, "command:%c   address:%c%c%c  value:%c%c%c \n", msp->currentInFrame.Command , msp->currentInFrame.Address[0], msp->currentInFrame.Address[1], msp->currentInFrame.Address[2], msp->currentInFrame.Value[0], msp->currentInFrame.Value[1], msp->currentInFrame.Value[2]);
	myPrintf(msp->func.Uart,msg);
	sprintf(msg, "command:%c   address:%i  value:%i \r\n",msp->currentInFrame.Command ,address, value);
	myPrintf(msp->func.Uart,msg);
	switch (address)
	{
	case 1: //hour
		  msp->func.sTime.Hours = value;
		  if (HAL_RTC_SetTime(msp->func.hrtc, &(msp->func.sTime), RTC_FORMAT_BCD) != HAL_OK)
		  {
		    Error_Handler();
		  }
		  sprintf (msg,"current time: %i:%i:%i\r\n", msp->func.sTime.Hours, msp->func.sTime.Minutes,msp->func.sTime.Seconds);
		  myPrintf(msp->func.Uart,msg);
	break;
	case 2: //minute
		  msp->func.sTime.Minutes = value;
		  if (HAL_RTC_SetTime(msp->func.hrtc, &(msp->func.sTime), RTC_FORMAT_BCD) != HAL_OK)
		  {
		    Error_Handler();
		  }
		  sprintf (msg,"current time: %i:%i:%i\r\n", msp->func.sTime.Hours, msp->func.sTime.Minutes,msp->func.sTime.Seconds);
		  		  myPrintf(msp->func.Uart,msg);

	break;
	}//switch
}

void rCommand (struct mySerialProtocolth *msp)
{
	char msg[50]; //Ebbe a tömbbe többet akartunk tölteni mint amennyi fér így mindig kifagytunk !!!
	int address = 0;
	int value = 0;
	myPrintf(msp->func.Uart,msg);
	address = binary_converter(msp->currentInFrame.Address,3);
	sprintf(msg, "command:%c   address:%c%c%c\n", msp->currentInFrame.Command , msp->currentInFrame.Address[0], msp->currentInFrame.Address[1], msp->currentInFrame.Address[2]);
	if (address>0 && address<255)
	{
		datastorageLoad(msp->func.dataStorage);
		value = msp->func.dataStorage->Flashmemory.Rx_Data[address];
		sprintf (msg,"dataStorage[%i]= %i\r\n", address, value);
		myPrintf(msp->func.Uart,msg);
	}//if
}

void wCommand (struct mySerialProtocolth *msp)
{
	char msg[50]; //Ebbe a tömbbe többet akartunk tölteni mint amennyi fér így mindig kifagytunk !!!
	int address = 0;
	int32_t res=0;
	int16_t value = 0;
	address = binary_converter(msp->currentInFrame.Address,3);
	value = binary_converter(msp->currentInFrame.Value,3);
	sprintf(msg, "command:%c   address:%c%c%c  value:%c%c%c \r\n", msp->currentInFrame.Command , msp->currentInFrame.Address[0], msp->currentInFrame.Address[1], msp->currentInFrame.Address[2], msp->currentInFrame.Value[0], msp->currentInFrame.Value[1], msp->currentInFrame.Value[2]);
	myPrintf(msp->func.Uart,msg);
	sprintf(msg, "command:%c   address:%i  value:%i \r\n",msp->currentInFrame.Command ,address, value);
	myPrintf(msp->func.Uart,msg);

	if (address == 0)
	{
		if (value == 1)
		{
			datastorageInit(msp->func.dataStorage);
			datastorageSave(msp->func.dataStorage);
			datastorageLoad(msp->func.dataStorage);
			myPrintf(msp->func.Uart,"Factory defaults has been set!\n");
		}
	} //if

	if (address>=1 && address<127)
	{
		msp->func.dataStorage->Flashmemory.Tx_Data[address] = value;
		res=datastorageSave(msp->func.dataStorage);
		sprintf(msg,"dataStorage result: %i!\n",res);
		myPrintf(msp->func.Uart,msg);
	} //if


}//void

void xCommand (struct mySerialProtocolth *msp)
{
}

void yCommand (struct mySerialProtocolth *msp)
{
}

void YCommand (struct mySerialProtocolth *msp)
{
	char msg[50]; //Ebbe a tömbbe többet akartunk tölteni mint amennyi fér így mindig kifagytunk !!!
	int address = 0;
	int value = 0;
	address = binary_converter(msp->currentInFrame.Address,3);
	value = binary_converter(msp->currentInFrame.Value,3);

	sprintf(msg, "command:%c   address:%c%c%c  value:%c%c%c \r\n", msp->currentInFrame.Command , msp->currentInFrame.Address[0], msp->currentInFrame.Address[1], msp->currentInFrame.Address[2], msp->currentInFrame.Value[0], msp->currentInFrame.Value[1], msp->currentInFrame.Value[2]);
	myPrintf(msp->func.Uart,msg);
	sprintf(msg, "command:%c   address:%i  value:%i \r\n",msp->currentInFrame.Command ,address, value);
	myPrintf(msp->func.Uart,msg);

	switch (address)
	{
	case 1:
		if (value == 1)
		{
			HAL_GPIO_WritePin(D3_GPIO_Port, D3_Pin, GPIO_PIN_SET);
			myPrintf(msp->func.Uart,"D3_ON\n");

		}

		if (value == 0)
		{
			HAL_GPIO_WritePin(D3_GPIO_Port, D3_Pin, GPIO_PIN_RESET);
			myPrintf(msp->func.Uart,"D3_OFF\n");
		}

	break;


	case 2:
		if (value == 1)
		{
			HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, GPIO_PIN_SET);
			HAL_UART_Transmit(msp->func.Uart, "D4_ON", 6, 1000);
		}

		if (value == 0)
		{
			HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, GPIO_PIN_RESET);
			HAL_UART_Transmit(msp->func.Uart, "D4_OFF", 6, 1000);
		}

	break;

	case 3:
		if (value == 1)
		{
			HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, GPIO_PIN_SET);
			HAL_UART_Transmit(msp->func.Uart, "D5_ON", 6, 1000);
		}

		if (value == 0)
		{
			HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, GPIO_PIN_RESET);
			HAL_UART_Transmit(msp->func.Uart, "D5_OFF", 6, 1000);
		}
	break;

	case 4:
		if (value == 1)
		{
			HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, GPIO_PIN_SET);
			HAL_UART_Transmit(msp->func.Uart, "D6_ON", 6, 1000);
		}

		if (value == 0)
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
 case 't':
	 tCommand(msp);
 break;
 case 'T':
	 TCommand(msp);
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

					if (mySerialProtocols[eidx]->currentInFrame.Command == 't')
						mySerialProtocols[eidx]->func.protocol_st=  ST_WF_ENDCHAR;
					else
						mySerialProtocols[eidx]->func.protocol_st= ST_WF_ADDRESS_1;
					break;

				case ST_WF_ADDRESS_1:
					mySerialProtocols[eidx]->currentInFrame.Address[0] =mySerialProtocols[eidx]->func.oneCharBuffer;
					setTimer(&mySerialProtocols[eidx]->func.serialTimeOutTimer);
					mySerialProtocols[eidx]->func.protocol_st=  ST_WF_ADDRESS_2;
					break;

				case ST_WF_ADDRESS_2:
						mySerialProtocols[eidx]->currentInFrame.Address[1] =mySerialProtocols[eidx]->func.oneCharBuffer;
						setTimer(&mySerialProtocols[eidx]->func.serialTimeOutTimer);
						mySerialProtocols[eidx]->func.protocol_st=  ST_WF_ADDRESS_3;
						break;

				case ST_WF_ADDRESS_3:
						mySerialProtocols[eidx]->currentInFrame.Address[2] =mySerialProtocols[eidx]->func.oneCharBuffer;
						setTimer(&mySerialProtocols[eidx]->func.serialTimeOutTimer);

						if (mySerialProtocols[eidx]->currentInFrame.Command == 'r')
							mySerialProtocols[eidx]->func.protocol_st=  ST_WF_ENDCHAR;
						else
							mySerialProtocols[eidx]->func.protocol_st=  ST_WF_VALUE_1;
						break;

				case ST_WF_VALUE_1:
					mySerialProtocols[eidx]->currentInFrame.Value[0] =mySerialProtocols[eidx]->func.oneCharBuffer;
					setTimer(&mySerialProtocols[eidx]->func.serialTimeOutTimer);
					mySerialProtocols[eidx]->func.protocol_st=  ST_WF_VALUE_2;
					break;
				case ST_WF_VALUE_2:
					mySerialProtocols[eidx]->currentInFrame.Value[1] =mySerialProtocols[eidx]->func.oneCharBuffer;
					setTimer(&mySerialProtocols[eidx]->func.serialTimeOutTimer);
					mySerialProtocols[eidx]->func.protocol_st=  ST_WF_VALUE_3;
					break;
				case ST_WF_VALUE_3:
					mySerialProtocols[eidx]->currentInFrame.Value[2] =mySerialProtocols[eidx]->func.oneCharBuffer;
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
