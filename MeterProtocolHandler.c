#include "MeterProtocolHandler.h"

void MeterProtocolHandler(void){
    
    BYTE * buffer_ptr;
    WORD buffersize;
    
    buffer_ptr = ComSerialInterface_GetBuffer();    
    buffersize = ComSerialInterface_GetBufferSize();
    MeterProtocolHandler_PrintData(buffer_ptr, buffersize);
}

void MeterProtocolHandler_PrintData(BYTE* frame, BYTE frameLen){
    
    BYTE index;
    BYTE * frame_ptr = frame;
    
    printf("Arrived Data: ");
    
    for(index = 0; index < frameLen; index++)
        printf("%02X ", *frame_ptr++);
    
    printf("\n");
}