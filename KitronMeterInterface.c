#include "KitronMeterInterface.h"

COMMAND_ID_FUNCTION KitronMeterInterface_CommandIdFunctionList [] = 
{
    {   READ_MODE ,                                         /* Meter Common Command Id              */
        KitronMeterInterface_ReadImportedActiveEnergyData } ,             /* Meter CommandIdFunction_Callback     */
        
    {   READ_EXPORTED_ACTIVE_ENERGY_MTR ,                                         /* Meter Common Command Id              */
        KitronMeterInterface_ReadExportedActiveEnergyData } ,             /* Meter CommandIdFunction_Callback     */    
        
    {   READ_IMPORTED_REACTIVE_ENERGY_MTR ,                                         /* Meter Common Command Id              */
        KitronMeterInterface_ReadImportedReactiveEnergyData } ,             /* Meter CommandIdFunction_Callback     */
        
    {   READ_EXPORTED_REACTIVE_ENERGY_MTR ,                                         /* Meter Common Command Id              */
        KitronMeterInterface_ReadExportedReactiveEnergyData } ,             /* Meter CommandIdFunction_Callback     */
        
    {   READ_ALL_VOLTAGE_MTR ,                                         /* Meter Common Command Id              */
        KitronMeterInterface_ReadAllVoltageData } ,             /* Meter CommandIdFunction_Callback     */
        
    {   READ_ALL_CURRENT_MTR ,                                         /* Meter Common Command Id              */
        KitronMeterInterface_ReadAllCurrentData } ,             /* Meter CommandIdFunction_Callback     */
        
    {   READ_ACTIVE_POWER_MTR ,                                         /* Meter Common Command Id              */
        KitronMeterInterface_ReadActivePowerData } ,             /* Meter CommandIdFunction_Callback     */
        
    {   READ_POWER_FACTOR_MTR ,                                         /* Meter Common Command Id              */
        KitronMeterInterface_ReadPowerFactorData } ,             /* Meter CommandIdFunction_Callback     */    
        
        COMMAND_ID_FUNCTION_NULL
};

//******************************************************************************
// KITRON Meter Interface API
//******************************************************************************
BOOL KitronMeterInterface_OBISBuildRequestMessageFrame( BYTE startCharacter, 
                                                        BYTE requestCommand, 
                                                        BYTE * deviceAddress,
                                                        WORD deviceAddressLen,
                                                        BYTE endCharacter,
                                                        WORD completionCharacter,
                                                        BYTE * frame,
                                                        WORD * frameLen){
    
    BYTE * frame_ptr = frame;
        
    memcpy(frame_ptr, &startCharacter, KITRON_METER_INTERFACE_OBIS_START_CHARACTER_SIZE);
    frame_ptr += KITRON_METER_INTERFACE_OBIS_START_CHARACTER_SIZE;
    
    memcpy(frame_ptr, &requestCommand, KITRON_METER_INTERFACE_OBIS_REQUEST_COMMAND_CHARACTER_SIZE);
    frame_ptr +=  KITRON_METER_INTERFACE_OBIS_REQUEST_COMMAND_CHARACTER_SIZE;
    
    if (deviceAddressLen >= KITRON_METER_INTERFACE_OBIS_DEVICE_ADDRESS_MAX_SIZE)
        return FALSE;
    
    memcpy(frame_ptr, deviceAddress, deviceAddressLen);
    frame_ptr += deviceAddressLen;    
    
    memcpy(frame_ptr, (BYTE *) &endCharacter, KITRON_METER_INTERFACE_OBIS_END_CHARACTER_SIZE);
    frame_ptr += KITRON_METER_INTERFACE_OBIS_END_CHARACTER_SIZE;
    
    inverted_memcpy(frame_ptr, (BYTE *) &completionCharacter, KITRON_METER_INTERFACE_OBIS_COMPLETION_CHARACTERS_SIZE);
    frame_ptr += KITRON_METER_INTERFACE_OBIS_COMPLETION_CHARACTERS_SIZE;    
        
    * frameLen =  (frame_ptr - frame);
    
    return TRUE;        
}

