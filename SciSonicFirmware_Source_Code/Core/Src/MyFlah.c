/*
/  Filename: MyFlah.c	                                                    /
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
#include "main.h"
#include "MyFlah.h"

SPI_HandleTypeDef hspi2;

extern void WDT();

#define hspi	hspi2
#define ENABLE_CS()		HAL_GPIO_WritePin(F_CS_GPIO_Port,F_CS_Pin,GPIO_PIN_RESET)
#define Disable_CS()	HAL_GPIO_WritePin(F_CS_GPIO_Port,F_CS_Pin,GPIO_PIN_SET)
#define SPI_TIMEOUT		0xFF
#define DUMMY_BYTE		0xAB

//------------------------------------------------------------------------------------
uint8_t SPI_RW(uint8_t data)
{
	uint8_t	ret;
	HAL_SPI_TransmitReceive(&hspi,&data,&ret,1,SPI_TIMEOUT);
	//HAL_SPI_TransmitReceive_IT(&hspi,&data,&ret,1);
	while(hspi.State!=HAL_SPI_STATE_READY);
	return ret;
}
//------------------------------------------------------------------------------------
void SPI_Transmit(uint8_t *pBuffer, uint32_t NumByteToWrite)
{
	uint16_t count = (uint16_t)NumByteToWrite;
	HAL_SPI_Transmit(&hspi,pBuffer,count,SPI_TIMEOUT);
	while(hspi.State!=HAL_SPI_STATE_READY);
	//HAL_SPI_Transmit_IT(&hspi,pBuffer,count);
}
//------------------------------------------------------------------------------------
void SPI_Receive(uint8_t *pBuffer, uint32_t NumByteToRead)
{
	uint16_t count = (uint16_t)NumByteToRead;
	HAL_SPI_Receive(&hspi,pBuffer,count,SPI_TIMEOUT);
	//HAL_SPI_Receive_IT(&hspi,pBuffer,count);
}
//------------------------------------------------------------------------------------
void Flash_Init()
{
	Disable_CS();
	Flash_WriteDisable();
	Flash_WaitForWriteEnd();
	Flash_ReadStatusRegister1();
}
//------------------------------------------------------------------------------------
void Flash_WriteEnable(void)
{
	ENABLE_CS();
	SPI_RW(0x06);
	Disable_CS();
}
//------------------------------------------------------------------------------------
void Flash_WriteDisable(void)
{
	ENABLE_CS();
	SPI_RW(0x04);
	Disable_CS();
}
//------------------------------------------------------------------------------------
/*void Flash_WriteStatusRegister()
{

}*/
//------------------------------------------------------------------------------------
uint8_t Flash_ReadStatusRegister1()
{
	uint8_t	status=0;
	ENABLE_CS();
	SPI_RW(0x05);
	status = SPI_RW(DUMMY_BYTE);
	Disable_CS();
return status;
}
//------------------------------------------------------------------------------------
uint8_t Flash_ReadStatusRegister2()
{
	uint8_t	status=0;
	ENABLE_CS();
	SPI_RW(0x35);
	status = SPI_RW(DUMMY_BYTE);
	Disable_CS();
return status;
}
//------------------------------------------------------------------------------------
void Flash_WaitForWriteEnd(void)
{
	uint8_t	status=0;
	ENABLE_CS();
	SPI_RW(0x05);
	  do
	  {
		  status = SPI_RW(DUMMY_BYTE);
		  HAL_Delay(1);
		  WDT();
	  }
	  while ((status & 0x01) == 0x01);
	Disable_CS();
}
//------------------------------------------------------------------------------------
void Flash_EraseChip()
{
	Flash_WaitForWriteEnd();
	Flash_WriteEnable();
	ENABLE_CS();
	SPI_RW(0xC7);
	Disable_CS();
	Flash_WaitForWriteEnd();
	Flash_WriteDisable();
	Flash_WaitForWriteEnd();
}
//------------------------------------------------------------------------------------
void Flash_WritePage(uint8_t *pBuffer, uint32_t Page_Number, uint32_t NumByteToWrite_up_to_PageSize)
{
	uint8_t addr;
	uint8_t *pbuf = pBuffer;
	Flash_WaitForWriteEnd();
	Flash_WriteEnable();
	Page_Number = (Page_Number*256);
	ENABLE_CS();
	SPI_RW(0x02);
	addr = (uint8_t)((Page_Number & 0x00FF0000) >> 16);
	SPI_RW(addr);
	addr = (uint8_t)((Page_Number & 0x0000FF00) >> 8);
	SPI_RW(addr);
	addr = (uint8_t)((Page_Number & 0x000000FF));
	SPI_RW(addr);
	//SPI_Transmit(pBuffer,NumByteToWrite_up_to_PageSize);
	while(NumByteToWrite_up_to_PageSize--)
	{
		SPI_RW(*pbuf);
		pbuf++;
	}
	Disable_CS();
	Flash_WaitForWriteEnd();
	Flash_WriteDisable();
	Flash_WaitForWriteEnd();
}
//------------------------------------------------------------------------------------
void Flash_ReadPage(uint8_t *pBuffer, uint32_t Page_Number, uint32_t NumByteToRead_up_to_PageSize)
{
	uint8_t addr;
	uint8_t *pbuf = pBuffer;
	Flash_WaitForWriteEnd();
	Flash_WriteDisable();
	Flash_WaitForWriteEnd();
	Page_Number = (Page_Number*256);
	ENABLE_CS();
	SPI_RW(0x0B);
	addr = (uint8_t)((Page_Number & 0x00FF0000) >> 16);
	SPI_RW(addr);
	addr = (uint8_t)((Page_Number & 0x0000FF00) >> 8);
	SPI_RW(addr);
	addr = (uint8_t)((Page_Number & 0x000000FF));
	SPI_RW(addr);
	SPI_RW(DUMMY_BYTE);
	//SPI_Receive(pBuffer,NumByteToRead_up_to_PageSize);
	while(NumByteToRead_up_to_PageSize--)
	{
		*pbuf = SPI_RW(DUMMY_BYTE);
		pbuf++;
	}
	Disable_CS();
}
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
