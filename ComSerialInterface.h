/* 
 * File:   ComSerialInterface.h
 * Author: fascio
 *
 * Created on 4 de enero de 2016, 12:35
 */

#ifndef __COM_SERIAL_INTERFACE_H__
#define	__COM_SERIAL_INTERFACE_H__

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

HANDLE ComSerialInterface_OpenHandler(  LPCSTR  portName, 
                                        DWORD   baudRate,
                                        DWORD   byteSize,
                                        DWORD   stopBits,
                                        BYTE    parity  );

void ComSerialInterface_CloseHandler( HANDLE hSerial);
DWORD ComSerialInterface_ReadPort( HANDLE hSerial, BYTE * buffer, DWORD buffersize);
DWORD ComSerialInterface_WritePort( HANDLE hSerial, BYTE * buffer, DWORD buffersize);
void ComSerialInterface_CleanBuffer(void);

void ComSerialInterface_Init(void);
DWORD ComSerialInterface_WriteData(BYTE * data, DWORD dataLen);
DWORD ComSerialInterface_ReadData(BYTE * data, DWORD dataLen);
void ComSerialInterface_Check(void);

void ComSerialInterface_PrintData(BYTE * frame, WORD frameLen);
BYTE * ComSerialInterface_GetBuffer( void);    
WORD ComSerialInterface_GetBufferSize(void);

#endif	/* __COM_SERIAL_INTERFACE_H__ */

