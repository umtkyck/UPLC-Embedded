/*
/  Filename: Globals.h	                                                    /
/  Feature:      					                                        /
/  Module:   	                                                            /
/  Author:	 Mesaflix						                                /
/                                                                           /
/                                                                           /
/  ******************** Ltd. owns the copyright in this                     /
/  and associated documents and all rights are reserved.                    /
/  This document and associated documents must not be                       /
/  used for any purpose other than that for which they                      /
/  are supplied and must not be copied in whole or in                       /
/  part, or disclosed to others without the prior                           /
/  written consent of Mesaflix Ltd.  Any copy                   			/
/  of this document made by any method must also contain a                  /
/  copy of this legend.                                                     /
/                                                                           /
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



typedef struct CntChannelsVals
{
	float CNT0_InitVal;
	float CNT0_Val;
	float CNT1_InitVal;
	float CNT1_Val;
	float CNT2_InitVal;
	float CNT2_Val;
	float CNT3_InitVal;
	float CNT3_Val;
	float CNT4_InitVal;
	float CNT4_Val;
	float CNT5_InitVal;
	float CNT5_Val;
	float CNT6_InitVal;
	float CNT6_Val;
	float CNT7_InitVal;
	float CNT7_Val;
	float CNT8_InitVal;
	float CNT8_Val;
	float CNT9_InitVal;
	float CNT9_Val;
	float CNT10_InitVal;
	float CNT10_Val;
	float CNT11_InitVal;
	float CNT11_Val;
	float CNT12_InitVal;
	float CNT12_Val;
	float CNT13_InitVal;
	float CNT13_Val;
	float CNT14_InitVal;
	float CNT14_Val;
}CntChannelsVals;

typedef struct CntChannels
{
	CntChannelsVals CNT0;
	CntChannelsVals CNT1;
	CntChannelsVals CNT2;
	CntChannelsVals CNT3;
	CntChannelsVals CNT4;
	CntChannelsVals CNT5;
	CntChannelsVals CNT6;
	CntChannelsVals CNT7;
	CntChannelsVals CNT8;
	CntChannelsVals CNT9;
	CntChannelsVals CNT10;
	CntChannelsVals CNT11;
	CntChannelsVals CNT12;
	CntChannelsVals CNT13;
	CntChannelsVals CNT14;
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
