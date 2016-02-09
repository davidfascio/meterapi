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
// G155 Meter Interface API
//******************************************************************************
BOOL KitronMeterInterface_BuildFrame(   BYTE modbusId,                                        
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

void KitronMeterInterface_SendFrame( BYTE modbusId,                                     
                                     BYTE commandFunction,
                                     WORD registerAddress, 
                                     WORD registerAmount, 
                                     BYTE * data, 
                                     BYTE dataLen ){
    
    BYTE frame[KITRON_METER_INTERFACE_MAX_FRAME_BUFFER_SIZE];
    BYTE frameLen;
    BOOL framebuilt;
    
    framebuilt = KitronMeterInterface_BuildFrame(   modbusId,           /* modbusId         */
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
    ComSerialInterface_SendData(frame, frameLen);        
}

void KitronMeterInterface_ReadImportedActiveEnergyData(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen){
    
    print_info("Requesting READ IMPORTED ACTIVE ENERGY DATA Command");
    
    KitronMeterInterface_SendFrame( modbusId,                                     
                                    KITRON_METER_INTERFACE_READ_COMMAND_FUNCTION,
                                    KITRON_METER_INTERFACE_TOTAL_IMPORT_ACTIVE_ENERGY_REGISTER_ADDRESS,
                                    KITRON_METER_INTERFACE_TOTAL_IMPORT_ACTIVE_ENERGY_REGISTER_ADDRESS_SIZE,
                                    NULL,
                                    0);      
}

void KitronMeterInterface_ReadExportedActiveEnergyData(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen){
    
    print_info("Requesting READ EXPORTED ACTIVE ENERGY DATA Command");
    
    KitronMeterInterface_SendFrame( modbusId,                                     
                                    KITRON_METER_INTERFACE_READ_COMMAND_FUNCTION,
                                    KITRON_METER_INTERFACE_TOTAL_EXPORT_ACTIVE_ENERGY_REGISTER_ADDRESS,
                                    KITRON_METER_INTERFACE_TOTAL_EXPORT_ACTIVE_ENERGY_REGISTER_ADDRESS_SIZE,
                                    NULL,
                                    0);      
}

void KitronMeterInterface_ReadImportedReactiveEnergyData(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen){
    
    print_info("Requesting READ IMPORTED REACTIVE ENERGY DATA Command");
    
    KitronMeterInterface_SendFrame( modbusId,                                     
                                    KITRON_METER_INTERFACE_READ_COMMAND_FUNCTION,
                                    KITRON_METER_INTERFACE_TOTAL_IMPORT_REACTIVE_ENERGY_REGISTER_ADDRESS,
                                    KITRON_METER_INTERFACE_TOTAL_IMPORT_REACTIVE_ENERGY_REGISTER_ADDRESS_SIZE,
                                    NULL,
                                    0);      
}

void KitronMeterInterface_ReadExportedReactiveEnergyData(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen){
    
    print_info("Requesting READ EXPORTED REACTIVE ENERGY DATA Command");
    
    KitronMeterInterface_SendFrame( modbusId,                                     
                                    KITRON_METER_INTERFACE_READ_COMMAND_FUNCTION,
                                    KITRON_METER_INTERFACE_TOTAL_EXPORT_REACTIVE_ENERGY_REGISTER_ADDRESS,
                                    KITRON_METER_INTERFACE_TOTAL_EXPORT_REACTIVE_ENERGY_REGISTER_ADDRESS_SIZE,
                                    NULL,
                                    0);      
    
}

void KitronMeterInterface_ReadAllVoltageData(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen){
    
    print_info("Requesting READ ALL VOLTAGE DATA Command");
    
    KitronMeterInterface_SendFrame( modbusId,                                     
                                    KITRON_METER_INTERFACE_READ_COMMAND_FUNCTION,
                                    KITRON_METER_INTERFACE_ALL_VOLTAGE_PHASE_REGISTER_ADDRESS,
                                    KITRON_METER_INTERFACE_ALL_VOLTAGE_PHASE_REGISTER_ADDRESS_SIZE,
                                    NULL,
                                    0);      
}

void KitronMeterInterface_ReadAllCurrentData(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen){
    
    print_info("Requesting READ ALL CURRENT DATA Command");
    
    KitronMeterInterface_SendFrame( modbusId,                                     
                                    KITRON_METER_INTERFACE_READ_COMMAND_FUNCTION,
                                    KITRON_METER_INTERFACE_ALL_CURRENT_PHASE_REGISTER_ADDRESS,
                                    KITRON_METER_INTERFACE_ALL_CURRENT_PHASE_REGISTER_ADDRESS_SIZE,
                                    NULL,
                                    0);      
}

void KitronMeterInterface_ReadPowerFactorData(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen){
    
    print_info("Requesting READ POWER FACTOR DATA Command");
    
    KitronMeterInterface_SendFrame( modbusId,                                     
                                    KITRON_METER_INTERFACE_READ_COMMAND_FUNCTION,
                                    KITRON_METER_INTERFACE_TOTAL_POWER_FACTOR_REGISTER_ADDRESS,
                                    KITRON_METER_INTERFACE_TOTAL_POWER_FACTOR_REGISTER_ADDRESS_SIZE,
                                    NULL,
                                    0);      
}

void KitronMeterInterface_ReadActivePowerData(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen){
    
    print_info("Requesting READ ACTIVE POWER DATA Command");
    
    KitronMeterInterface_SendFrame( modbusId,                                     
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