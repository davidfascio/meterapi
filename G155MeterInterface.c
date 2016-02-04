#include "G155MeterInterface.h"


COMMAND_ID_FUNCTION G155MeterInterface_CommandIdFunctionList [] = 
{
    {   PSW_MTR ,                                   /* Meter Common Command Id              */
        G155MeterInterface_SendPassword },          /* Meter CommandIdFunction_Callback     */
    
    {   Dis_MTR ,                                   /* Meter Common Command Id              */
        G155MeterInterface_Disconnect },            /* Meter CommandIdFunction_Callback     */
    
    {   Con_MTR ,                                   /* Meter Common Command Id              */
        G155MeterInterface_Connect },               /* Meter CommandIdFunction_Callback     */
        
    {   Res_MTR ,                                   /* Meter Common Command Id              */
        G155MeterInterface_Reset },                 /* Meter CommandIdFunction_Callback     */
    
    {   READ_MODE ,                                 /* Meter Common Command Id              */
        G155MeterInterface_ReadMeteringData } ,     /* Meter CommandIdFunction_Callback     */
        
    {   SEND_MAC_BROADCAST_MTR ,                    /* Meter Common Command Id              */
        G155MeterInterface_SendMACAddress },        /* Meter CommandIdFunction_Callback     */
        
    {   ASSIGN_MODBUS_ID_MTR ,                      /* Meter Common Command Id              */
        G155MeterInterface_SendAssignModbusId },    /* Meter CommandIdFunction_Callback     */
        
    {   REQUEST_SERIAL_NUMBER_MTR ,                 /* Meter Common Command Id              */
        G155MeterInterface_RequestSerialNumber },   /* Meter CommandIdFunction_Callback     */    
        
    COMMAND_ID_FUNCTION_NULL        
};

BYTE G155MeterInterface_Handler(    BYTE modbusId, 
                                    BYTE * serialNumber, 
                                    WORD serialNumberLen, 
                                    BYTE command, 
                                    BYTE * response, 
                                    WORD * responseLen)
{
    memset(response , '1', 16);
    * responseLen  = 16;
    return 0;    
}

BOOL G155MeterInterface_BuildFrame( BYTE modbusId,
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
    
    memcpy(frame_ptr, &modbusId, G155_METER_INTERFACE_MODBUS_ID_HEADER_SIZE);
    frame_ptr += G155_METER_INTERFACE_MODBUS_ID_HEADER_SIZE;
    
    memcpy(frame_ptr, (BYTE *) &commandFunction, G155_METER_INTERFACE_MODBUS_COMMAND_FUNCTION_HEADER_SIZE);
    frame_ptr += G155_METER_INTERFACE_MODBUS_COMMAND_FUNCTION_HEADER_SIZE;
    
    memcpy(frame_ptr, &registerAddress, G155_METER_INTERFACE_MODBUS_REGISTER_ADDRESS_HEADER_SIZE);
    frame_ptr += G155_METER_INTERFACE_MODBUS_REGISTER_ADDRESS_HEADER_SIZE;            
    
    switch(commandFunction){
        
        case WRITE_SINGLE_FN_MDB:            
            break;
            
        case WRITE_VARIOUS_FN_MDB:
        case READ_FN_MDB:
            
            memset(frame_ptr, 0, G155_METER_INTERFACE_MODBUS_EMPTY_LOCATION_HEADER_SIZE);
            frame_ptr += G155_METER_INTERFACE_MODBUS_EMPTY_LOCATION_HEADER_SIZE;
            
            memcpy(frame_ptr, (BYTE *) &registerAmount, G155_METER_INTERFACE_MODBUS_REGISTER_QTY_HEADER_SIZE);
            frame_ptr += G155_METER_INTERFACE_MODBUS_REGISTER_QTY_HEADER_SIZE;
            
            if(dataLen){
                
                memcpy(frame_ptr, (BYTE *) &dataLen, G155_METER_INTERFACE_MODBUS_DATA_LEN_HEADER_SIZE);
                frame_ptr += G155_METER_INTERFACE_MODBUS_DATA_LEN_HEADER_SIZE;
            }
            
            break;
            
        default:
            return FALSE;        
    }
    
    // Adding data buffer into frame buffer
    if(dataLen){
        
        memcpy(frame_ptr, data, dataLen);
        frame_ptr += dataLen;
    }
    
    // Calculing CRC Value
    crcFrame = wfnCRC_CALC((BYTE *) frame, (frame_ptr - frame), G155_METER_INTERFACE_MODBUS_CRC_BASE);
    
    // Adding CRC into frame buffer
    memcpy(frame_ptr, (BYTE *) &crcFrame, G155_METER_INTERFACE_MODBUS_CRC_HEADER_SIZE);
    frame_ptr += G155_METER_INTERFACE_MODBUS_CRC_HEADER_SIZE;
    
    * frameLen = (BYTE) (frame_ptr - frame);
    
    return TRUE;
}

