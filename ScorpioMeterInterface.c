#include "ScorpioMeterInterface.h"

COMMAND_ID_FUNCTION ScorpioMeterInterface_CommandIdFunctionList [] = 
{
    {   PSW_MTR,                                            /* Meter Common Command Id              */
        ScorpioMeterInterface_SendPassword  },              /* Meter CommandIdFunction_Callback     */
    
    {   Dis_MTR,                                            /* Meter Common Command Id              */
        ScorpioMeterInterface_Disconnect  },                /* Meter CommandIdFunction_Callback     */
        
    {   Con_MTR,                                            /* Meter Common Command Id              */
        ScorpioMeterInterface_Connect  },                   /* Meter CommandIdFunction_Callback     */
    
    {   Res_MTR,                                            /* Meter Common Command Id              */
        ScorpioMeterInterface_Reset  },                   /* Meter CommandIdFunction_Callback     */        
        
    {   READ_MODE ,                                         /* Meter Common Command Id              */
        ScorpioMeterInterface_ReadMeteringData } ,             /* Meter CommandIdFunction_Callback     */
        
    {   LINK_ADDING_MTR ,                                         /* Meter Common Command Id              */
        ScorpioMeterInterface_LinkMeter } ,             /* Meter CommandIdFunction_Callback     */     
        
    {   LINK_DELETING_MTR,                                         /* Meter Common Command Id              */
        ScorpioMeterInterface_LinkMeter } ,             /* Meter CommandIdFunction_Callback     */     
        
    COMMAND_ID_FUNCTION_NULL
};

BOOL ScorpioMeterInterface_BuildFrame(  BYTE * serialNumber,
                                        WORD serialNumberLen,
                                        WORD commandFunction,
                                        WORD registerAddress, 
                                        WORD registerAmount, 
                                        BYTE * data, 
                                        BYTE dataLen, 
                                        BYTE * frame, 
                                        BYTE * frameLen)
{
    BYTE * frame_ptr =  frame;
    WORD crcFrame;
    
    if(serialNumberLen != Lenght_Meter_ID)
        return FALSE;
    
    memcpy(frame_ptr, serialNumber, serialNumberLen);
    frame_ptr += serialNumberLen;
    
    inverted_memcpy(frame_ptr, (BYTE *) &commandFunction, SCORPIO_METER_INTERFACE_COMMAND_FUNCTION_HEADER_SIZE);
    frame_ptr += SCORPIO_METER_INTERFACE_COMMAND_FUNCTION_HEADER_SIZE;
    
    inverted_memcpy(frame_ptr, (BYTE *) &registerAddress, SCORPIO_METER_INTERFACE_REGISTER_ADDRESS_HEADER_SIZE);
    frame_ptr += SCORPIO_METER_INTERFACE_REGISTER_ADDRESS_HEADER_SIZE;            
    
    inverted_memcpy(frame_ptr, (BYTE *) &registerAmount, SCORPIO_METER_INTERFACE_REGISTER_AMOUNT_HEADER_SIZE);
    frame_ptr += SCORPIO_METER_INTERFACE_REGISTER_AMOUNT_HEADER_SIZE;
    
    switch(commandFunction){
        
        case SCORPIO_METER_INTERFACE_READ_COMMAND_FUNCTION:
            break;
            
        case SCORPIO_METER_INTERFACE_WRITE_COMMAND_FUNCTION:  
            
            // Adding data buffer into frame buffer
            if(dataLen){

                memcpy(frame_ptr, data, dataLen);
                frame_ptr += dataLen;
            }            
            break;
            
        default:
            return FALSE;        
    }
    
    // Calculing CRC Value
    crcFrame = wfnCRC_CALC((BYTE *) frame, (frame_ptr - frame), SCORPIO_METER_INTERFACE_CRC_BASE);
    
    // Adding CRC into frame buffer
    inverted_memcpy(frame_ptr, (BYTE *) &crcFrame, SCORPIO_METER_INTERFACE_CRC_HEADER_SIZE);
    frame_ptr += SCORPIO_METER_INTERFACE_CRC_HEADER_SIZE;
    
    * frameLen = (BYTE) (frame_ptr - frame);
    
    return TRUE;
}

