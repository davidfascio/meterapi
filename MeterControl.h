/* 
 * File:   MeterControl.h
 * Author: fascio
 *
 * Created on 21 de diciembre de 2015, 11:33
 */

#ifndef __METER_CONTROL_H__
#define	__METER_CONTROL_H__

//******************************************************************************
// Includes
//******************************************************************************

#include "EventsEngine.h"
#include "MeterCommons.h"
//#include "MeterInterface.h"

//******************************************************************************
// Error Codes
//******************************************************************************

#define METER_CONTROL_METER_ID_ERROR_CODE                             (0)
#define METER_CONTROL_DEFAULT_NUMBER_OF_RETRIES                       (3)

#define METER_CONTROL_NO_METER_ID                                     (0xFF)

//#define METER_CONTROL_MAX_SERIAL_NUMBER_SIZE                          (20)
//******************************************************************************
// Data types
//******************************************************************************

typedef enum{
    
    METER_TIMEOUT_STOP,
    METER_TIMEOUT_INITIALIZED,
    METER_TIMEOUT_EXPIRED
            
}METER_TIMEOUT;

typedef struct {
    
    BYTE meterId;                           // Maybe this field wont be necessary for meter control struct
    METER_DESCRIPTOR meterDescriptor;        
    BYTE commandId;
    BYTE retries;
    BOOL answerRequired;
    BOOL dataAvailable;
    METER_TIMEOUT responseTimeout;
    METER_TIMEOUT stabilizationTimeout;
    
}METER_CONTROL, * METER_CONTROL_PTR;

//******************************************************************************
// Meter Control Function
//******************************************************************************
void MeterControl_Setup( BYTE meterId, BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE meterType, BYTE commandId, WORD stabilizationTimeoutValue);
void MeterControl_Reset(WORD stabilizationTimeoutValue);
void MeterControl_Clear(void);

void MeterControl_SetDataAvailable(BOOL dataAvailable);
BOOL MeterControl_IsDataAvailable(void);

void MeterControl_SetAnswerRequired(BOOL answerRequired);
BOOL MeterControl_IsAnswerRequired(void);

void MeterControl_SetCommandId(BYTE commandId);
BYTE MeterControl_GetCommandId(void);

BYTE MeterControl_GetMeterType(void);
BYTE MeterControl_GetRetries(void);
void MeterControl_SetRetries(BYTE retries);
BYTE MeterControl_GetMeterId(void);

BYTE MeterControl_GetModbusId(void);
WORD MeterControl_GetSerialNumber(BYTE * serialNumber, WORD serialNumberLen);

//void MeterControl_SetMeterType(BYTE meterType);
METER_TIMEOUT MeterControl_GetStabilizationTimeout(void);
METER_TIMEOUT MeterControl_GetResponseTimeout(void);

void MeterControl_InitializeResponseTimeout(WORD timeoutValue);
void MeterControl_StopResponseTimeout(void);
void MeterControl_ExpireResponseTimeout(void);

void MeterControl_InitializeStabilizationTimeout(WORD timeoutValue);
void MeterControl_StopStabilizationTimeout(void);
void MeterControl_ExpireStabilizationTimeout(void);

#endif	/* __METER_CONTROL_H__ */

