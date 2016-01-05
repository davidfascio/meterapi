/* 
 * File:   G155MeterInterface.h
 * Author: fascio
 *
 * Created on 16 de diciembre de 2015, 10:55
 */

#ifndef __G155_METER_INTERFACE_H__
#define	__G155_METER_INTERFACE_H__

//******************************************************************************
// Includes
//******************************************************************************
#include "Utility.h"
#include "MeterCommons.h"
#include "ComSerialInterface.h"

//******************************************************************************
// Defines
//******************************************************************************

// Valid meter type
#define G155_TYPE                                                          (0x84)

//Registers G155 for Commands
#define KEY1_G155                                                          (0xC3)
#define KEY2_G155                                                          (0x01)
#define RLY_G155_ON                                                        (0x00)
#define RLY_G155_OFF                                                       (0x01)
#define RST_G155_CMD                                                       (0x08)
#define ADD_KEY_G155                                                       (0x1C)
#define ADD_RLY_G155                                                       (0x0C)
#define NO_CMD                                                             (0xF0)                                   

#define ENERGY_G155                                             0x05
#define G155_METER_INTERFACE_ENERGY_REGISTER_QTY_SIZE           (8)

#define ZIGBEE_ADD                                              0x12
#define G155_METER_INTERFACE_MAC_ADDRESS_REGISTER_QTY_SIZE      (4)

#define ID_ADD                                                  0x11
#define CFE_NUMBER_ADD                                          0x0E
#define G155_METER_INTERFACE_SERIAL_NUMBER_REGISTER_QTY_SIZE    (3)
//Modbus Commands
#define READ_FN_MDB                                                         (0x03)
#define WRITE_SINGLE_FN_MDB                                                 (0x06)
#define WRITE_VARIOUS_FN_MDB                                                (0x10)

// G155 Modbus Protocol
#define G155_METER_INTERFACE_MODBUS_ID_HEADER_SIZE                          (1)
#define G155_METER_INTERFACE_MODBUS_COMMAND_FUNCTION_HEADER_SIZE            (2)
#define G155_METER_INTERFACE_MODBUS_REGISTER_ADDRESS_HEADER_SIZE            (1)
#define G155_METER_INTERFACE_MODBUS_CRC_HEADER_SIZE                         (2)
#define G155_METER_INTERFACE_MODBUS_EMPTY_LOCATION_HEADER_SIZE              (1)                       
#define G155_METER_INTERFACE_MODBUS_REGISTER_QTY_HEADER_SIZE                (1) 
#define G155_METER_INTERFACE_MODBUS_DATA_LEN_HEADER_SIZE                    (1) 

#define G155_METER_INTERFACE_MODBUS_CRC_BASE                            (0xFFFF)
#define G155_METER_INTERFACE_MAX_FRAME_BUFFER_SIZE                      (50)
//******************************************************************************
// Data types
//******************************************************************************

typedef struct{
    
    BYTE modbusId;
    WORD commandFunction;
    BYTE registerAddress;
    BYTE registerAmount;
    BYTE retryCounter;
    BYTE timeout;
    BYTE answerRequired;
} G155_METER_INTERFACE, * G155_METER_INTERFACE_PTR;

//******************************************************************************
// G155 Meter Interface API
//******************************************************************************
BYTE G155MeterInterface_Handler(    BYTE modbusId, 
                                    BYTE * serialNumber, 
                                    WORD serialNumberLen, 
                                    BYTE command, 
                                    BYTE * response, 
                                    WORD * responseLen);

BOOL G155MeterInterface_DataIsAvailable(void);
BOOL G155MeterInterface_BuildFrame( BYTE modbusId,
                                    WORD commandFunction,
                                    BYTE registerAddress, 
                                    BYTE registerAmount, 
                                    BYTE * data, 
                                    BYTE dataLen, 
                                    BYTE * frame, 
                                    BYTE * frameLen);

void G155MeterInterface_SendFrame(  BYTE modbusId,
                                    WORD commandFunction,
                                    BYTE registerAddress, 
                                    BYTE registerAmount, 
                                    BYTE * data, 
                                    BYTE dataLen );

void G155MeterInterface_SendPassword(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen);
void G155MeterInterface_Disconnect(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen);
void G155MeterInterface_Connect(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen);
void G155MeterInterface_Reset(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen);
void G155MeterInterface_ReadMeteringData(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen);


void G155MeterInterface_SendMACAddress(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen);
void G155MeterInterface_SendAssignModbusId(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen);
void G155MeterInterface_RequestSerialNumber(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen);

void G155MeterInterface_Update(void);

extern COMMAND_ID_FUNCTION G155MeterInterface_CommandIdFunctionList [];

void G155MeterInterface_PrintData(BYTE* frame, BYTE frameLen);
#endif	/* __G155_METER_INTERFACE_H__ */