void KitronMeterInterface_OBISSendRequestMessageFrame(  BYTE startCharacter, 
                                                        BYTE requestCommand, 
                                                        BYTE * deviceAddress,
                                                        WORD deviceAddressLen,
                                                        BYTE endCharacter,
                                                        WORD completionCharacter){
    
    BYTE frame[KITRON_METER_INTERFACE_MAX_FRAME_BUFFER_SIZE];
    WORD frameLen;
    BOOL framebuilt;
    
    framebuilt = KitronMeterInterface_OBISBuildRequestMessageFrame( startCharacter, 
                                                                    requestCommand, 
                                                                    deviceAddress,
                                                                    deviceAddressLen,
                                                                    endCharacter,
                                                                    completionCharacter,
                                                                    frame,              /* frame            */
                                                                    &frameLen);         /* frameLen         */
    
    if(framebuilt == FALSE)
        return;
    
    KitronMeterInterface_OBISSendData(frame, frameLen);         
}

BOOL KitronMeterInterface_OBISBuildOptionSelectMessageFrame(BYTE acknowledgeCharacter,
                                                            BYTE protocolControlCharacter,
                                                            BYTE baudrateIndetification,
                                                            BYTE modeControlCharacter,
                                                            WORD completionCharacter,
                                                            BYTE * frame,
                                                            WORD * frameLen){
    
    BYTE * frame_ptr = frame;
        
    memcpy(frame_ptr, &acknowledgeCharacter, KITRON_METER_INTERFACE_OBIS_ACK_CHARACTER_SIZE);
    frame_ptr += KITRON_METER_INTERFACE_OBIS_ACK_CHARACTER_SIZE;
    
    memcpy(frame_ptr, &protocolControlCharacter, KITRON_METER_INTERFACE_OBIS_PROTOCOL_CONTROL_CHARACTER_SIZE);
    frame_ptr +=  KITRON_METER_INTERFACE_OBIS_PROTOCOL_CONTROL_CHARACTER_SIZE;
    
    memcpy(frame_ptr, &baudrateIndetification, KITRON_METER_INTERFACE_OBIS_BAUDRATE_IDENTIFICATION_CHARACTER_SIZE);
    frame_ptr +=  KITRON_METER_INTERFACE_OBIS_BAUDRATE_IDENTIFICATION_CHARACTER_SIZE;
    
    memcpy(frame_ptr, &modeControlCharacter, KITRON_METER_INTERFACE_OBIS_MODE_CONTROL_CHARACTER_SIZE);
    frame_ptr +=  KITRON_METER_INTERFACE_OBIS_MODE_CONTROL_CHARACTER_SIZE;
            
    inverted_memcpy(frame_ptr, (BYTE *) &completionCharacter, KITRON_METER_INTERFACE_OBIS_COMPLETION_CHARACTERS_SIZE);
    frame_ptr += KITRON_METER_INTERFACE_OBIS_COMPLETION_CHARACTERS_SIZE;    
        
    * frameLen =  (frame_ptr - frame);
    
    return TRUE;    
}

void KitronMeterInterface_OBISSendOptionSelectMessageFrame( BYTE acknowledgeCharacter,
                                                            BYTE protocolControlCharacter,
                                                            BYTE baudrateIndetification,
                                                            BYTE modeControlCharacter,
                                                            WORD completionCharacter){
    
    BYTE frame[KITRON_METER_INTERFACE_MAX_FRAME_BUFFER_SIZE];
    WORD frameLen;
    BOOL framebuilt;
    
    framebuilt = KitronMeterInterface_OBISBuildOptionSelectMessageFrame( acknowledgeCharacter,
                                                                         protocolControlCharacter,
                                                                         baudrateIndetification,
                                                                         modeControlCharacter,
                                                                         completionCharacter,
                                                                         frame,              /* frame            */
                                                                         &frameLen);         /* frameLen         */
    
    if(framebuilt == FALSE)
        return;
    
    KitronMeterInterface_OBISSendData(frame, frameLen);     
}