void G155MeterInterface_SendFrame(  BYTE modbusId,
                                    WORD commandFunction,
                                    BYTE registerAddress, 
                                    BYTE registerAmount, 
                                    BYTE * data, 
                                    BYTE dataLen )
{    
    BYTE frame[G155_METER_INTERFACE_MAX_FRAME_BUFFER_SIZE];
    BYTE frameLen;
    BOOL framebuilt;
    
    framebuilt = G155MeterInterface_BuildFrame( modbusId,           /* modbus Id        */
                                                commandFunction,    /* commandFunction  */     
                                                registerAddress,    /* registerAddress, */
                                                registerAmount,     /* registerAmount   */
                                                data,               /* data             */
                                                dataLen,            /* dataLen          */
                                                frame,              /* frame            */
                                                &frameLen);         /* frameLen         */
    
    if(framebuilt == FALSE)
        return;
    
    //print_info("Sent Data: ");
    //ComSerialInterface_PrintData(frame, frameLen);
    ComSerialInterface_SendData(frame, frameLen);
}



void G155MeterInterface_SendPassword( BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen)
{
    BYTE buffer [G155_METER_INTERFACE_LOCAL_BUFFER_SIZE];
    BYTE * buffer_ptr = buffer;
    BYTE bufferLen;      
    
    BYTE passwordType;
    BYTE passwordValue = KEY1_G155;
    BYTE commandId;
    
    memcpy(&commandId, data,  dataLen);
    
    switch(commandId){
        
        case Dis_MTR:
            print_info("Sending PASSWORD for DISCONNECTION Command");
            passwordType = KEY2_G155;
            break;
            
        case Con_MTR:           
            print_info("Sending PASSWORD for CONNECTION Command");
            passwordType = KEY2_G155;
            break;
            
        case Res_MTR:
            
            print_info("Sending PASSWORD for RESET Command");
            passwordType = RST_G155_CMD;
            break;
            
        default:
            return;
    }
    
    memcpy(buffer_ptr, (BYTE *) &passwordValue , sizeof(passwordValue));
    buffer_ptr += sizeof(passwordValue);
    
    inverted_memcpy(buffer_ptr, (BYTE *) &passwordType , sizeof(passwordType) );
    buffer_ptr += sizeof(passwordType);
    
    bufferLen = buffer_ptr - buffer;
    
    G155MeterInterface_SendFrame(   modbusId,               /* modbus Id        */
                                    WRITE_SINGLE_FN_MDB,    /* commandFunction  */     
                                    ADD_KEY_G155,           /* registerAddress, */
                                    0,                      /* registerAmount   */
                                    buffer,                 /* data             */
                                    bufferLen);             /* dataLen          */
}

