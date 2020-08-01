/*
 * ProgParser.h
 *
 *  Created on: 23 нояб. 2019 г.
 *      Author: volodai
 */

#ifndef INC_PROGPARSER_H_
#define INC_PROGPARSER_H_

#include "main.h"
#include "Globals.h"
#include "Flasher.h"
#include <stdbool.h>

#define MAXIVAR			5		  //максимальное количество входов
#define MAXIOVAR		15   	  //максимальное количество X
#define MAXMVAR			15        //максимальное количество M
#define MAXTVAR			10        //максимальное количество таймеров
#define MAXCOIL			2         //максимальное количество выходов
#define MAXCNTVAR		15

#define AND				'A'
#define OR				'O'
#define NOT				'N'
#define OT				'V'
#define SET				'S'
#define RES				'R'
#define MOV				'B'
#define TF				'c'
#define TN				't'
#define CNTP			'+'
#define CNTN			'-'
#define CNTE			'='
#define DELIM			'|'
#define ACT_DELIM		'\r'

typedef struct Xs
{
	float *Input;
	uint8_t Condition;
	uint8_t ConditionValue;
	bool Value;
}Xs;

typedef struct Xparm
{
	uint8_t Inletter;
	uint8_t InNum;
	uint8_t xnum;
	uint8_t ccondition;
	bool percent;
	uint8_t value;
}Xparm;

typedef struct Timer_S
{
	bool En;
	bool Q;
	uint32_t Delay;
    uint32_t StartTick;
	uint8_t Type;
}Timer_S;

typedef struct CntDataStr
{
	float CntVal[MAXCNTVAR];
}CntDataStr;

extern Xs Xses[MAXIOVAR];
extern bool Mvars[MAXMVAR];
extern Timer_S Timers[MAXTVAR];
extern bool Coils[MAXCOIL];


extern void ParsersInit();
extern void ProgInit();
extern void ProcessIO();


#endif /* INC_PROGPARSER_H_ */