BOOL KitronMeterInterface_OBISBuildCommandMessageFrame( BYTE startOfHeader,
                                                        BYTE commandMessageIdentifier,
                                                        BYTE commandTypeIdentifier,
                                                        BYTE frameStartCharacter,
                                                        BYTE * dataSet,
                                                        WORD dataSetLen,
                                                        BYTE frameEndCharacter,
                                                        BYTE * frame,
                                                        WORD * frameLen){
    
    BYTE * frame_ptr = frame;
        
    memcpy(frame_ptr, &startOfHeader, KITRON_METER_INTERFACE_OBIS_START_OF_HEADER_CHARACTER_SIZE);
    frame_ptr += KITRON_METER_INTERFACE_OBIS_START_OF_HEADER_CHARACTER_SIZE;
    
    memcpy(frame_ptr, &commandMessageIdentifier, KITRON_METER_INTERFACE_OBIS_COMMAND_MESSAGE_CHARACTER_SIZE);
    frame_ptr +=  KITRON_METER_INTERFACE_OBIS_COMMAND_MESSAGE_CHARACTER_SIZE;
    
    memcpy(frame_ptr, &commandTypeIdentifier, KITRON_METER_INTERFACE_OBIS_COMMAND_TYPE_CHARACTER_SIZE);
    frame_ptr +=  KITRON_METER_INTERFACE_OBIS_COMMAND_TYPE_CHARACTER_SIZE;
    
    if (frameStartCharacter != KITRON_METER_INTERFACE_OBIS_NULL_CHARACTER){
        
        memcpy(frame_ptr, &frameStartCharacter, KITRON_METER_INTERFACE_OBIS_FRAME_START_CHARACTER_SIZE);
        frame_ptr +=  KITRON_METER_INTERFACE_OBIS_FRAME_START_CHARACTER_SIZE;
    }
    
    if (dataSetLen >= KITRON_METER_INTERFACE_OBIS_DATA_SET_MAX_SIZE)
        return FALSE;

    memcpy(frame_ptr, dataSet, dataSetLen);
    frame_ptr += dataSetLen;    


    if (frameEndCharacter != KITRON_METER_INTERFACE_OBIS_NULL_CHARACTER){
        
        memcpy(frame_ptr, &frameEndCharacter, KITRON_METER_INTERFACE_OBIS_FRAME_END_CHARACTER_SIZE);
        frame_ptr +=  KITRON_METER_INTERFACE_OBIS_FRAME_END_CHARACTER_SIZE;
    }
    
    * frameLen = (frame_ptr - frame);
    
    return TRUE;        
}
                                         
void KitronMeterInterface_OBISSendCommandMessageFrame(  BYTE startOfHeader,
                                                        BYTE commandMessageIdentifier,
                                                        BYTE commandTypeIdentifier,
                                                        BYTE frameStartCharacter,
                                                        BYTE *dataSet,
                                                        WORD dataSetLen,
                                                        BYTE frameEndCharacter){
    
    BYTE frame[KITRON_METER_INTERFACE_MAX_FRAME_BUFFER_SIZE];
    WORD frameLen;
    BOOL framebuilt;
    
    framebuilt = KitronMeterInterface_OBISBuildCommandMessageFrame(  startOfHeader,
                                                                    commandMessageIdentifier,
                                                                    commandTypeIdentifier,
                                                                    frameStartCharacter,
                                                                    dataSet,
                                                                    dataSetLen,
                                                                    frameEndCharacter,
                                                                    frame,              /* frame            */
                                                                    &frameLen);         /* frameLen         */
    
    if(framebuilt == FALSE)
        return;
    
    KitronMeterInterface_OBISSendData(frame, frameLen);        
}

void KitronMeterInterface_OBISSendData(BYTE * frame, WORD frameLen){
    
    ComSerialInterface_Setup( COM_SERIAL_INTERFACE_HANDLER_9600_BAUDRATE, 
                              COM_SERIAL_INTERFACE_HANDLER_BYTE_7_SIZE, 
                              COM_SERIAL_INTERFACE_HANDLER_ONE_STOP_BIT, 
                              COM_SERIAL_INTERFACE_HANDLER_EVEN_PARITY);
    //print_message(frame);
    ComSerialInterface_SendData(frame, frameLen);        
}
BOOL KitronMeterInterface_ModbusBuildFrame(   BYTE modbusId,                                        
                                              BYTE commandFunction,
                                              WORD registerAddress, 
                                              WORD registerAmount, 
                                              BYTE * data, 
                                              BYTE dataLen, 
                                              BYTE * frame, 
                                              BYTE * frameLen){
    
    BYTE * frame_ptr = frame;
    WORD crcFrame;
    
    memcpy(frame_ptr, &modbusId, KITRON_METER_INTERFACE_MODBUS_ID_HEADER_SIZE);
    frame_ptr += KITRON_METER_INTERFACE_MODBUS_ID_HEADER_SIZE;
    
    memcpy(frame_ptr, &commandFunction, KITRON_METER_INTERFACE_COMMAND_FUNCTION_HEADER_SIZE);
    frame_ptr +=  KITRON_METER_INTERFACE_COMMAND_FUNCTION_HEADER_SIZE;
    
    inverted_memcpy(frame_ptr, (BYTE *) &registerAddress, KITRON_METER_INTERFACE_REGISTER_ADDRESS_HEADER_SIZE);
    frame_ptr += KITRON_METER_INTERFACE_REGISTER_ADDRESS_HEADER_SIZE;
    
    inverted_memcpy(frame_ptr, (BYTE *) &registerAmount, KITRON_METER_INTERFACE_REGISTER_AMOUNT_HEADER_SIZE);
    frame_ptr += KITRON_METER_INTERFACE_REGISTER_AMOUNT_HEADER_SIZE;
    
    switch(commandFunction){
        
        case KITRON_METER_INTERFACE_READ_COMMAND_FUNCTION:            
            break;
            
        default:
            return FALSE;
    }
    
    // Calculing CRC Value
    crcFrame = wfnCRC_CALC((BYTE *) frame, (frame_ptr - frame), KITRON_METER_INTERFACE_CRC_BASE);
    
    // Adding CRC into frame buffer
    memcpy(frame_ptr, (BYTE *) &crcFrame, KITRON_METER_INTERFACE_CRC_HEADER_SIZE);
    frame_ptr += KITRON_METER_INTERFACE_CRC_HEADER_SIZE;
    
    * frameLen = (BYTE) (frame_ptr - frame);
    
    return TRUE;    
}

