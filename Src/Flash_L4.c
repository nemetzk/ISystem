/*
 * Flash_L4.c
 *
 *  Created on: Apr 6, 2022
 *      Author: nemet
 */

#include "Flash_L4.h"

void  FlashErase (struct flashMemoryth *FlashMem)
{
	/*
	uint32_t FirstPage = 0, NbOfPages = 0, BankNumber = 0;
	uint32_t Address = 0, PAGEError = 0;
	__IO uint32_t data32 = 0 , MemoryProgramStatus = 0;
*/

	  /* Unlock the Flash to enable the flash control register access *************/
	  HAL_FLASH_Unlock();

	  /* Erase the user Flash area
	    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

	  /* Clear OPTVERR bit set on virgin samples */
	  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
	  /* Get the 1st page to erase */
	  FlashMem->Private.FirstPage = GetPage(FLASH_USER_START_ADDR);
	  /* Get the number of pages to erase from 1st page */
	  FlashMem->Private.NbOfPages = GetPage(FLASH_USER_END_ADDR) - FlashMem->Private.FirstPage + 1;
	  /* Get the bank */
	  FlashMem->Private.BankNumber = GetBank(FLASH_USER_START_ADDR);
	  /* Fill EraseInit structure*/
	  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	  EraseInitStruct.Banks       = FlashMem->Private.BankNumber;
	  EraseInitStruct.Page        = FlashMem->Private.FirstPage;
	  EraseInitStruct.NbPages     = FlashMem->Private.NbOfPages;

	  /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
	     you have to make sure that these data are rewritten before they are accessed during code
	     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
	     DCRST and ICRST bits in the FLASH_CR register. */
	  if (HAL_FLASHEx_Erase(&EraseInitStruct, &FlashMem->Private.PAGEError) != HAL_OK)
	  {
	    /*
	      Error occurred while page erase.
	      User can add here some code to deal with this error.
	      PAGEError will contain the faulty page and then to know the code error on this page,
	      user can call function 'HAL_FLASH_GetError()'
	    */
	    /* Infinite loop */
	    while (1)
	    {
	      /* Make LED3 blink (100ms on, 2s off) to indicate error in Erase operation */

	    }
	  }

	  /* Program the user Flash area word by word
	    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

	  FlashMem->Private.Address = FLASH_USER_START_ADDR;
	  while (FlashMem->Private.Address < FLASH_USER_END_ADDR)
	  {
	    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, FlashMem->Private.Address, DATA_64) == HAL_OK)
	    {
	    	FlashMem->Private.Address = FlashMem->Private.Address + 8;
	    }
	   else
	    {
	      /* Error occurred while writing data in Flash memory.
	         User can add here some code to deal with this error */
	      while (1)
	      {
	        /* Make LED3 blink (100ms on, 2s off) to indicate error in Write operation */

	      }
	    }
	  }
	  //HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x0803F800, 0x1111111122222222);
	  /* Lock the Flash to disable the flash control register access (recommended
	     to protect the FLASH memory against possible unwanted operation) *********/
	  HAL_FLASH_Lock();

	  /* Check if the programmed data is OK
	      MemoryProgramStatus = 0: data programmed correctly
	      MemoryProgramStatus != 0: number of words not programmed correctly ******/
	  FlashMem->Private.Address = FLASH_USER_START_ADDR;
	  FlashMem->Private.MemoryProgramStatus = 0x0;

	  while (FlashMem->Private.Address < FLASH_USER_END_ADDR)
	  {
		  FlashMem->Private.data32 = *(__IO uint32_t *)FlashMem->Private.Address;

	    if (FlashMem->Private.data32 != DATA_32)
	    {
	    	FlashMem->Private.MemoryProgramStatus++;
	    }
	    FlashMem->Private.Address = FlashMem->Private.Address + 4;
	  }

}


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


uint32_t Flash_Write_Data (struct flashMemoryth *Flashm)
{
/*
 *  **************** Előkészítés ***************
 */
	uint32_t addr = Flashm->address;
	int64_t *array_64;
	array_64 = (int64_t *)Flashm->Tx_Data;
	int numofwords = 0;
	numofwords = (sizeof(Flashm->Tx_Data)/8)+((sizeof(Flashm->Tx_Data)%8)!=0);


/*
 *  ************* Flash törlése ****************
 */

	static FLASH_EraseInitTypeDef EraseInitStruct;
	//uint32_t PAGEError;
	int sofar=0;
    HAL_FLASH_Unlock();

	uint32_t StartPage = GetPage(Flashm->address);
	uint32_t EndPageAdress = Flashm->address + numofwords*8;
	uint32_t EndPage = GetPage(EndPageAdress);
/*
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
	Flashm->Private.FirstPage = GetPage(FLASH_USER_START_ADDR);
	Flashm->Private.NbOfPages = GetPage(FLASH_USER_END_ADDR) - Flashm->Private.FirstPage + 1;
	Flashm->Private.BankNumber = GetBank(FLASH_USER_START_ADDR);

 */
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
	Flashm->Private.FirstPage = StartPage;
	Flashm->Private.NbOfPages = EndPage -StartPage + 1;
	Flashm->Private.BankNumber = GetBank(Flashm->address);

	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.Banks       = Flashm->Private.BankNumber;
	EraseInitStruct.Page        = StartPage;
	EraseInitStruct.NbPages     = ((EndPage - StartPage)/FLASH_PAGE_SIZE) +1;
	Flashm->Private.PAGEError = 0;

	   if (HAL_FLASHEx_Erase( &EraseInitStruct, &Flashm->Private.PAGEError) != HAL_OK)
	   {
		   //return HAL_FLASH_GetError ();
	   }

// * ************** Adatok írása a flash-be ***************


	   while (sofar<numofwords)
	   	   {
	   	     if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr,array_64[sofar]) == HAL_OK)
	   	     {
	   	    	 addr += 8;  // use StartPageAddress += 2 for half word and 8 for double word
	   	    	 sofar++;
	   	     }
	   	     else
	   	     {
	   	       /* Error occurred while writing data in Flash memory*/
	   	    	 return HAL_FLASH_GetError ();
	   	     }
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

void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* MSI is enabled after System reset, activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}

