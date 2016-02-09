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
    
    * commadCallBack = NO_COMMAND_MTR;
    switch(command){
        
        case NO_COMMAND_MTR: 
            return KITRON_METER_HANDLER_NO_ERROR_CODE;
       
        case READ_MODE:
        case READ_IMPORTED_ACTIVE_ENERGY_MTR:
            
            if( (kitronMeterHandlerControl.functionCommand == KITRON_METER_HANDLER_READ_COMMAND_FUNCTION) &&
                (kitronMeterHandlerControl.dataSize == KITRON_METER_HANDLER_IMPORTED_ACTIVE_ENERGY_DATA_SIZE) ){           
            
                if(maxResponseLen >= sizeof(Data_Readings)){
                    
                    KitronMeterHandler_ParseImportedActiveEnergyToDataReading((Data_Readings_Ptr) response, kitronMeterHandlerControl.data, kitronMeterHandlerControl.dataSize);
                    * responseLen = sizeof(Data_Readings);
                    * commadCallBack = READ_EXPORTED_ACTIVE_ENERGY_MTR;
                    return KITRON_METER_HANDLER_NO_ERROR_CODE;
                }
            }
                        
            break;
        
        case READ_EXPORTED_ACTIVE_ENERGY_MTR:
            
            if( (kitronMeterHandlerControl.functionCommand == KITRON_METER_HANDLER_READ_COMMAND_FUNCTION) &&
                (kitronMeterHandlerControl.dataSize == KITRON_METER_HANDLER_EXPORTED_ACTIVE_ENERGY_DATA_SIZE) ){           
            
                if(maxResponseLen >= sizeof(Data_Readings)){
                    
                    KitronMeterHandler_ParseExportedActiveEnergyToDataReading((Data_Readings_Ptr) response, kitronMeterHandlerControl.data, kitronMeterHandlerControl.dataSize);
                    * responseLen = sizeof(Data_Readings);
                    * commadCallBack = READ_IMPORTED_REACTIVE_ENERGY_MTR;
                    return KITRON_METER_HANDLER_NO_ERROR_CODE;
                }
            }
            
            break;
        
        case READ_IMPORTED_REACTIVE_ENERGY_MTR:
            
            if( (kitronMeterHandlerControl.functionCommand == KITRON_METER_HANDLER_READ_COMMAND_FUNCTION) &&
                (kitronMeterHandlerControl.dataSize == KITRON_METER_HANDLER_IMPORTED_REACTIVE_ENERGY_DATA_SIZE) ){           
            
                if(maxResponseLen >= sizeof(Data_Readings)){
                    
                    KitronMeterHandler_ParseImportedReactiveEnergyToDataReading((Data_Readings_Ptr) response, kitronMeterHandlerControl.data, kitronMeterHandlerControl.dataSize);
                    * responseLen = sizeof(Data_Readings);
                    * commadCallBack = READ_EXPORTED_REACTIVE_ENERGY_MTR;
                    return KITRON_METER_HANDLER_NO_ERROR_CODE;
                }
            }
            
            break;
            
        case READ_EXPORTED_REACTIVE_ENERGY_MTR:
            
            if( (kitronMeterHandlerControl.functionCommand == KITRON_METER_HANDLER_READ_COMMAND_FUNCTION) &&
                (kitronMeterHandlerControl.dataSize == KITRON_METER_HANDLER_EXPORTED_REACTIVE_ENERGY_DATA_SIZE) ){           
            
                if(maxResponseLen >= sizeof(Data_Readings)){
                    
                    KitronMeterHandler_ParseExportedReactiveEnergyToDataReading((Data_Readings_Ptr) response, kitronMeterHandlerControl.data, kitronMeterHandlerControl.dataSize);
                    * responseLen = sizeof(Data_Readings);
                    * commadCallBack = READ_ALL_VOLTAGE_MTR;
                    return KITRON_METER_HANDLER_NO_ERROR_CODE;
                }
            }
            
            break;
            
        case READ_ALL_VOLTAGE_MTR:
            
            if( (kitronMeterHandlerControl.functionCommand == KITRON_METER_HANDLER_READ_COMMAND_FUNCTION) &&
                (kitronMeterHandlerControl.dataSize == KITRON_METER_HANDLER_VOLTAGE_DATA_SIZE) ){           
            
                if(maxResponseLen >= sizeof(Data_Readings)){
                    
                    KitronMeterHandler_ParseVoltageToDataReading((Data_Readings_Ptr) response, kitronMeterHandlerControl.data, kitronMeterHandlerControl.dataSize);
                    * responseLen = sizeof(Data_Readings);
                    * commadCallBack = READ_ALL_CURRENT_MTR;
                    return KITRON_METER_HANDLER_NO_ERROR_CODE;
                }
            }
            
            break;
            
        case READ_ALL_CURRENT_MTR:
            
            if( (kitronMeterHandlerControl.functionCommand == KITRON_METER_HANDLER_READ_COMMAND_FUNCTION) &&
                (kitronMeterHandlerControl.dataSize == KITRON_METER_HANDLER_CURRENT_DATA_SIZE) ){           
            
                if(maxResponseLen >= sizeof(Data_Readings)){
                    
                    KitronMeterHandler_ParseCurrentToDataReading((Data_Readings_Ptr) response, kitronMeterHandlerControl.data, kitronMeterHandlerControl.dataSize);
                    * responseLen = sizeof(Data_Readings);
                    * commadCallBack = READ_POWER_FACTOR_MTR;
                    return KITRON_METER_HANDLER_NO_ERROR_CODE;
                }
            }
            
            break;
            
            case READ_POWER_FACTOR_MTR:
            
            if( (kitronMeterHandlerControl.functionCommand == KITRON_METER_HANDLER_READ_COMMAND_FUNCTION) &&
                (kitronMeterHandlerControl.dataSize == KITRON_METER_HANDLER_POWER_FACTOR_DATA_SIZE) ){           
            
                if(maxResponseLen >= sizeof(Data_Readings)){
                    
                    KitronMeterHandler_ParsePowerFactorToDataReading((Data_Readings_Ptr) response, kitronMeterHandlerControl.data, kitronMeterHandlerControl.dataSize);
                    * responseLen = sizeof(Data_Readings);
                    * commadCallBack = READ_ACTIVE_POWER_MTR;
                    return KITRON_METER_HANDLER_NO_ERROR_CODE;
                }
            }
            
            break;
            
        case READ_ACTIVE_POWER_MTR:
            
            if( (kitronMeterHandlerControl.functionCommand == KITRON_METER_HANDLER_READ_COMMAND_FUNCTION) &&
                (kitronMeterHandlerControl.dataSize == KITRON_METER_HANDLER_ACTIVE_POWER_DATA_SIZE) ){           
            
                if(maxResponseLen >= sizeof(Data_Readings)){
                    
                    KitronMeterHandler_ParseActivePowerToDataReading((Data_Readings_Ptr) response, kitronMeterHandlerControl.data, kitronMeterHandlerControl.dataSize);
                    * responseLen = sizeof(Data_Readings);
                    * commadCallBack = NO_COMMAND_MTR;
                    return KITRON_METER_HANDLER_NO_ERROR_CODE;
                }
            }
            
            break;     
            
        /*case READ_ALL_APPARENT_POWER_MTR:
            
            if( (kitronMeterHandlerControl.functionCommand == KITRON_METER_HANDLER_READ_COMMAND_FUNCTION) &&
                (kitronMeterHandlerControl.dataSize == KITRON_METER_HANDLER_APPARENT_POWER_DATA_SIZE) ){           
            
                if(maxResponseLen >= sizeof(Data_Readings)){
                    
                    KitronMeterHandler_ParseApparentPowerToDataReading((Data_Readings_Ptr) response, kitronMeterHandlerControl.data, kitronMeterHandlerControl.dataSize);
                    * responseLen = sizeof(Data_Readings);
                    * commadCallBack = NO_COMMAND_MTR;
                    return KITRON_METER_HANDLER_NO_ERROR_CODE;
                }
            }
            
            break;*/
    }
    
    return KITRON_METER_HANDLER_COMMAND_FUNCTION_NOT_SUPPORTED_ERROR_CODE;
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