void KitronMeterInterface_ModbusSendFrame( BYTE modbusId,                                     
                                     BYTE commandFunction,
                                     WORD registerAddress, 
                                     WORD registerAmount, 
                                     BYTE * data, 
                                     BYTE dataLen ){
    
    BYTE frame[KITRON_METER_INTERFACE_MAX_FRAME_BUFFER_SIZE];
    BYTE frameLen;
    BOOL framebuilt;
    
    framebuilt = KitronMeterInterface_ModbusBuildFrame(   modbusId,           /* modbusId         */
                                                    commandFunction,    /* commandFunction  */     
                                                    registerAddress,    /* registerAddress, */
                                                    registerAmount,     /* registerAmount   */
                                                    data,               /* data             */
                                                    dataLen,            /* dataLen          */
                                                    frame,              /* frame            */
                                                    &frameLen);         /* frameLen         */
    
    if(framebuilt == FALSE)
        return;
    //!print_info("Sent Data: ");   
    //! ComSerialInterface_PrintData(frame, frameLen);        
    KitronMeterInterface_ModbusSendData(frame, frameLen);        
}

void KitronMeterInterface_ModbusSendData(BYTE * frame, WORD frameLen){
    
    ComSerialInterface_Setup( COM_SERIAL_INTERFACE_HANDLER_9600_BAUDRATE, 
                              COM_SERIAL_INTERFACE_HANDLER_BYTE_8_SIZE, 
                              COM_SERIAL_INTERFACE_HANDLER_ONE_STOP_BIT, 
                              COM_SERIAL_INTERFACE_HANDLER_NO_PARITY);
    //print_message(frame);
    ComSerialInterface_SendData(frame, frameLen);        
}

void KitronMeterInterface_ReadImportedActiveEnergyData(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen){
    
    print_info("Requesting READ IMPORTED ACTIVE ENERGY DATA Command");
    
    KitronMeterInterface_ModbusSendFrame( modbusId,                                     
                                    KITRON_METER_INTERFACE_READ_COMMAND_FUNCTION,
                                    KITRON_METER_INTERFACE_TOTAL_IMPORT_ACTIVE_ENERGY_REGISTER_ADDRESS,
                                    KITRON_METER_INTERFACE_TOTAL_IMPORT_ACTIVE_ENERGY_REGISTER_ADDRESS_SIZE,
                                    NULL,
                                    0);      
}

void KitronMeterInterface_ReadExportedActiveEnergyData(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen){
    
    print_info("Requesting READ EXPORTED ACTIVE ENERGY DATA Command");
    
    KitronMeterInterface_ModbusSendFrame( modbusId,                                     
                                    KITRON_METER_INTERFACE_READ_COMMAND_FUNCTION,
                                    KITRON_METER_INTERFACE_TOTAL_EXPORT_ACTIVE_ENERGY_REGISTER_ADDRESS,
                                    KITRON_METER_INTERFACE_TOTAL_EXPORT_ACTIVE_ENERGY_REGISTER_ADDRESS_SIZE,
                                    NULL,
                                    0);      
}

