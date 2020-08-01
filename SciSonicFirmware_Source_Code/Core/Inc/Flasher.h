/*
 * Flasher.h
 *
 *  Created on: Nov 20, 2019
 *      Author: volodai
 */

#ifndef INC_FLASHER_H_
#define INC_FLASHER_H_
#include "main.h"
#include "Globals.h"

extern bool FlasherInit();

extern void PC_BuffProcessing(RecData * Recved);

extern bool TestFlash();

#endif /* INC_FLASHER_H_ */
