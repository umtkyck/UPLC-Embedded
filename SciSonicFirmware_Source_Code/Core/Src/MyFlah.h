/*
 * MyFlah.h
 *
 *  Created on: Nov 24, 2019
 *      Author: volodai
 */

#ifndef SRC_MYFLAH_H_
#define SRC_MYFLAH_H_

extern void Flash_Init();
extern void Flash_WriteEnable(void);
extern void Flash_WriteDisable(void);
extern uint8_t Flash_ReadStatusRegister1();
extern uint8_t Flash_ReadStatusRegister2();
extern void Flash_WaitForWriteEnd(void);
extern void Flash_EraseChip();
extern void Flash_WritePage(uint8_t *pBuffer, uint32_t Page_Number, uint32_t NumByteToWrite_up_to_PageSize);
extern void Flash_ReadPage(uint8_t *pBuffer, uint32_t Page_Number, uint32_t NumByteToRead_up_to_PageSize);

#endif /* SRC_MYFLAH_H_ */