void KitronMeterInterface_ReadImportedReactiveEnergyData(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen){
    
    print_info("Requesting READ IMPORTED REACTIVE ENERGY DATA Command");
    
    KitronMeterInterface_ModbusSendFrame( modbusId,                                     
                                    KITRON_METER_INTERFACE_READ_COMMAND_FUNCTION,
                                    KITRON_METER_INTERFACE_TOTAL_IMPORT_REACTIVE_ENERGY_REGISTER_ADDRESS,
                                    KITRON_METER_INTERFACE_TOTAL_IMPORT_REACTIVE_ENERGY_REGISTER_ADDRESS_SIZE,
                                    NULL,
                                    0);      
}

void KitronMeterInterface_ReadExportedReactiveEnergyData(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen){
    
    print_info("Requesting READ EXPORTED REACTIVE ENERGY DATA Command");
    
    KitronMeterInterface_ModbusSendFrame( modbusId,                                     
                                    KITRON_METER_INTERFACE_READ_COMMAND_FUNCTION,
                                    KITRON_METER_INTERFACE_TOTAL_EXPORT_REACTIVE_ENERGY_REGISTER_ADDRESS,
                                    KITRON_METER_INTERFACE_TOTAL_EXPORT_REACTIVE_ENERGY_REGISTER_ADDRESS_SIZE,
                                    NULL,
                                    0);      
    
}

void KitronMeterInterface_ReadAllVoltageData(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen){
    
    print_info("Requesting READ ALL VOLTAGE DATA Command");
    
    KitronMeterInterface_ModbusSendFrame( modbusId,                                     
                                    KITRON_METER_INTERFACE_READ_COMMAND_FUNCTION,
                                    KITRON_METER_INTERFACE_ALL_VOLTAGE_PHASE_REGISTER_ADDRESS,
                                    KITRON_METER_INTERFACE_ALL_VOLTAGE_PHASE_REGISTER_ADDRESS_SIZE,
                                    NULL,
                                    0);      
}

void KitronMeterInterface_ReadAllCurrentData(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen){
    
    print_info("Requesting READ ALL CURRENT DATA Command");
    
    KitronMeterInterface_ModbusSendFrame( modbusId,                                     
                                    KITRON_METER_INTERFACE_READ_COMMAND_FUNCTION,
                                    KITRON_METER_INTERFACE_ALL_CURRENT_PHASE_REGISTER_ADDRESS,
                                    KITRON_METER_INTERFACE_ALL_CURRENT_PHASE_REGISTER_ADDRESS_SIZE,
                                    NULL,
                                    0);      
}

void KitronMeterInterface_ReadPowerFactorData(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen){
    
    print_info("Requesting READ POWER FACTOR DATA Command");
    
    KitronMeterInterface_ModbusSendFrame( modbusId,                                     
                                    KITRON_METER_INTERFACE_READ_COMMAND_FUNCTION,
                                    KITRON_METER_INTERFACE_TOTAL_POWER_FACTOR_REGISTER_ADDRESS,
                                    KITRON_METER_INTERFACE_TOTAL_POWER_FACTOR_REGISTER_ADDRESS_SIZE,
                                    NULL,
                                    0);      
}

void KitronMeterInterface_ReadActivePowerData(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen){
    
    print_info("Requesting READ ACTIVE POWER DATA Command");
    
    KitronMeterInterface_ModbusSendFrame( modbusId,                                     
                                    KITRON_METER_INTERFACE_READ_COMMAND_FUNCTION,
                                    KITRON_METER_INTERFACE_TOTAL_INSTANTANEOUS_ACTIVE_POWER_REGISTER_ADDRESS,
                                    KITRON_METER_INTERFACE_TOTAL_INSTANTANEOUS_ACTIVE_POWER_REGISTER_ADDRESS_SIZE,
                                    NULL,
                                    0);      
}

/*void KitronMeterInterface_ReadAllApparentPowerData(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen){
    
    print_info("Requesting READ ALL APPARENT POWER DATA Command");
    
    KitronMeterInterface_SendFrame( modbusId,                                     
                                    KITRON_METER_INTERFACE_READ_COMMAND_FUNCTION,
                                    KITRON_METER_INTERFACE_ALL_APPARENT_POWER_PHASE_REGISTER_ADDRESS,
                                    KITRON_METER_INTERFACE_ALL_APPARENT_POWER_PHASE_REGISTER_ADDRESS_SIZE,
                                    NULL,
                                    0);      
}*/