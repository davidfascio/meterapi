/* 
 * File:   Meters_Table.h
 * Author: fascio
 *
 * Created on 21 de agosto de 2015, 03:46 PM
 */

#ifndef __METERS_TABLE_H__
#define	__METERS_TABLE_H__

#include "GenericTypeDefs.h"
#include "MeterInterface.h"
#include "MeterCommons.h"
#include "MeterControl.h"
#include "SystemEvents.h"



#define NUM_MAX_METERS                                      24
#define SIGNATURE_MTR                                       'p'
#define MAC_SIZE                                             (8)
#define SHORT_MAC_SIZE                                       (4)
#define BROADCAST_MDB                                           0x00
#define BROADCAST_MDB_ESP                                       0xF7

#define METER_TABLE_METER_NO_ERROR_CODE                     (0)
#define METER_TABLE_METER_ID_ERROR_CODE                     (0xFE)
#define METER_TABLE_SERIAL_NUMBER_ERROR_CODE                (-1)
#define METER_TABLE_COULD_NOT_SAVE_SERIAL_NUMBER_IN_NVM     (-2)
#define METER_TABLE_METER_ALREADY_EXISTS                    (-3)
#define METER_TABLE_SERIAL_NUMBER_NOT_MATCHED_ERROR_CODE    (-4)
#define METER_TABLE_COMMAND_METER_API_BUSY                  (-5)
#define METER_TABLE_EMPTY_QUEUE                             (-6)

#define METER_TABLE_SERIAL_NUMBER_NOT_FOUND                 (0xFF)

#define METER_TABLE_OFFSET_SIGNATURE_MTR                            (1)
#define METER_TABLE_EMPTY_LOCATION                         (0xFF)
#define METER_TABLE_EMPTY_VALUE                             (0xFF)
#define METER_TABLE_NO_METER_ID_FOUND                      (0xFF)
#define METER_TABLE_MAX_RESPONSE_SIZE                       (250)

#define METER_TABLE_BROADCAST_METER_ID                      0xFF
//typedef struct __attribute__((packed,aligned(1))) _Data_Readings

//typedef struct __attribute__((packed,aligned(1))) _Meter_Eneri
typedef struct _Meter_Eneri
{
    BYTE Serial_Num[Lenght_Meter_ID];
    BYTE Type;
    LONG_ADDR MACAdd_Display;
    BYTE Signature;
} Meter_Eneri;

typedef struct _Readings_Eneri
{
    Data_Readings Reading;
    BYTE CRC[2];
} Readings_Eneri;

typedef struct _Meters_Table
{
    Meter_Eneri Meter_DEV[NUM_MAX_METERS];
    Readings_Eneri Readings[NUM_MAX_METERS];
} Meters_Table;

void vfnAddDelMeterDriver(void);

void MeterTable_SetStateMachine(BYTE actualState);
BYTE MeterTable_GetMeterTypeByMeterId(BYTE meterId);
BYTE MeterTable_GetModbusIdByMeterId(BYTE meterId);
BYTE MeterTable_GetSerialNumberByMeterId(BYTE meterId, BYTE * serialNumber, BYTE serialNumberLen);
BYTE MeterTable_GetNextStateMachine(void);
BYTE findAvailableMeterId(void);
BYTE findAvailableModbusId(void);

void MeterTable_SendCommand(    BYTE command ,
                                BYTE * data, 
                                WORD dataLen, 
                                BOOL answerRequired,
                                WORD timeoutValue, 
                                BYTE maxRetries,
                                WORD stabilizationTimeoutValue, 
                                BYTE nextState);

void MeterTable_SendCommandByIdentificator( BYTE modbusId, 
                                            BYTE * serialNumber, 
                                            WORD serialNumberLen, 
                                            BYTE command ,
                                            BYTE * data, 
                                            WORD dataLen, 
                                            BOOL answerRequired,
                                            WORD timeoutValue, 
                                            BYTE maxRetries,
                                            WORD stabilizationTimeoutValue, 
                                            BYTE nextState);

void MeterTable_SendNextCommand(WORD stabilizationTimeoutValue, BYTE nextState);
void MeterTable_ErrorReset(void);
BYTE API_MeterTable_SendCommand(BYTE meterId, BYTE commandId);
BYTE API_MeterTable_ExcecuteCommand(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE commandId, BYTE meterType, BOOL broadcastSent);
//BYTE API_MeterTable_ExcecuteCommand(BYTE meterId, BYTE commandId, BYTE meterType);
BYTE MeterTable_ExcecuteCommand(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE commandId, BYTE meterType, BOOL broadcastSent);
//BYTE MeterTable_ExcecuteCommand(BYTE meterId, BYTE commandId, BYTE meterType);

//******************************************************************************
// Handler  
//******************************************************************************
BOOL MeterTable_IsValidSerialNumber(BYTE * serialNumber, WORD serialNumberLen);
BYTE MeterTable_FindMeterBySerialNumber(BYTE * serialNumber, WORD serialNumberLen);
BYTE MeterTable_AddNewMeter(BYTE meterId, BYTE meterType,BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen);
BYTE MeterTable_DeleteMeter(BYTE meterId, BYTE meterType,BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen);
BYTE MeterTable_UpdateMeter(BYTE meterId, BYTE meterType, BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen);
BYTE MeterTable_AddNewMeterBySerialNumber(BYTE meterType, BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen);
BYTE MeterTable_DeleteMeterBySerialNumber(BYTE meterType, BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen);

BYTE MeterTable_ResponseHandler(BYTE meterType, BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE command);
void MeterTable_ReceiveHandler(void);

BOOL MeterTable_IsCommandMeterAPIBusy(void);
void MeterTable_SetCommandMeterAPIBusy(BOOL state);

typedef struct{
    
    BYTE meterId;
    BYTE commandId;
    BYTE meterType;
    
} QUEUE_INFO, * QUEUE_INFO_PTR;

BYTE API_MeterTable_QueueInfoCheck(void);
void API_MeterTable_ExcecuteNewBaptismProcess(void);
void API_MeterTable_ExcecuteCommandInvoke( METER_DESCRIPTOR_PTR meterDescriptor, BYTE commandCallBack);

#endif	/* __METERS_TABLE_H__ */

