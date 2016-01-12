#include "MeterCommons.h"

void MeterDescriptor_SetModbusId(METER_DESCRIPTOR_PTR meterDescriptor, BYTE modbusId){
    
    if(meterDescriptor == NULL)
        return;
    
    meterDescriptor->modbusId = modbusId;
}

BYTE MeterDescriptor_GetModbusId(METER_DESCRIPTOR_PTR meterDescriptor){
    
    return meterDescriptor->modbusId;
}

void MeterDescriptor_SetSerialNumber(METER_DESCRIPTOR_PTR meterDescriptor, BYTE * serialNumber, WORD serialNumberLen){
    
    if(meterDescriptor == NULL)
        return;
    
    if(serialNumberLen >= METER_DESCRIPTOR_MAX_SERIAL_NUMBER_SIZE)
        return;
    
    memset(meterDescriptor->serialNumber, 0, METER_DESCRIPTOR_MAX_SERIAL_NUMBER_SIZE);
    memcpy(meterDescriptor->serialNumber, serialNumber, serialNumberLen);
    meterDescriptor->serialNumberLen = serialNumberLen;
}

WORD MeterDescriptor_GetSerialNumber(METER_DESCRIPTOR_PTR meterDescriptor, BYTE * serialNumber, WORD serialNumberLen){
    
    if(meterDescriptor == NULL)
        return 0;
    
    if(serialNumberLen < meterDescriptor->serialNumberLen)
        return 0;
    
    memset(serialNumber, 0 , serialNumberLen);
    memcpy(serialNumber, meterDescriptor->serialNumber, meterDescriptor->serialNumberLen);
    
    return meterDescriptor->serialNumberLen;
}

void MeterDescriptor_SetMeterType(METER_DESCRIPTOR_PTR meterDescriptor, BYTE meterType){
    
    if(meterDescriptor == NULL)
        return;
    
    meterDescriptor->meterType = meterType;
}

BYTE MeterDescriptor_GetMeterType(METER_DESCRIPTOR_PTR meterDescriptor){
    
    return meterDescriptor->meterType;    
}