void KitronMeterHandler_ParseImportedActiveEnergyToDataReading( Data_Readings_Ptr dataReading, BYTE * data, WORD dataLen){
    
    KitronMeter_ImportedActiveEnergy_DataReading importedActiveEnergy;
    
    if(dataLen != KITRON_METER_HANDLER_IMPORTED_ACTIVE_ENERGY_DATA_SIZE)
        return;
    
    inverted_memcpy((BYTE *) &importedActiveEnergy, data, dataLen);    
    KitronMeterHandler_PrintImportedActiveEnergyData(&importedActiveEnergy);
}

void KitronMeterHandler_ParseExportedActiveEnergyToDataReading( Data_Readings_Ptr dataReading, BYTE * data, WORD dataLen){
    
    KitronMeter_ExportedActiveEnergy_DataReading exportedActiveEnergy;
    
    if(dataLen != KITRON_METER_HANDLER_EXPORTED_ACTIVE_ENERGY_DATA_SIZE)
        return;
    
    inverted_memcpy((BYTE *) &exportedActiveEnergy, data, dataLen);    
    KitronMeterHandler_PrintExportedActiveEnergyData(&exportedActiveEnergy);    
}

void KitronMeterHandler_ParseImportedReactiveEnergyToDataReading( Data_Readings_Ptr dataReading, BYTE * data, WORD dataLen){
    
    KitronMeter_ImportedReactiveEnergy_DataReading importedReactiveEnergy;
    
    if(dataLen != KITRON_METER_HANDLER_IMPORTED_REACTIVE_ENERGY_DATA_SIZE)
        return;
    
    inverted_memcpy((BYTE *) &importedReactiveEnergy, data, dataLen);    
    KitronMeterHandler_PrintImportedReactiveEnergyData(&importedReactiveEnergy);
}