void ScorpioMeterInterface_SendFrame(BYTE * serialNumber,
                                     WORD serialNumberLen,
                                     WORD commandFunction,
                                     WORD registerAddress, 
                                     WORD registerAmount, 
                                     BYTE * data, 
                                     BYTE dataLen )
{
    BYTE frame[SCORPIO_METER_INTERFACE_MAX_FRAME_BUFFER_SIZE];
    BYTE frameLen;
    BOOL framebuilt;
    
    framebuilt = ScorpioMeterInterface_BuildFrame(  serialNumber,       /* serialNumber     */
                                                    serialNumberLen,    /* serialNumberLen  */
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

void ScorpioMeterInterface_SendPassword(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen){
    
    BYTE buffer [SCORPIO_METER_INTERFACE_LOCAL_BUFFER_SIZE];
    BYTE * buffer_ptr = buffer;
    BYTE bufferLen;    
    WORD passwordType;
    WORD passwordValue = SCORPIO_METER_INTERFACE_PASSWORD;
    BYTE commandId;
    
    memcpy(&commandId, data,  dataLen);
    
    switch(commandId){
        
        case Dis_MTR:
            print_info("Sending PASSWORD for DISCONNECTION Command");
            passwordType = SCORPIO_METER_INTERFACE_PASSWORD_REPLAY;
            break;
            
        case Con_MTR:           
            print_info("Sending PASSWORD for CONNECTION Command");
            passwordType = SCORPIO_METER_INTERFACE_PASSWORD_REPLAY;
            break;
            
        case Res_MTR:
            
            print_info("Sending PASSWORD for RESET Command");
            passwordType = SCORPIO_METER_INTERFACE_PASSWORD_FORCE_RESET;
            break;
            
        default:
            return;
    }
    
    
    inverted_memcpy(buffer_ptr, (BYTE *) &passwordValue , sizeof(passwordValue));
    buffer_ptr += sizeof(passwordValue);
    
    inverted_memcpy(buffer_ptr, (BYTE *) &passwordType , sizeof(passwordType) );
    buffer_ptr += sizeof(passwordType);
    
    bufferLen = buffer_ptr - buffer;
    
    ScorpioMeterInterface_SendFrame(serialNumber, 
                                    serialNumberLen, 
                                    SCORPIO_METER_INTERFACE_WRITE_COMMAND_FUNCTION,
                                    SCORPIO_METER_INTERFACE_PASSWORD_REGISTER_ADDRESS,
                                    SCORPIO_METER_INTERFACE_PASSWORD_REGISTER_ADDRESS_SIZE,
                                    (BYTE *) buffer,
                                    bufferLen);
}

void ScorpioMeterInterface_Disconnect(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen)
{
    BYTE buffer [SCORPIO_METER_INTERFACE_LOCAL_BUFFER_SIZE];
    BYTE * buffer_ptr = buffer;
    BYTE bufferLen;
    
    DWORD relayStatus = SCORPIO_METER_INTERFACE_SYSTEM_FLAGS_RELAY_TURN_OFF;
        
    inverted_memcpy(buffer_ptr, (BYTE *) &relayStatus , sizeof(relayStatus));
    buffer_ptr += sizeof(relayStatus);
            
    bufferLen = buffer_ptr - buffer;
    
    print_info("Sending DISCONNECT Command");
    
    ScorpioMeterInterface_SendFrame(serialNumber, 
                                    serialNumberLen, 
                                    SCORPIO_METER_INTERFACE_WRITE_COMMAND_FUNCTION,
                                    SCORPIO_METER_INTERFACE_SYSTEM_FLAGS_REGISTER_ADDRESS,
                                    SCORPIO_METER_INTERFACE_SYSTEM_FLAGS_REGISTER_ADDRESS_SIZE,
                                    (BYTE *) buffer,
                                    bufferLen);  
}

void ScorpioMeterInterface_Connect(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen){
 
    BYTE buffer [SCORPIO_METER_INTERFACE_LOCAL_BUFFER_SIZE];
    BYTE * buffer_ptr = buffer;
    BYTE bufferLen;
    
    DWORD relayStatus = SCORPIO_METER_INTERFACE_SYSTEM_FLAGS_RELAY_TURN_ON;
        
    inverted_memcpy(buffer_ptr, (BYTE *) &relayStatus , sizeof(relayStatus));
    buffer_ptr += sizeof(relayStatus);
            
    bufferLen = buffer_ptr - buffer;
    
    print_info("Sending CONNECT Command");
    
    ScorpioMeterInterface_SendFrame(serialNumber, 
                                    serialNumberLen, 
                                    SCORPIO_METER_INTERFACE_WRITE_COMMAND_FUNCTION,
                                    SCORPIO_METER_INTERFACE_SYSTEM_FLAGS_REGISTER_ADDRESS,
                                    SCORPIO_METER_INTERFACE_SYSTEM_FLAGS_REGISTER_ADDRESS_SIZE,
                                    (BYTE *) buffer,
                                    bufferLen);  
}

void ScorpioMeterInterface_Reset(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen){
    
    BYTE buffer [SCORPIO_METER_INTERFACE_LOCAL_BUFFER_SIZE];
    BYTE * buffer_ptr = buffer;
    BYTE bufferLen;
    
    DWORD status = SCORPIO_METER_INTERFACE_SYSTEM_FLAGS_FORCE_RESET;
        
    inverted_memcpy(buffer_ptr, (BYTE *) &status , sizeof(status));
    buffer_ptr += sizeof(status);
            
    bufferLen = buffer_ptr - buffer;
    
    print_info("Sending Reset Command");
    
    ScorpioMeterInterface_SendFrame(serialNumber, 
                                    serialNumberLen, 
                                    SCORPIO_METER_INTERFACE_WRITE_COMMAND_FUNCTION,
                                    SCORPIO_METER_INTERFACE_SYSTEM_FLAGS_REGISTER_ADDRESS,
                                    SCORPIO_METER_INTERFACE_SYSTEM_FLAGS_REGISTER_ADDRESS_SIZE,
                                    (BYTE *) buffer,
                                    bufferLen); 
}

void ScorpioMeterInterface_ReadMeteringData(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen){
    
    print_info("Requesting METERING Command");
    
    ScorpioMeterInterface_SendFrame(serialNumber, 
                                    serialNumberLen, 
                                    SCORPIO_METER_INTERFACE_READ_COMMAND_FUNCTION,
                                    SCORPIO_METER_INTERFACE_METERING_REGISTER_ADDRESS,
                                    SCORPIO_METER_INTERFACE_METERING_REGISTER_ADDRESS_SIZE,
                                    NULL,
                                    0);  
}

void ScorpioMeterInterface_LinkMeter(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen){

    print_info("LINKING METER Command");
    print_info("Associated SHORT MAC ADDRESS : ");
    ComSerialInterface_PrintData(data, dataLen);
    
    ScorpioMeterInterface_SendFrame(serialNumber, 
                                    serialNumberLen, 
                                    SCORPIO_METER_INTERFACE_WRITE_COMMAND_FUNCTION,
                                    SCORPIO_METER_INTERFACE_LINKER_ADDDEL_METER_NUMBER_REGISTER_ADDRESS,
                                    SCORPIO_METER_INTERFACE_LINKER_ADDDEL_METER_NUMBER_REGISTER_ADDRESS_SIZE,
                                    data,
                                    dataLen);      
}