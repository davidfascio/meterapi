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
#include "SystemQueue.h"

#include "MeterCommons.h"
#include "MeterInterface.h"
#include "Meters_Table.h"

//******************************************************************************
// Meter Control Defines
//******************************************************************************
#define METER_CONTROL_METER_ID_ERROR_CODE                               (0)
#define METER_CONTROL_DEFAULT_NUMBER_OF_RETRIES                         (3)
#define METER_CONTROL_NO_METER_ID                                       (0xFF)
#define METER_CONTROL_MAX_RESPONSE_SIZE                                 (250)

//******************************************************************************
// Meter Control Error Codes
//******************************************************************************
#define METER_CONTROL_NO_ERROR_CODE                                     (0)
#define METER_CONTROL_API_METER_COMMAND_BUSY_ERROR_CODE                 (1)
#define METER_CONTROL_EMPTY_QUEUE_ERROR_CODE                            (-6)
#define METER_CONTROL_COMMAND_NOT_FOUND_ERROR_CODE                      (-7)

//******************************************************************************
// Meter Control Data types
//******************************************************************************

typedef enum{
    
    METER_TIMEOUT_STOP,
    METER_TIMEOUT_INITIALIZED,
    METER_TIMEOUT_EXPIRED
            
}METER_TIMEOUT;

typedef struct {
    
    //BYTE meterId;                           // Maybe this field wont be necessary for meter control struct
    METER_DESCRIPTOR meterDescriptor;        
    BYTE commandId;
    BYTE retries;
    BOOL broadcastSent;
    BOOL answerRequired;
    BOOL dataAvailable;
    METER_TIMEOUT responseTimeout;
    METER_TIMEOUT stabilizationTimeout;
    
}METER_CONTROL, * METER_CONTROL_PTR;

//******************************************************************************
// Meter Control Set and Get Function Prototypes
//******************************************************************************
void MeterControl_Setup(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE meterType, BYTE commandId, BOOL broadcastSent, WORD stabilizationTimeoutValue);
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

void MeterControl_SetBroadcastSent(BOOL broadcastSent);
BOOL MeterControl_IsBroadcastSent(void);

//******************************************************************************
// Meter Control Function Prototypes
//******************************************************************************

void MeterControl_SendCommand(    BYTE command ,
                                BYTE * data, 
                                WORD dataLen, 
                                BOOL answerRequired,
                                WORD timeoutValue, 
                                BYTE maxRetries,
                                WORD stabilizationTimeoutValue, 
                                BYTE nextState);

void MeterControl_SendCommandByIdentificator( BYTE modbusId, 
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

void MeterControl_SendNextCommand(WORD stabilizationTimeoutValue, BYTE nextState);
void MeterControl_ErrorReset(void);
BYTE MeterControl_ExcecuteCommand(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE commandId, BYTE meterType, BOOL broadcastSent);

void MeterControl_Print( void );
//******************************************************************************
// API Meter Control Send Functions
//******************************************************************************

BYTE API_MeterControl_SendCommand(BYTE meterId, BYTE commandId);
void API_MeterControl_ExcecuteBaptismProccess(void);
BYTE API_MeterControl_ExcecuteCommand(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE commandId, BYTE meterType, BOOL broadcastSent);
BOOL API_MeterControl_IsCommandMeterBusy(void);
void API_MeterTable_SetCommandMeterBusy(BOOL state);
BYTE API_MeterControl_QueueInfoCheck(void);

//******************************************************************************
// API Meter Control Handler Functions
//******************************************************************************
BYTE API_MeterControl_ResponseHandler(BYTE meterType, BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE command);
void API_MeterControl_ReceiveHandler(void);

//******************************************************************************
// API Meter Control Invoke Functions
//******************************************************************************
void API_MeterControl_ExcecuteCommandInvoke( METER_DESCRIPTOR_PTR meterDescriptor, BYTE commandCallBack);

//******************************************************************************
// Local Meter Control State Machine Function Protoypes
//******************************************************************************

void vfnAddDelMeterDriver(void);
void MeterControl_SetStateMachine(BYTE actualState);
BYTE MeterControl_GetNextStateMachine(void);

#endif	/* __METER_CONTROL_H__ */