void KitronMeterHandler_ParseExportedReactiveEnergyToDataReading( Data_Readings_Ptr dataReading, BYTE * data, WORD dataLen){
    
    KitronMeter_ExportedReactiveEnergy_DataReading exportedReactiveEnergy;
    
    if(dataLen != KITRON_METER_HANDLER_EXPORTED_REACTIVE_ENERGY_DATA_SIZE)
        return;
    
    inverted_memcpy((BYTE *) &exportedReactiveEnergy, data, dataLen);    
    KitronMeterHandler_PrintExportedReactiveEnergyData(&exportedReactiveEnergy);
}

void KitronMeterHandler_ParseVoltageToDataReading( Data_Readings_Ptr dataReading, BYTE * data, WORD dataLen){
    
    KitronMeter_Voltage_DataReading voltage;
    
    if(dataLen != KITRON_METER_HANDLER_VOLTAGE_DATA_SIZE)
        return;
    
    inverted_memcpy((BYTE *) &voltage, data, dataLen);    
    KitronMeterHandler_PrintVoltageData(&voltage);
    
}

void KitronMeterHandler_ParseCurrentToDataReading( Data_Readings_Ptr dataReading, BYTE * data, WORD dataLen){
    
    KitronMeter_Current_DataReading current;
    
    if(dataLen != KITRON_METER_HANDLER_CURRENT_DATA_SIZE)
        return;
    
    inverted_memcpy((BYTE *) &current, data, dataLen);    
    KitronMeterHandler_PrintCurrentData(&current);
}

void KitronMeterHandler_ParseActivePowerToDataReading( Data_Readings_Ptr dataReading, BYTE * data, WORD dataLen){
    
    KitronMeter_ActivePower_DataReading activePower;
    
    if(dataLen != KITRON_METER_HANDLER_ACTIVE_POWER_DATA_SIZE)
        return;
    
    inverted_memcpy((BYTE *) &activePower, data, dataLen);    
    KitronMeterHandler_PrintActivePowerData(&activePower);
}

void KitronMeterHandler_ParsePowerFactorToDataReading( Data_Readings_Ptr dataReading, BYTE * data, WORD dataLen){
    
    KitronMeter_PowerFactor_DataReading powerFactor;
    
    if(dataLen != KITRON_METER_HANDLER_POWER_FACTOR_DATA_SIZE)
        return;
    
    inverted_memcpy((BYTE *) &powerFactor, data, dataLen);    
    KitronMeterHandler_PrintPowerFactorData(&powerFactor);
}

/*void KitronMeterHandler_ParseApparentPowerToDataReading( Data_Readings_Ptr dataReading, BYTE * data, WORD dataLen){
    
    KitronMeter_ApparentPower_DataReading apparentPower;
    
    if(dataLen != KITRON_METER_HANDLER_APPARENT_POWER_DATA_SIZE)
        return;
    
    inverted_memcpy((BYTE *) &apparentPower, data, dataLen);    
    KitronMeterHandler_PrintApparentPowerData(&apparentPower);    
}*/

void KitronMeterHandler_PrintImportedActiveEnergyData(KitronMeter_ImportedActiveEnergy_DataReading_Ptr importedActiveEnergy){
    
    println_message("[KITRON METER IMPORTED ACTIVE ENERGY DATA INFORMATION]");    
    print_message("\n\r\t\tTotalImportedActiveEnergy: %f", importedActiveEnergy->TotalImportedActiveEnergy);
}

