/*
 * dataStorage.c
 *
 *  Created on: 2022. ápr. 12.
 *      Author: nemet
 */


#include "dataStorage.h"


datastorageInit(struct dataStorageth *myDatastorage)
{

 	  //FlashErase(&(myDatastorage->Flashmemory));
	  myDatastorage->Flashmemory.Tx_Data[0] = -111;
	  myDatastorage->Flashmemory.Tx_Data[1] = -222;
	  myDatastorage->Flashmemory.Tx_Data[2] = -333;
	  myDatastorage->Flashmemory.Tx_Data[3] = -444;
	  myDatastorage->Flashmemory.Tx_Data[4] = -555;
	  myDatastorage->Flashmemory.Tx_Data[5] = -666;
	  myDatastorage->Flashmemory.data = myDatastorage->Flashmemory.Tx_Data;
}

datastorageSave( struct dataStorageth *myDatastorage)
{
	Flash_Write_Data(&(myDatastorage->Flashmemory));
}

datastorageLoad( struct dataStorageth *myDatastorage)
{
	Flash_Read(&(myDatastorage->Flashmemory));
}