void G155MeterInterface_Disconnect(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen)
{
    BYTE buffer [] = {0x00, RLY_G155_OFF};
    BYTE bufferLen = sizeof(buffer);
    
    print_info("Sending DISCONNECT Command");
    
    G155MeterInterface_SendFrame(   modbusId,               /* modbus Id        */
                                    WRITE_SINGLE_FN_MDB,    /* commandFunction  */     
                                    ADD_RLY_G155,           /* registerAddress, */
                                    0,                      /* registerAmount   */
                                    buffer,                 /* data             */
                                    bufferLen);             /* dataLen          */   
}
void G155MeterInterface_Connect(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen){
 
    BYTE buffer [] = {0x00, RLY_G155_ON};
    BYTE bufferLen = sizeof(buffer);
    
    print_info("Sending CONNECT Command");
    
    G155MeterInterface_SendFrame(   modbusId,               /* modbus Id        */
                                    WRITE_SINGLE_FN_MDB,    /* commandFunction  */     
                                    ADD_RLY_G155,           /* registerAddress, */
                                    0,                      /* registerAmount   */
                                    buffer,                 /* data             */
                                    bufferLen);             /* dataLen          */
}

void G155MeterInterface_Reset(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen){
        
    BYTE buffer [] = {0x00, RST_G155_CMD};
    BYTE bufferLen = sizeof(buffer);
    
    print_info("Sending RESET Command");
    
    G155MeterInterface_SendFrame(   modbusId,               /* modbus Id        */
                                    WRITE_SINGLE_FN_MDB,    /* commandFunction  */     
                                    ADD_RLY_G155,           /* registerAddress, */
                                    0,                      /* registerAmount   */
                                    buffer,                 /* data             */
                                    bufferLen);             /* dataLen          */
}

void G155MeterInterface_ReadMeteringData(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen){

    print_info("Sending READ_DATA Command");    
    
    G155MeterInterface_SendFrame(   modbusId,                                       /* modbus Id        */
                                    READ_FN_MDB,                                    /* commandFunction  */     
                                    ENERGY_G155,                                    /* registerAddress, */
                                    G155_METER_INTERFACE_ENERGY_REGISTER_QTY_SIZE,  /* registerAmount   */
                                    NULL,                                           /* data             */
                                    0);                                             /* dataLen          */
}

void G155MeterInterface_SendMACAddress(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen){
    
    print_info("Send MAC Broadcast");
    
    G155MeterInterface_SendFrame(   modbusId,                                               /* modbus Id        */
                                    WRITE_VARIOUS_FN_MDB,                                   /* commandFunction  */     
                                    ZIGBEE_ADD,                                             /* registerAddress, */
                                    G155_METER_INTERFACE_MAC_ADDRESS_REGISTER_QTY_SIZE,     /* registerAmount   */
                                    data,                                                   /* data             */
                                    dataLen);                                               /* dataLen          */
}
void G155MeterInterface_SendAssignModbusId(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen){
    
    BYTE buffer [] = {0x00, * data};
    BYTE bufferLen = sizeof(buffer);
    
    print_info("Send Assign Modbus Id");
    
    G155MeterInterface_SendFrame(   modbusId,               /* modbus Id        */
                                    WRITE_SINGLE_FN_MDB,    /* commandFunction  */     
                                    ID_ADD,                 /* registerAddress, */
                                    0,                      /* registerAmount   */
                                    buffer,                 /* data             */
                                    bufferLen);             /* dataLen          */
}
void G155MeterInterface_RequestSerialNumber(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen){
    
    print_info("Request Serial Number");
    
    G155MeterInterface_SendFrame(   modbusId,                                   /* modbus Id        */
                                    READ_FN_MDB,                                /* commandFunction  */     
                                    CFE_NUMBER_ADD,                             /* registerAddress, */
                                    G155_METER_INTERFACE_SERIAL_NUMBER_REGISTER_QTY_SIZE,   /* registerAmount   */
                                    NULL,                                       /* data             */
                                    0);                                         /* dataLen          */
}