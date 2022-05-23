
#include "inits.h"

void main_init(void)
{
	  settingsData.Flashmemory.address = 0x0803f800;


	   //datastorageInit(&settingsData); //ezt inkább próbánál az első adatbetöltésnél kell használni egyszer
	   //datastorageSave(&settingsData);
	   datastorageLoad(&settingsData);

	   /* Segítség az STM32F4 vezérlő flash memória modul kidolgozásához
	    * https://controllerstech.com/flash-programming-in-stm32/
	    *
	    *
	    */

	   MSP1.sbusUart = &huart1;
	   MSP2.sbusUart = &huart2;
}
