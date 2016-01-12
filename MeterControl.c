#include "MeterControl.h"

METER_CONTROL meterControl;

void MeterControl_Setup( BYTE meterId, BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE meterType, BYTE commandId, WORD stabilizationTimeoutValue)
//void MeterControl_Setup( BYTE meterId, BYTE meterType, BYTE commandId, WORD stabilizationTimeoutValue)
{   
    meterControl.meterId   = meterId;
    meterControl.modbusId = modbusId;
    meterControl.serialNumberLen = serialNumberLen;
    memcpy(meterControl.serialNumber, serialNumber, serialNumberLen);
    meterControl.commandId = commandId;
    meterControl.meterType = meterType;
    meterControl.retries   = 0;
    meterControl.answerRequired = FALSE;
    //meterControl.timeout = METER_TIMEOUT_EXPIRED;    
    meterControl.dataAvailable = FALSE;
    MeterControl_StopResponseTimeout();
    MeterControl_InitializeStabilizationTimeout(stabilizationTimeoutValue);
}

void MeterControl_Reset(WORD stabilizationTimeoutValue){
    
    meterControl.retries = 0;
    meterControl.answerRequired = FALSE;
    meterControl.dataAvailable = FALSE;
    MeterControl_StopResponseTimeout();
    MeterControl_InitializeStabilizationTimeout(stabilizationTimeoutValue);
}

void MeterControl_SetDataAvailable(BOOL dataAvailable){
    
    meterControl.dataAvailable = dataAvailable;
}

BOOL MeterControl_IsDataAvailable(void){
    
    return meterControl.dataAvailable;
}

void MeterControl_SetAnswerRequired(BOOL answerRequired){

    meterControl.answerRequired = answerRequired;
}

BOOL MeterControl_IsAnswerRequired(void){

    return meterControl.answerRequired;
}

void MeterControl_SetCommandId(BYTE commandId){
    
    meterControl.commandId = commandId;
}

BYTE MeterControl_GetCommandId(void){
    
    return meterControl.commandId;
}

BYTE MeterControl_GetMeterType(void){
    
    return meterControl.meterType;
}

BYTE MeterControl_GetRetries(void){
    
    return meterControl.retries;
}

void MeterControl_SetRetries(BYTE retries){
    
    meterControl.retries = retries;
}

BYTE MeterControl_GetMeterId(void){
    
    return meterControl.meterId;
}

BYTE MeterControl_GetModbusId(void){
    
    return meterControl.modbusId;
}

WORD MeterControl_GetSerialNumber(BYTE * serialNumber, WORD serialNumberLen){
    
    if (serialNumberLen > METER_CONTROL_MAX_SERIAL_NUMBER_SIZE)
        return 0;   
    
    memset(serialNumber, 0, serialNumberLen);
    memcpy(serialNumber, meterControl.serialNumber, meterControl.serialNumberLen);        
    return meterControl.serialNumberLen;    
}

METER_TIMEOUT MeterControl_GetStabilizationTimeout(void){
    
    return meterControl.stabilizationTimeout;
}

METER_TIMEOUT MeterControl_GetResponseTimeout(void){
    
    return meterControl.responseTimeout;
}

void MeterControl_Clear(void){
    
    memset(&meterControl, 0, sizeof(meterControl));
}

void MeterControl_InitializeResponseTimeout(WORD timeoutValue){
    
    if(timeoutValue == 0)
        return MeterControl_StopResponseTimeout();
    
    vfnOneShotReload(TIME_OUT_METER_RESPONSE_ONESHOT, timeoutValue);
    meterControl.responseTimeout = METER_TIMEOUT_INITIALIZED;
}

void MeterControl_StopResponseTimeout(void){
    
    vfnOneShotDisable(TIME_OUT_METER_RESPONSE_ONESHOT);
    meterControl.responseTimeout = METER_TIMEOUT_STOP;
}

void MeterControl_ExpireResponseTimeout(void){
    vfnOneShotDisable(TIME_OUT_METER_RESPONSE_ONESHOT);
    meterControl.responseTimeout = METER_TIMEOUT_EXPIRED;
}

void MeterControl_InitializeStabilizationTimeout(WORD timeoutValue){
    
    if(timeoutValue == 0)
        return MeterControl_StopStabilizationTimeout();
    
    vfnOneShotReload(TIME_OUT_METER_STABILIZE_ONESHOT, timeoutValue);
    meterControl.stabilizationTimeout = METER_TIMEOUT_INITIALIZED;
}

void MeterControl_StopStabilizationTimeout(void){
    vfnOneShotDisable(TIME_OUT_METER_STABILIZE_ONESHOT);
    meterControl.stabilizationTimeout = METER_TIMEOUT_STOP;
}

void MeterControl_ExpireStabilizationTimeout(void){
    vfnOneShotDisable(TIME_OUT_METER_STABILIZE_ONESHOT);
    meterControl.stabilizationTimeout = METER_TIMEOUT_EXPIRED;
}
