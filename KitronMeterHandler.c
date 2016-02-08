#include "KitronMeterHandler.h"

KITRON_METER_HANDLER kitronMeterHandlerControl;

void KitronMeterHandler_Setup(BYTE modbusId, BYTE functionCommand, BYTE dataSize, BYTE * data){
    
    kitronMeterHandlerControl.modbusId = modbusId;
    kitronMeterHandlerControl.functionCommand = functionCommand;
    kitronMeterHandlerControl.dataSize = dataSize;
    memcpy(kitronMeterHandlerControl.data, data, dataSize);
}

WORD API_KitronMeterHandler_ReceiveHandler( BYTE * buffer, WORD  buffersize, METER_DESCRIPTOR_PTR meterDescriptor, BYTE * commandCallBack){
    
    BYTE * buffer_ptr = buffer;
    BYTE modbusId;
    BYTE functionCommand;
    BYTE dataSize;
    BYTE * data;
    WORD crc;
    
    crc = wfnCRC_CALC(buffer, buffersize, KITRON_METER_HANDLER_CRC_BASE);
    
    if (crc != KITRON_METER_HANDLER_CRC_OK)
        return KITRON_METER_HANDLER_WRONG_CRC_ERROR_CODE;
    
    memcpy(&modbusId, buffer_ptr, KITRON_METER_HANDLER_MODBUS_ID_HEADER_SIZE);
    
    if (modbusId == KITRON_METER_HANDLER_INVALID_MODBUS_ID )
        return KITRON_METER_HANDLER_INVALID_MODBUS_ID_ERROR_CODE;
    
    buffer_ptr += KITRON_METER_HANDLER_MODBUS_ID_HEADER_SIZE;
    
    
    memcpy(&functionCommand, buffer_ptr, KITRON_METER_HANDLER_COMMAND_FUNCTION_HEADER_SIZE);
    
    if (functionCommand != KITRON_METER_HANDLER_READ_COMMAND_FUNCTION)
        return KITRON_METER_HANDLER_COMMAND_FUNCTION_NOT_SUPPORTED_ERROR_CODE;
    
    buffer_ptr += KITRON_METER_HANDLER_COMMAND_FUNCTION_HEADER_SIZE;
    
    memcpy(&dataSize, buffer_ptr, KITRON_METER_HANDLER_DATA_SIZE_HEADER_SIZE);
    
    if( dataSize != (buffersize - KITRON_METER_HANDLER_FILEDS_HEADER_SIZE))
        return KITRON_METER_HANDLER_WRONG_DATA_SIZE_ERROR_CODE;
    
    buffer_ptr += KITRON_METER_HANDLER_DATA_SIZE_HEADER_SIZE;
    
    data = buffer_ptr;
    
    KitronMeterHandler_Setup(modbusId, functionCommand, dataSize, data);
    
    MeterDescriptor_Setup(meterDescriptor, modbusId, NULL, 0, KITRON_METER_TYPE);    
    * commandCallBack = API_KitronMeterHandler_GetInvokeFunctionId(functionCommand);
    
    return KITRON_METER_HANDLER_NO_ERROR_CODE;
}

WORD API_KitronmeterHandler_ResponseHandler( BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE command, BYTE * response, WORD maxResponseLen, WORD * responseLen, BYTE * commadCallBack){

    if (kitronMeterHandlerControl.modbusId != modbusId)
        return KITRON_METER_HANDLER_INVALID_MODBUS_ID_ERROR_CODE;
    
    switch(command){
        
        case NO_COMMAND_MTR: 
            return KITRON_METER_HANDLER_NO_ERROR_CODE;
       
        case READ_MODE:
        case READ_ALL_VOLTAGE_MTR:
            
            if( (kitronMeterHandlerControl.functionCommand == KITRON_METER_HANDLER_READ_COMMAND_FUNCTION) &&
                (kitronMeterHandlerControl.dataSize == KITRON_METER_HANDLER_VOLTAGE_DATA_SIZE) ){           
            
                if(maxResponseLen >= sizeof(Data_Readings)){
                    
                    KitronMeterHandler_ParseVoltageToDataReading((Data_Readings_Ptr) response, kitronMeterHandlerControl.data, kitronMeterHandlerControl.dataSize);
                
                    return KITRON_METER_HANDLER_NO_ERROR_CODE;
                }
            }
            
            break;
            
        case READ_ALL_CURRENT_MTR:
            
            // ADD CODE:
            
            return KITRON_METER_HANDLER_NO_ERROR_CODE;
            
        case READ_ALL_ACTIVE_POWER_MTR:
            
            // ADD CODE:
            
            return KITRON_METER_HANDLER_NO_ERROR_CODE;
            
        case READ_ALL_POWER_FACTOR_MTR:
            
            // ADD CODE:
            
            return KITRON_METER_HANDLER_NO_ERROR_CODE;
            
        case READ_ALL_APPARENT_POWER_MTR:
            
            // ADD CODE:
            
            return KITRON_METER_HANDLER_NO_ERROR_CODE;
    }
}

BYTE API_KitronMeterHandler_GetInvokeFunctionId(BYTE command){
    
    switch(command){

        /*
         * case KITRON_METER_HANDLER_COMMAND:
         *      return METER_COMMON_COMMAND_FOR_SPECIFIC_PURPORSE;         
         */
            
        default:
            return NO_COMMAND_MTR;
    }
}



void KitronMeterHandler_ParseVoltageToDataReading( Data_Readings_Ptr dataReading, BYTE * data, WORD dataLen){
    
    KitronMeter_Voltage_DataReading voltage;
    
    if(dataLen != KITRON_METER_HANDLER_VOLTAGE_DATA_SIZE)
        return;
    
    inverted_memcpy((BYTE *) &voltage, data, dataLen);    
    KitronMeterHandler_PrintDataReading(&voltage);
    
}

void KitronMeterHandler_PrintDataReading(KitronMeter_Voltage_DataReading_Ptr voltage){
    
    println_message("[KITRON METER DATA READING INFORMATION]");
    
    print_message("\n\r\t\tVoltage (A): %f", voltage->VoltagePhaseA);
    print_message("\n\r\t\tVoltage (B): %f", voltage->VoltagePhaseB);
    print_message("\n\r\t\tVoltage (C): %f", voltage->VoltagePhaseC);
    
}