void KitronMeterHandler_PrintExportedActiveEnergyData(KitronMeter_ExportedActiveEnergy_DataReading_Ptr exportedActiveEnergy){
    
    println_message("[KITRON METER EXPORTED ACTIVE ENERGY DATA INFORMATION]");    
    print_message("\n\r\t\tTotalExportedActiveEnergy: %f", exportedActiveEnergy->TotalExportedActiveEnergy);    
}

void KitronMeterHandler_PrintImportedReactiveEnergyData(KitronMeter_ImportedReactiveEnergy_DataReading_Ptr importedReactiveEnergy){
    
    println_message("[KITRON METER IMPORTED REACTIVE ENERGY DATA INFORMATION]");    
    print_message("\n\r\t\tTotalImportedReactiveEnergy: %f", importedReactiveEnergy->TotalImportedReactiveEnergy);    
}

void KitronMeterHandler_PrintExportedReactiveEnergyData(KitronMeter_ExportedReactiveEnergy_DataReading_Ptr exportedReactiveEnergy){
    
    println_message("[KITRON METER EXPORTED REACTIVE ENERGY DATA INFORMATION]");    
    print_message("\n\r\t\tTotalExportedReactiveEnergy: %f", exportedReactiveEnergy->TotalExportedReactiveEnergy);    }

void KitronMeterHandler_PrintVoltageData(KitronMeter_Voltage_DataReading_Ptr voltage){
    
    println_message("[KITRON METER VOLTAGE DATA INFORMATION]");    
    print_message("\n\r\t\tVoltage (A): %f", voltage->VoltagePhaseA);
    print_message("\n\r\t\tVoltage (B): %f", voltage->VoltagePhaseB);
    print_message("\n\r\t\tVoltage (C): %f", voltage->VoltagePhaseC);    
}

void KitronMeterHandler_PrintCurrentData(KitronMeter_Current_DataReading_Ptr current){
    
    println_message("[KITRON METER CURRENT DATA INFORMATION]");    
    print_message("\n\r\t\tCurrent (A): %f", current->CurrentPhaseA);
    print_message("\n\r\t\tCurrent (B): %f", current->CurrentPhaseB);
    print_message("\n\r\t\tCurrent (C): %f", current->CurrentPhaseC);    
}

void KitronMeterHandler_PrintActivePowerData(KitronMeter_ActivePower_DataReading_Ptr activePower){
    
    println_message("[KITRON METER ACTIVE POWER DATA INFORMATION]");    
    print_message("\n\r\t\tTotalInstantaneousActivePower: %f", activePower->TotalInstantaneousActivePower);
    //!print_message("\n\r\t\tActivePower (A): %f", activePower->ActivePowerPhaseA);
    //!print_message("\n\r\t\tActivePower (B): %f", activePower->ActivePowerPhaseB);
    //!print_message("\n\r\t\tActivePower (C): %f", activePower->ActivePowerPhaseC);
}

void KitronMeterHandler_PrintPowerFactorData(KitronMeter_PowerFactor_DataReading_Ptr powerFactor){
    
    println_message("[KITRON METER POWER FACTOR DATA INFORMATION]");    
    print_message("\n\r\t\tTotalPowerFactor: %f", powerFactor->TotalPowerFactor);
    //!print_message("\n\r\t\tPowerFactor (A): %f", powerFactor->PowerFactorPhaseA);
    //!print_message("\n\r\t\tPowerFactor (B): %f", powerFactor->PowerFactorPhaseB);
    //!print_message("\n\r\t\tPowerFactor (C): %f", powerFactor->PowerFactorPhaseC);    
}

/*void KitronMeterHandler_PrintApparentPowerData(KitronMeter_ApparentPower_DataReading_Ptr apparentPower){
    
    println_message("[KITRON METER APPARENT POWER DATA INFORMATION]");    
    print_message("\n\r\t\tTotalInstantaneousApparentPower: %f", apparentPower->TotalInstantaneousApparentPower);
    print_message("\n\r\t\tApparentPowerPhase (A): %f", apparentPower->ApparentPowerPhaseA);
    print_message("\n\r\t\tApparentPowerPhase (B): %f", apparentPower->ApparentPowerPhaseB);
    print_message("\n\r\t\tApparentPowerPhase (C): %f", apparentPower->ApparentPowerPhaseC);
}*/
