#include "KitronMeterHandler.h"

KITRON_METER_HANDLER kitronMeterHandlerControl;
KITRON_METER_HANDLER_OBIS kitronMeterHandlerOBISControl;

void KitronMeterHandler_ModbusSetup(BYTE modbusId, BYTE functionCommand, BYTE dataSize, BYTE * data){
    
    kitronMeterHandlerControl.modbusId = modbusId;
    kitronMeterHandlerControl.functionCommand = functionCommand;
    kitronMeterHandlerControl.dataSize = dataSize;
    memcpy(kitronMeterHandlerControl.data, data, dataSize);
}

WORD API_KitronMeterHandler_ModbusReceiveHandler( BYTE * buffer, WORD  buffersize, METER_DESCRIPTOR_PTR meterDescriptor, BYTE * commandCallBack){
    
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
    
    KitronMeterHandler_ModbusSetup(modbusId, functionCommand, dataSize, data);
    
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

WORD API_KitronMeterHandler_OBISReceiveHandler( BYTE * buffer, WORD  buffersize, METER_DESCRIPTOR_PTR meterDescriptor, BYTE * commandCallBack){
    
    BYTE * buffer_ptr = buffer;
    BYTE startCharacter;
    WORD error_code;
    
    BYTE modbusId;
    BYTE functionCommand;
    BYTE dataSize;
    BYTE * data;
    BYTE bcc;
    
    bcc = wfnBCC_CALC(buffer, buffersize - KITRON_METER_HANDLER_OBIS_BCC_HEADER_SIZE, 0);
        
    if (memcmp(buffer + buffersize - KITRON_METER_HANDLER_OBIS_BCC_HEADER_SIZE, 
                &bcc, 
                KITRON_METER_HANDLER_OBIS_BCC_HEADER_SIZE))    
        return KITRON_METER_HANDLER_OBIS_WRONG_BCC_ERROR_CODE;
    
    memcpy(&startCharacter, buffer_ptr, KITRON_METER_HANDLER_OBIS_START_CHARACTER_HEADER_SIZE );
    buffer_ptr += KITRON_METER_HANDLER_OBIS_START_CHARACTER_HEADER_SIZE;
    
    switch(startCharacter){
        
        // OBIS Identification Message
        case KITRON_METER_HANDLER_OBIS_START_CHARACTER:
            
            error_code = KitronMeterHandler_OBISReceiveIdentificationMessage(buffer, buffersize);
            break;
            
        // OBIS Command Message    
        case KITRON_METER_HANDLER_OBIS_START_OF_HEADER_CHARACTER:
            
            error_code = KitronMeterHandler_OBISReceiveCommandMessage(buffer, buffersize);
            break;
        
        // OBIS Data Message    
        case KITRON_METER_HANDLER_OBIS_FRAME_START_CHARACTER:
            
            error_code = KitronMeterHandler_OBISReceiveDataMessage(buffer, buffersize);
            break;
            
        default:
            return KITRON_METER_HANDLER_OBIS_MESSAGE_IS_NOT_SUPPORTED_ERROR_CODE;
    }
    
    KitronMeterHandler_ModbusSetup(modbusId, functionCommand, dataSize, data);
    
    MeterDescriptor_Setup(meterDescriptor, modbusId, NULL, 0, KITRON_METER_TYPE);    
    * commandCallBack = API_KitronMeterHandler_GetInvokeFunctionId(functionCommand);
    
    return KITRON_METER_HANDLER_NO_ERROR_CODE;
}

WORD KitronMeterHandler_OBISReceiveIdentificationMessage(BYTE * buffer, WORD buffersize){
    
    BYTE * buffer_ptr = buffer;
    BYTE startCharacter;
    
    memcpy(&startCharacter, buffer_ptr, KITRON_METER_HANDLER_OBIS_START_CHARACTER_HEADER_SIZE);
    buffer_ptr += KITRON_METER_HANDLER_OBIS_START_CHARACTER_HEADER_SIZE;
    
    if (startCharacter != KITRON_METER_HANDLER_OBIS_START_CHARACTER)
        return KITRON_METER_HANDLER_OBIS_IS_NOT_IDENTIFICATION_MESSAGE_ERROR_CODE;    
    
    kitronMeterHandlerOBISControl.identification.manufacturesIdentificationLen = 
            KITRON_METER_HANDLER_OBIS_MANUFACTURES_IDENTIFICATION_HEADER_SIZE;            
            
    memcpy(kitronMeterHandlerOBISControl.identification.manufacturesIdentification, 
           buffer_ptr, kitronMeterHandlerOBISControl.identification.manufacturesIdentificationLen);                        


    buffer_ptr += KITRON_METER_HANDLER_OBIS_MANUFACTURES_IDENTIFICATION_HEADER_SIZE;


    kitronMeterHandlerOBISControl.identification.baudrateIdentification = * buffer_ptr;            
    buffer_ptr += KITRON_METER_HANDLER_OBIS_BAUD_RATE_IDENTIFICATION_HEADER_SIZE;


    BYTE * identification_end_ptr = memchr( buffer_ptr, 
                                            KITRON_METER_HANDLER_OBIS_FIRST_COMPLETION_CHARACTER, 
                                            buffersize - (buffer_ptr - buffer));

    if(identification_end_ptr == NULL)
        return KITRON_METER_HANDLER_OBIS_NULL_PTR_ERROR_CODE;

    kitronMeterHandlerOBISControl.identification.identificationLen = identification_end_ptr - buffer_ptr;

    memcpy( kitronMeterHandlerOBISControl.identification.identification, 
            buffer_ptr, 
            kitronMeterHandlerOBISControl.identification.identificationLen);     
    
    return KITRON_METER_HANDLER_NO_ERROR_CODE;
}

WORD KitronMeterHandler_OBISReceiveCommandMessage(BYTE * buffer, WORD buffersize){
    
    BYTE * buffer_ptr = buffer;
    BYTE startCharacter;
    
    
    startCharacter = * buffer_ptr;
    buffer_ptr += KITRON_METER_HANDLER_OBIS_START_CHARACTER_HEADER_SIZE;
    
    if (startCharacter != KITRON_METER_HANDLER_OBIS_START_OF_HEADER_CHARACTER)
        return KITRON_METER_HANDLER_OBIS_IS_NOT_COMMAND_MESSAGE_ERROR_CODE;    
    
    kitronMeterHandlerOBISControl.commandMessage.commandMessageIdentifier = * buffer_ptr;
    buffer_ptr += KITRON_METER_HANDLER_OBIS_COMMAND_MESSAGE_IDENTIFIER_HEADER_SIZE;
    
    kitronMeterHandlerOBISControl.commandMessage.commandTypeIdentifier = * buffer_ptr;
    buffer_ptr += KITRON_METER_HANDLER_OBIS_COMMAND_TYPE_IDENTIFIER_HEADER_SIZE;  
        
    return KitronMeterHandler_OBISReceiveDataMessage(buffer_ptr, buffersize - (buffer_ptr - buffer));
}

WORD KitronMeterHandler_OBISReceiveDataMessage(BYTE * buffer, WORD buffersize){
    
    BYTE * buffer_ptr = buffer;
    BYTE frameStartCharacter;
    BYTE frontBoundaryCharacter;
    BYTE rearBoundaryCharacter;
    
    frameStartCharacter = * buffer_ptr;
    buffer_ptr += KITRON_METER_HANDLER_OBIS_START_CHARACTER_HEADER_SIZE;
    
    if (frameStartCharacter != KITRON_METER_HANDLER_OBIS_FRAME_START_CHARACTER)
        return KITRON_METER_HANDLER_OBIS_IS_NOT_DATA_MESSAGE_ERROR_CODE;
    
    frontBoundaryCharacter = * buffer_ptr;
    buffer_ptr += KITRON_METER_HANDLER_OBIS_BOUNDARY_CHARACTER_HEADER_SIZE;
    
    if (frontBoundaryCharacter != KITRON_METER_HANDLER_OBIS_FRONT_BOUNDARY_CHARACTER)
        return KITRON_METER_HANDLER_OBIS_WRONG_BOUNDARY_CHARACTER_ERROR_SIZE;
    
    BYTE * dataSet_end_ptr = memchr(buffer_ptr, KITRON_METER_HANDLER_OBIS_REAR_BOUNDARY_CHARACTER, buffersize - (buffer_ptr - buffer));
    
    if(dataSet_end_ptr == NULL)
        return KITRON_METER_HANDLER_OBIS_NULL_PTR_ERROR_CODE;

    kitronMeterHandlerOBISControl.commandMessage.dataMessage.dataSetLen = dataSet_end_ptr - buffer_ptr;
    
    memcpy( kitronMeterHandlerOBISControl.commandMessage.dataMessage.dataSet, 
            buffer_ptr,
            kitronMeterHandlerOBISControl.commandMessage.dataMessage.dataSetLen);
    
    return KITRON_METER_HANDLER_NO_ERROR_CODE;
}

WORD API_KitronMeterHandler_ReceiveHandler( BYTE * buffer, WORD  buffersize, METER_DESCRIPTOR_PTR meterDescriptor, BYTE * commandCallBack){
    
    WORD error_code;
    
    error_code = API_KitronMeterHandler_ModbusReceiveHandler( buffer, buffersize, meterDescriptor, commandCallBack);
    
    if (error_code == KITRON_METER_HANDLER_NO_ERROR_CODE)
        return error_code;
    
    return API_KitronMeterHandler_OBISReceiveHandler( buffer, buffersize, meterDescriptor, commandCallBack);
}
