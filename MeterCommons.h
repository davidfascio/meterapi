/* 
 * File:   MeterCommons.h
 * Author: fascio
 *
 * Created on 21 de diciembre de 2015, 11:26
 */

#ifndef __METER_COMMONS_H__
#define	__METER_COMMONS_H__

//******************************************************************************
// Includes
//******************************************************************************
#include "GenericTypeDefs.h"
#include "EventsEngine.h"
#include "SystemLog.h"

//******************************************************************************
// Common Defines
//******************************************************************************

#define SIGNATURE_MTR                                       'p'
#define BROADCAST_MDB                                           0x00
#define BROADCAST_MDB_ESP                                       0xF7

// Common meter types
#define METER_INTERFACE_NO_METER_TYPE                                   (0x00)

// Command Id defines
#define NO_COMMAND_MTR                                                  (0)

#define Dis_MTR                                                         (1)
#define Con_MTR                                                         (2)
#define Res_MTR                                                         (3)
#define READ_MODE                                                       (4)
#define PSW_MTR                                                         (5)
#define SEND_MAC_BROADCAST_MTR                                          (6)
#define ASSIGN_MODBUS_ID_MTR                                            (7)
#define REQUEST_SERIAL_NUMBER_MTR                                       (8)

#define LINK_ADDING_MTR                                                 (9)
#define LINK_DELETING_MTR                                               (10)

#define READ_ALL_VOLTAGE_MTR                                            (11)
#define READ_ALL_CURRENT_MTR                                            (12)
#define READ_ALL_ACTIVE_POWER_MTR                                       (13)
#define READ_ALL_POWER_FACTOR_MTR                                       (14)
#define READ_ALL_APPARENT_POWER_MTR                                     (15)


#define Lenght_Meter_ID                                                 (16)
#define METER_DESCRIPTOR_MAX_SERIAL_NUMBER_SIZE                         (20)

//******************************************************************************
// Data types
//******************************************************************************

typedef struct{
    
    BYTE commandId;
    void (* CommandIdFunction_Callback)(BYTE modusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen);
    
} COMMAND_ID_FUNCTION, * COMMAND_ID_FUNCTION_PTR;

#define COMMAND_ID_FUNCTION_NULL                            {NO_COMMAND_MTR, NULL}

typedef struct{

    BYTE modbusId;
    BYTE serialNumber[METER_DESCRIPTOR_MAX_SERIAL_NUMBER_SIZE];
    WORD serialNumberLen;
    BYTE meterType;
    
} METER_DESCRIPTOR, * METER_DESCRIPTOR_PTR;

void MeterDescriptor_Setup(METER_DESCRIPTOR_PTR meterDescriptor, BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE meterType);
void MeterDescriptor_SetModbusId(METER_DESCRIPTOR_PTR meterDescriptor, BYTE modbusId);
BYTE MeterDescriptor_GetModbusId(METER_DESCRIPTOR_PTR meterDescriptor);

void MeterDescriptor_SetSerialNumber(METER_DESCRIPTOR_PTR meterDescriptor, BYTE * serialNumber, WORD serialNumberLen);
WORD MeterDescriptor_GetSerialNumber(METER_DESCRIPTOR_PTR meterDescriptor, BYTE * serialNumber, WORD serialNumberLen);

void MeterDescriptor_SetMeterType(METER_DESCRIPTOR_PTR meterDescriptor, BYTE meterType);
BYTE MeterDescriptor_GetMeterType(METER_DESCRIPTOR_PTR meterDescriptor);


typedef struct{
    
    BYTE meterType; 
    BYTE broadcastId;
    BYTE * broadcastSerialNumber;
    WORD broadcastSerialNumberLen;
    WORD stabilizationTimeoutValue;
    COMMAND_ID_FUNCTION_PTR commandIdFunctionList;    
    WORD (* meterHandler_ResponseProcessCallback)( BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE command, BYTE * response, WORD maxResponseLen, WORD * responseLen);    
    WORD (* meterHandler_ReceiveProcessCallback)( BYTE * buffer, WORD  bufferLen, METER_DESCRIPTOR_PTR meterDescriptor, BYTE * commandCallBack);    
    BOOL dataAvailable;
} METER_COMMAND_ID_FUNCTION_API, * METER_COMMAND_ID_FUNCTION_API_PTR;

#define METER_COMMAND_ID_FUNCTION_API_NULL                  { METER_INTERFACE_NO_METER_TYPE, 0, NULL, 0, 0, NULL, NULL , NULL, FALSE }

typedef struct  _Data_Readings
{
    DWORD   ENERGY_ACT_C_Add;
    DWORD   ENERGY_ACT_B_Add;
    DWORD   ENERGY_ACT_A_Add;
    DWORD   POWER_ACT_SYSTEM_Add;
    DWORD   FLAGS_Add_LWEND;
    WORD    DELTA_REAC_Add;
    WORD    DELTA_ACT_Add;
    WORD    INDEX_CON_Add;
    DWORD   ROLLER_DEMAND_Add;
    WORD    POWER_FACTOR_Add;
    WORD    CURRENT_C_Add;
    WORD    CURRENT_B_Add;
    WORD    CURRENT_A_Add;
    WORD    VOLTAGE_C_Add;
    WORD    VOLTAGE_B_Add;
    WORD    VOLTAGE_A_Add;
    WORD    FRECUENCY_Add;
    DWORD   TIME_STAMP_Add;
    DWORD   ENERGY_NEG_ACT_TOTAL_Add;
    DWORD   ENERGY_NEG_REACT_TOTAL_Add;
    DWORD   ENERGY_REACT_TOTAL_Add;
}Data_Readings , * Data_Readings_Ptr;

#endif	/* __METER_COMMONS_H__ */
