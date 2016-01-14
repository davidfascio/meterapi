#include "ScorpioMeterInterface.h"

COMMAND_ID_FUNCTION ScorpioMeterInterface_CommandIdFunctionList [] = 
{
    {   PSW_MTR,                                            /* Meter Common Command Id              */
        ScorpioMeterInterface_SendPassword  },              /* Meter CommandIdFunction_Callback     */
    
    {   Dis_MTR,                                            /* Meter Common Command Id              */
        ScorpioMeterInterface_Disconnect  },                /* Meter CommandIdFunction_Callback     */
        
    {   Con_MTR,                                            /* Meter Common Command Id              */
        ScorpioMeterInterface_Connect  },                   /* Meter CommandIdFunction_Callback     */
        
    {   READ_MODE ,                                         /* Meter Common Command Id              */
        ScorpioMeterInterface_ReadMeteringData } ,             /* Meter CommandIdFunction_Callback     */
        
    COMMAND_ID_FUNCTION_NULL
};

BOOL ScorpioMeterInterface_BuildFrame(  BYTE * serialNumber,
                                        WORD serialNumberLen,
                                        WORD commandFunction,
                                        BYTE registerAddress, 
                                        BYTE registerAmount, 
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
    
    inverted_memcpy(frame_ptr, &registerAddress, SCORPIO_METER_INTERFACE_REGISTER_ADDRESS_HEADER_SIZE);
    frame_ptr += SCORPIO_METER_INTERFACE_REGISTER_ADDRESS_HEADER_SIZE;            
    
    inverted_memcpy(frame_ptr, &registerAmount, SCORPIO_METER_INTERFACE_REGISTER_AMOUNT_HEADER_SIZE);
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
                                     BYTE registerAddress, 
                                     BYTE registerAmount, 
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
            
    ComSerialInterface_SendData(frame, frameLen);        
}

void ScorpioMeterInterface_SendPassword(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen){
    
    BYTE buffer [SCORPIO_METER_INTERFACE_LOCAL_BUFFER_SIZE];
    BYTE * buffer_ptr = buffer;
    BYTE bufferLen;
    
    WORD passwordValue = SCORPIO_METER_INTERFACE_PASSWORD;
    WORD passwordType = SCORPIO_METER_INTERFACE_PASSWORD_REPLAY;
    
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
    
    ScorpioMeterInterface_SendFrame(serialNumber, 
                                    serialNumberLen, 
                                    SCORPIO_METER_INTERFACE_WRITE_COMMAND_FUNCTION,
                                    SCORPIO_METER_INTERFACE_SYSTEM_FLAGS_REGISTER_ADDRESS,
                                    SCORPIO_METER_INTERFACE_SYSTEM_FLAGS_REGISTER_ADDRESS_SIZE,
                                    (BYTE *) buffer,
                                    bufferLen);  
}


void ScorpioMeterInterface_ReadMeteringData(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen){
         
    ScorpioMeterInterface_SendFrame(serialNumber, 
                                    serialNumberLen, 
                                    SCORPIO_METER_INTERFACE_READ_COMMAND_FUNCTION,
                                    SCORPIO_METER_INTERFACE_METERING_REGISTER_ADDRESS,
                                    SCORPIO_METER_INTERFACE_METERING_REGISTER_ADDRESS_SIZE,
                                    NULL,
                                    0);  
}

WORD API_Scorpio_Meter_response_handler( BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE command, BYTE * response, WORD * responseLen){
    
    return 0;
}

WORD API_Scorpio_Recieve_handler( BYTE * buffer, WORD  buffersize, METER_DESCRIPTOR_PTR meterDescriptor, BYTE * commandCallBack){
    
    *commandCallBack = 0;
    
    return 0;
}