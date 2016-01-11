#include "ComSerialInterface.h"
#include "EventsEngine.h"


HANDLE ComSerialInterface_OpenHandler(  LPCSTR  portName, 
                                        DWORD   baudRate,
                                        DWORD   byteSize,
                                        DWORD   stopBits,
                                        BYTE    parity  )
{
    LPCSTR portname = portName;
    DWORD accessdirection = GENERIC_READ | GENERIC_WRITE;
    DCB dcbSerialParams= {0};
    
    COMMTIMEOUTS timeouts = {0};
    
    /*
     * WINBASEAPI HANDLE WINAPI CreateFileA (   LPCSTR lpFileName, 
     *                                          DWORD dwDesiredAccess, 
     *                                          DWORD dwShareMode, 
     *                                          LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
     *                                          DWORD dwCreationDisposition, 
     *                                          DWORD dwFlagsAndAttributes, 
     *                                          HANDLE hTemplateFile);
     */
    
    HANDLE hSerial = CreateFile(   portname,
                                    accessdirection,
                                    0,
                                    0,
                                    OPEN_EXISTING,
                                    0,
                                    0   );
    
    if(hSerial == INVALID_HANDLE_VALUE){
        
        return NULL; //-1;
    }
    
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    
    if(!GetCommState(hSerial, &dcbSerialParams)){
        
        return NULL; //-2;
    }
    
    dcbSerialParams.BaudRate = baudRate;    // 19200;
    dcbSerialParams.ByteSize = byteSize;    // 8;
    dcbSerialParams.StopBits = stopBits;    // ONESTOPBIT;
    dcbSerialParams.Parity   = parity;      // EVENPARITY;
    
    if(!SetCommState(hSerial, &dcbSerialParams)){
        
        return NULL;    // -3;
    }
    
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    
    if(!SetCommTimeouts(hSerial, &timeouts)){
        
        return NULL; // -4;
        
    }

    return hSerial;
}

void ComSerialInterface_CloseHandler( HANDLE hSerial){
    
    CloseHandle(hSerial);
}

DWORD ComSerialInterface_ReadPort( HANDLE hSerial, BYTE * buffer, DWORD buffersize){
    
    DWORD dwBytesRead = 0;
    
    if(!ReadFile(hSerial, buffer, buffersize, &dwBytesRead, NULL)){
        
        return 0;
    }
    
    return dwBytesRead;
}

DWORD ComSerialInterface_WritePort( HANDLE hSerial, BYTE * buffer, DWORD buffersize){
    
    DWORD dwBytesRead = 0;
    
    if(!WriteFile(hSerial, buffer, buffersize, &dwBytesRead, NULL)){
        
        return 0;
    }
    
    return dwBytesRead;
}

#define COM_SERIAL_INTERFACE_HANDLER_PORT                       "\\\\.\\COM30"
#define COM_SERIAL_INTERFACE_HANDLER_BAUDRATE                   (19200)
#define COM_SERIAL_INTERFACE_HANDLER_BYTE_SIZE                  (8)
#define COM_SERIAL_INTERFACE_HANDLER_STOP_BITS                  (ONESTOPBIT)
#define COM_SERIAL_INTERFACE_HANDLER_PARITY                     (EVENPARITY)


HANDLE ComSerialInterfaceHandler = NULL;


void ComSerialInterface_PrintData(BYTE * frame, WORD frameLen){
    
    BYTE index;
    BYTE * frame_ptr = frame;
    
    
    for(index = 0; index < frameLen; index++)
        printf("%02X ", *frame_ptr++);
    
    printf("\n");
}
DWORD ComSerialInterface_WriteData(BYTE * data, DWORD dataLen){
    
    if(ComSerialInterfaceHandler == NULL){
        
        printf("Serial Port is closed\n");
        return 0;
    }
    
    return ComSerialInterface_WritePort(ComSerialInterfaceHandler, data, dataLen);
}

DWORD ComSerialInterface_ReadData(BYTE * data, DWORD dataLen){
    
    if(ComSerialInterfaceHandler == NULL){
    
        //printf("Serial Port is closed\n");
        return 0;
    }
    
    return ComSerialInterface_ReadPort(ComSerialInterfaceHandler, data, dataLen);
}


#define COM_SERIAL_INTERFACE_BUFFER_SIZE                                    (100)
#define COM_SERIAL_INTERFACE_BUFFER_WINDOW_SIZE                              (1)

BYTE ComSerialInterfaceBuffer[COM_SERIAL_INTERFACE_BUFFER_SIZE];
BYTE ComSerialInterfaceBufferUsed = 0;

void ComSerialInterface_Init(void){
    
    ComSerialInterfaceHandler = ComSerialInterface_OpenHandler( COM_SERIAL_INTERFACE_HANDLER_PORT,
                                                                COM_SERIAL_INTERFACE_HANDLER_BAUDRATE,
                                                                COM_SERIAL_INTERFACE_HANDLER_BYTE_SIZE,
                                                                COM_SERIAL_INTERFACE_HANDLER_STOP_BITS,
                                                                COM_SERIAL_INTERFACE_HANDLER_PARITY);
    
    ComSerialInterface_CleanBuffer();
    //memset(ComSerialInterfaceBuffer, 0 , sizeof(ComSerialInterfaceBuffer));
}

void ComSerialInterface_CleanBuffer(void){
    
    ComSerialInterfaceBufferUsed = 0;
    memset(ComSerialInterfaceBuffer, 0 , sizeof(ComSerialInterfaceBuffer));
}

void ComSerialInterface_Check(void){
    
    BYTE * ComSerialInterfaceBuffer_ptr = ComSerialInterfaceBuffer + ComSerialInterfaceBufferUsed;
    BYTE arrivedBytes;
    
    arrivedBytes = ComSerialInterface_ReadData(ComSerialInterfaceBuffer_ptr, COM_SERIAL_INTERFACE_BUFFER_WINDOW_SIZE);
        
    if(arrivedBytes){
        
        ComSerialInterfaceBufferUsed += arrivedBytes;
        vfnOneShotReload(WAIT_FRAME_UART_ONESHOT, _25_MSEC_);
    }
}

BYTE * ComSerialInterface_GetBuffer( void){
    
    return ComSerialInterfaceBuffer;
    
}

WORD ComSerialInterface_GetBufferSize(void){
    
    return ComSerialInterfaceBufferUsed;
}