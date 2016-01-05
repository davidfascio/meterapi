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
//******************************************************************************
// Common Defines
//******************************************************************************

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

#define Lenght_Meter_ID                                                 (16)

//******************************************************************************
// Data types
//******************************************************************************

typedef struct{
    
    BYTE commandId;
    void (* CommandIdFunction_Callback)(BYTE modusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen);
    
} COMMAND_ID_FUNCTION, * COMMAND_ID_FUNCTION_PTR;

#define COMMAND_ID_FUNCTION_NULL                            {NO_COMMAND_MTR, NULL}


typedef struct{
    
    BYTE meterType; 
    BYTE broadcastId;
    BYTE * broadcastSerialNumber;
    WORD broadcastSerialNumberLen;
    WORD stabilizationTimeoutValue;
    COMMAND_ID_FUNCTION_PTR commandIdFunctionList;    
    BYTE (* meterHandler_Callback)( BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE command, BYTE * response, WORD * responseLen);    
    
} METER_COMMAND_ID_FUNCTION_API, * METER_COMMAND_ID_FUNCTION_API_PTR;

#define METER_COMMAND_ID_FUNCTION_API_NULL                  { METER_INTERFACE_NO_METER_TYPE, 0, NULL, 0, 0, NULL, NULL }


#endif	/* __METER_COMMONS_H__ */
