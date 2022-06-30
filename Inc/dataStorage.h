/*
 * dataStorage.h
 *
 *  Created on: 2022. ápr. 12.
 *      Author: nemet
 */

#ifndef INC_DATASTORAGE_H_
#define INC_DATASTORAGE_H_

#include "Flash_L4.h"

/*
typedef struct rawValTH
{
	int16_t value;
	int16_t min;
	int16_t max;
	uint8_t outRangeFlg;
}rawValT;

typedef struct digValTH
{
	uint8_t calculationEnabled;
	uint8_t A;
	uint8_t B;
	uint8_t C;
	uint16_t Aval;
	uint16_t Bval;

}digValT;

typedef struct scaledValTH
{
	uint8_t calculationEnabled;
	uint32_t value;
	int16_t min;
	int16_t max;

}scaledValT;

typedef struct memoryAreath
{
	uint8_t num;
	struct rawValTH rawVal;
	struct digValTH digVal;
	struct scaledValTH scaledVal;
}memoryAreat;
*/
typedef struct dataStorageth
{
	struct flashMemoryth Flashmemory;
	int16_t memoryArea[NUM_OF_DATAS_IN_STORAGE_ARRAY+1];
	//struct memoryAreath memoryArea[NUM_OF_DATAS_IN_STORAGE_ARRAY];
};

datastorageInit(struct dataStorageth *myDatastorage);
datastorageSave(struct dataStorageth *myDatastorage);
datastorageLoad(struct dataStorageth *myDatastorage);

#endif /* INC_DATASTORAGE_H_ */
