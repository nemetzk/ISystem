/*
 * Flash_L4.c
 *
 *  Created on: Apr 6, 2022
 *      Author: nemet
 */

#include "Flash_L4.h"
//#define FLASH_USER_START_ADDR   ADDR_FLASH_PAGE_127   /* Start @ of user Flash area */
//#define FLASH_USER_END_ADDR     ADDR_FLASH_PAGE_127 + FLASH_PAGE_SIZE - 1   /* End @ of user Flash area */





void Flash_Read(struct flashMemoryth *Flashm)
{
    int numofwords = (NUM_OF_DATAS_IN_STORAGE_ARRAY/sizeof(Flashm->Rx_Data[0]))+((NUM_OF_DATAS_IN_STORAGE_ARRAY%sizeof(Flashm->Rx_Data[0]))!=0);
	Flash_Read_Data (Flashm->address,Flashm->Rx_Data,numofwords);
	// Itt a dolog elvérzik ha nem int16_t -ről van szó
}


void Flash_Read_Data (uint32_t StartPageAddress, int16_t *RxBuf, uint16_t numberofwords)
{
	while (1)
	{
		// Itt a dolog elvérzik ha nem int16_t -ről van szó - tovább kell fejleszteni a programot
		*RxBuf = *(__IO int16_t *)StartPageAddress;
		StartPageAddress += 2;
		RxBuf++;
		if (!(numberofwords--)) break;
	}
}
void Convert_To_Str (uint32_t *Data, char *Buf)
{
	int numberofbytes = ((strlen((char *)Data)/4) + ((strlen((char *)Data) % 4) != 0)) *4;

	for (int i=0; i<numberofbytes; i++)
	{
		Buf[i] = Data[i/4]>>(8*(i%4));
	}
}


uint8_t bytes_temp[4];

void float2Bytes(uint8_t * ftoa_bytes_temp,float float_variable)
{
    union {
      float a;
      uint8_t bytes[4];
    } thing;

    thing.a = float_variable;

    for (uint8_t i = 0; i < 4; i++) {
      ftoa_bytes_temp[i] = thing.bytes[i];
    }

}

void Erase_Flash() {

	// HAL_FLASH_Unlock();
    /* Clear OPTVERR bit set on virgin samples */
	uint32_t PAGEError;
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_MASSERASE;
    EraseInitStruct.Banks = FLASH_BANK_BOTH;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK) {

    }
    HAL_FLASH_Lock();
//
}

uint32_t Flash_Write_Data (struct flashMemoryth *Flashm)
{
/*
 *  **************** Előkészítés ***************
 */
	uint32_t addr = Flashm->address;
	uint64_t *array_64;
	uint32_t PAGEError;
	uint32_t FirstPage;
	uint32_t NbOfPages;
	uint32_t BankNumber;
	array_64 = (uint64_t *)Flashm->Tx_Data;
	int numofwords = 0;

	/*Variable used for Erase procedure*/
	FLASH_EraseInitTypeDef EraseInitStruct;

	numofwords = (sizeof(Flashm->Tx_Data)/8)+((sizeof(Flashm->Tx_Data)%8)!=0);

	  /* Erase the user Flash area
	    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

	  /* Clear OPTVERR bit set on virgin samples */
	 // __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
	  /* Get the 1st page to erase */
	  FirstPage = GetPage(Flashm->address);
	  /* Get the number of pages to erase from 1st page */
	  NbOfPages = GetPage(Flashm->address+numofwords) - FirstPage + 1;
	  /* Get the bank */
	  BankNumber = GetBank(Flashm->address);
	  /* Fill EraseInit structure*/
	  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	  EraseInitStruct.Banks       = BankNumber;
	  EraseInitStruct.Page        = FirstPage;
	  EraseInitStruct.NbPages     = NbOfPages;


	  /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
	     you have to make sure that these data are rewritten before they are accessed during code
	     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
	     DCRST and ICRST bits in the FLASH_CR register. */
		 /* Unlock the Flash to enable the flash control register access *************/
	  HAL_FLASH_Unlock();
	  while (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
	  {
	    /*
	      Error occurred while page erase.
	      User can add here some code to deal with this error.
	      PAGEError will contain the faulty page and then to know the code error on this page,
	      user can call function 'HAL_FLASH_GetError()'
	    */
	    /* Infinite loop */
		 HAL_FLASH_GetError();

	  }
// * ************** Adatok írása a flash-be ***************

	  uint8_t sofar;

	   while (sofar<numofwords)
	   	   {

	   	     while (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr,array_64[sofar]) == HAL_OK)
	   	     {

	   	     }

   	    	 addr += 8;  // use StartPageAddress += 2 for half word and 8 for double word
   	    	 sofar++;
	   	   }

	   /* Lock the Flash to disable the flash control register access (recommended
	      to protect the FLASH memory against possible unwanted operation) *********/
	   HAL_FLASH_Lock();

	   return 0;
}
static uint32_t GetPage(uint32_t Addr)
{
  uint32_t page = 0;

  if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
  {
    /* Bank 1 */
    page = (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
  }
  else
  {
    /* Bank 2 */
    page = (Addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
  }

  return page;
}

/**
  * @brief  Gets the bank of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The bank of a given address
  */
static uint32_t GetBank(uint32_t Addr)
{
  return FLASH_BANK_1;
}

/**
  * @brief  Gets the bank of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The bank of a given address
  */

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 80000000
  *            HCLK(Hz)                       = 80000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_M                          = 1
  *            PLL_N                          = 40
  *            PLL_R                          = 2
  *            PLL_P                          = 7
  *            PLL_Q                          = 4
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */


