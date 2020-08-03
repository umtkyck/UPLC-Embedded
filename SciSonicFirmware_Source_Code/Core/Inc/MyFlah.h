/*
/  Filename: MyFlah.h	                                                    /
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
