/*
 * Globals.h
 *
 *  Created on: Nov 20, 2019
 *      Author: volodai
 */

#ifndef INC_GLOBALS_H_
#define INC_GLOBALS_H_

#include "main.h"

#define COMMAND_PREFIX			'@'
#define ENTER_PROGRAMMING_MODE	'P'
#define CLOSE_PROGRAMMING_MODE	'E'
#define OK_ANSWER				'O'
#define ERR_ANSWER				'R'
#define BUSY_ANSWER				'B'
#define REPEAT_ANSWER		   	'T'
#define FirmVer					(uint8_t*)("v.1.0")

#define MAX_BUF 	100
typedef struct RecData
{
uint8_t Data[MAX_BUF];
uint32_t len;
}RecData;

typedef struct SData
{
uint8_t Data[10];
uint32_t len;
}SData;

typedef struct ADCChannels
{
	float FB1;  //0...100% от 3,3V
	float FB1p;  //0...100% от 3,3V
	float FB2;  //0...100% от 3,3V
	float FB2p;  //0...100% от 3,3V
	float CH1;
	float CH2;
	float CH3;
	float CH4;
	float CH5;
	float Vin;
	float Temp;
	float CH1p;
	float CH2p;
	float CH3p;
	float CH4p;
	float CH5p;
}ADCChannels;

typedef struct CntChannels
{
	float CNT0;
	float CNT1;
	float CNT2;
	float CNT3;
	float CNT4;
	float CNT5;
	float CNT6;
	float CNT7;
	float CNT8;
	float CNT9;
	float CNT10;
	float CNT11;
	float CNT12;
	float CNT13;
	float CNT14;
}CntChannels;

//----------------------------------------------------------------------
extern bool ProgrammingMode;
extern bool FlashIsProgrammed;
extern bool ProgInited;
extern bool BusyFlag;
extern RecData RecvData;
extern SData SendData;
extern ADCChannels Channels;
extern CntChannels CntChnls;
extern uint32_t ProgLen;
extern uint32_t PageAddr;
extern bool CareOuts;

extern uint8_t pageData[256];
extern uint16_t pageDataIndex;

//----------------------------------------------------------------------
extern void InitGlobals();



int datalen_cc;
int reclen_cc;
int pagedata_cc;
uint8_t received_cc[257];


#endif /* INC_GLOBALS_H_ */
