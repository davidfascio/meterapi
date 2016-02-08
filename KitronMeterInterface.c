#include "KitronMeterInterface.h"

COMMAND_ID_FUNCTION KitronMeterInterface_CommandIdFunctionList [] = 
{
    {   READ_MODE ,                                         /* Meter Common Command Id              */
        KitronMeterInterface_ReadAllVoltageData } ,             /* Meter CommandIdFunction_Callback     */
        
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

void KitronMeterInterface_ReadAllActivePowerData(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen){
    
    print_info("Requesting READ ALL ACTIVE POWER DATA Command");
    
    KitronMeterInterface_SendFrame( modbusId,                                     
                                    KITRON_METER_INTERFACE_READ_COMMAND_FUNCTION,
                                    KITRON_METER_INTERFACE_ALL_ACTIVE_POWER_PHASE_REGISTER_ADDRESS,
                                    KITRON_METER_INTERFACE_ALL_ACTIVE_POWER_PHASE_REGISTER_ADDRESS_SIZE,
                                    NULL,
                                    0);      
}

void KitronMeterInterface_ReadAllPowerFactorData(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen){
    
    print_info("Requesting READ ALL POWER FACTOR DATA Command");
    
    KitronMeterInterface_SendFrame( modbusId,                                     
                                    KITRON_METER_INTERFACE_READ_COMMAND_FUNCTION,
                                    KITRON_METER_INTERFACE_ALL_POWER_FACTOR_PHASE_REGISTER_ADDRESS,
                                    KITRON_METER_INTERFACE_ALL_POWER_FACTOR_PHASE_REGISTER_ADDRESS_SIZE,
                                    NULL,
                                    0);      
}

void KitronMeterInterface_ReadAllApparentPowerData(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen){
    
    print_info("Requesting READ ALL APPARENT POWER DATA Command");
    
    KitronMeterInterface_SendFrame( modbusId,                                     
                                    KITRON_METER_INTERFACE_READ_COMMAND_FUNCTION,
                                    KITRON_METER_INTERFACE_ALL_APPARENT_POWER_PHASE_REGISTER_ADDRESS,
                                    KITRON_METER_INTERFACE_ALL_APPARENT_POWER_PHASE_REGISTER_ADDRESS_SIZE,
                                    NULL,
                                    0);      
}