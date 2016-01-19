#include <string.h>
#include "handlerG155.h"
#include "G155MeterInterface.h"

G155_STRUCT g155_control;

WORD wfnCRC_CALC_G155(BYTE *ptFRAME, WORD wSizeFrame, WORD wCRCStart){
    WORD temp;
    BYTE c, flag;
    for (wSizeFrame;wSizeFrame>0;wSizeFrame--){
        temp = (WORD) *ptFRAME;
        temp &= 0x00FF;
        wCRCStart = wCRCStart ^ temp;
        for (c=0;c<8;c++){
            flag = wCRCStart & 0x01;
            wCRCStart = wCRCStart>>1;
            if(flag != 0) wCRCStart = wCRCStart ^ 0xA001; //, 0xA001, 0xC002, 0x8005
        }
        ptFRAME++;
    }
    //wCRCStart = (wCRCStart >> 8) | (wCRCStart << 8);
    return (wCRCStart);
}

BYTE Handler_FlagCheck_G155(BYTE * ptbFrame, WORD wSizeFrame, BYTE * flag_state){
  
    BYTE * ptBUFFER = ptbFrame + HANDLER_G155_FLAG_BUFFER_OFFSET;        
    
    if( *ptBUFFER == HANDLER_G155_FLAG_DATA_VALUE){
            
        ptBUFFER = ptbFrame + HANDLER_G155_FLAG_ADDR_BUFFER_OFFSET;
                
        if ( *ptBUFFER == HANDLER_G155_FLAG_ADDRESS_VALUE ){
        
            ptBUFFER = ptbFrame + HANDLER_G155_FLAG_STATE_BUFFER_OFFSET;
            
            if ( *ptBUFFER == HANDLER_G155_FLAG_RELAY_ON) {
                
               * flag_state = HANDLER_G155_FLAG_RELAY_ON;
               
               return HANDLER_G155_FLAG_DATA_VALUE; //TRUE
               
            }
            
            if ( *ptBUFFER == HANDLER_G155_FLAG_RELAY_OFF ){
                
                * flag_state = HANDLER_G155_FLAG_RELAY_OFF;
                
                return HANDLER_G155_FLAG_DATA_VALUE; //TRUE
                
            } 
            if ( *ptBUFFER == HANDLER_G155_ACCUMULATED_RESET ){
                
                * flag_state = HANDLER_G155_ACCUMULATED_RESET;
                
                return HANDLER_G155_FLAG_DATA_VALUE;//TRUE
                
            }
            if ( *ptBUFFER == HANDLER_G155_KVARHR_PULSE ){
                
                * flag_state = HANDLER_G155_KVARHR_PULSE;
                
                return HANDLER_G155_FLAG_DATA_VALUE;//TRUE
                
            }
            if ( *ptBUFFER == HANDLER_G155_RESET ){
                
                * flag_state = HANDLER_G155_RESET;
                return HANDLER_G155_FLAG_DATA_VALUE;//TRUE
            } 
        }
        
        if ( *ptBUFFER == HANDLER_G155_CALIBRATION_ADDRESS_VALUE ){
            
            ptBUFFER = ptbFrame + HANDLER_G155_CALIBRATION_ADDR_HIG_VALUE_OFFSET;
            
            if ( *ptBUFFER == HANDLER_G155_CALIBRATION_ADDR_HIGH_VALUE ){
                
                ptBUFFER = ptbFrame + HANDLER_G155_CALIBRATION_ADDR_LOW_VALUE_OFFSET;
                
                if ( *ptBUFFER == HANDLER_G155_CALIBRATION_ADDR_LOW_VALUE ){
                    
                    * flag_state = HANDLER_G155_CALIBRATION_ADDR_LOW_VALUE;
                    
                    return HANDLER_G155_FLAG_DATA_VALUE;//TRUE
                }
            }
        }
        if ( *ptBUFFER == HANDLER_G155_PASSWORD_ADDRESS_VALUE ){
            
            ptBUFFER = ptbFrame + HANDLER_G155_PASSWORD_ADDR_HIGH_VALUE_OFFSET;
            
            if ( *ptBUFFER == HANDLER_G155_PASSWORD_ADDR_HIGH_VALUE ){
                
                ptBUFFER = ptbFrame + HANDLER_G155_PASSWORD_ADDR_LOW_VALUE_OFFSET;
                
                if ( *ptBUFFER == HANDLER_G155_PASSWORD_ADDR_RELAY_VALUE ){
                    
                    * flag_state = HANDLER_G155_PASSWORD_ADDR_RELAY_VALUE;
                    
                    return HANDLER_G155_FLAG_DATA_VALUE;//TRUE
                }
                
                 if ( *ptBUFFER == HANDLER_G155_PASSWORD_ADDR_ACCUMULATED_RESET_VALUE ){
                    
                    * flag_state = HANDLER_G155_PASSWORD_ADDR_ACCUMULATED_RESET_VALUE;
                    
                    return HANDLER_G155_FLAG_DATA_VALUE; //TRUE
                }
                
                if ( *ptBUFFER == HANDLER_G155_PASSWORD_ADDR_PULSE_VALUE ){
                    
                    * flag_state = HANDLER_G155_PASSWORD_ADDR_PULSE_VALUE;
                    
                    return HANDLER_G155_FLAG_DATA_VALUE;//TRUE
                }
                
                if ( *ptBUFFER == HANDLER_G155_PASSWORD_ADDR_RESET_VALUE ){
                    
                    * flag_state = HANDLER_G155_PASSWORD_ADDR_RESET_VALUE;
                    
                    return HANDLER_G155_FLAG_DATA_VALUE;//TRUE
                }
                
            }
            
        }
        if ( *ptBUFFER == HANDLER_G155_MODBUSID_ADDRESS_VALUE){
            
            return HANDLER_G155_FLAG_DATA_VALUE;//TRUE
        }
    }
    
    return FALSE;  
}

BYTE Handler_FcnCheck_G155(BYTE* function_data){

    
    BYTE *ptBUFFER = function_data + HANDLER_G155_FCN_INDEX_OFFSET;
    
    if (*ptBUFFER == HANDLER_G155_FUNCTION_DATA_VALUE ){
        
        return HANDLER_G155_FUNCTION_DATA_VALUE;//TRUE      
    }
    if (*ptBUFFER == HANDLER_G155_FUNCTION_MULTIPLE_WRITE_DATA_VALUE){
        
        ptBUFFER = function_data + HANDLER_G155_FCN_REGISTER1_INDEX_OFFSET;
        
        if (*ptBUFFER == HANDLER_G155_FUNCTION_WRITE_CFE_VALUE || *ptBUFFER == HANDLER_G155_FUNCTION_WRITE_MAC_VALUE){
            
            ptBUFFER = function_data + HANDLER_G155_FCN_REGISTER2_INDEX_OFFSET;
            
            if (*ptBUFFER == HANDLER_G155_FUNCTION_WRITE_TIMESTAMP_VALUE || *ptBUFFER == HANDLER_G155_FUNCTION_WRITE_NEWREGISTER_VALUE){
                
                return HANDLER_G155_FUNCTION_MULTIPLE_WRITE_DATA_VALUE;//TRUE
            }
            
        }
        
    }
    return FALSE;   
}

BYTE Handler_SizeCheck_G155(BYTE* size_data_value, WORD wSizeFrame, BYTE* data){
    
    BYTE size;
    BYTE size_data;
    
    BYTE *ptBUFFER = size_data_value + HANDLER_G155_FCN_INDEX_OFFSET;  //locate to fcn index
    
    if (*ptBUFFER == HANDLER_G155_FLAG_DATA_VALUE ){
        
        size = wSizeFrame;
        size_data = size - HANDLER_G155_FLAG_ARRAY_DATA_SIZE ; // size data with flag
        
        if (size_data == 0 ){
            ptBUFFER++;
            memcpy(data,ptBUFFER, HANDLER_G155_FLAG_DATA_SIZE);
            return HANDLER_G155_FLAG_DATA_SIZE;//TRUE
        }
        return HANDLER_G155_FLAG_NOT_DATA_SIZE;
    }
    
    if (*ptBUFFER == HANDLER_G155_FUNCTION_DATA_VALUE ){
        
        ptBUFFER = size_data_value + HANDLER_G155_SIZE_INDEX_OFFSET; //locate to size index
        
        size = wSizeFrame;
        
        size_data = size - HANDLER_G155_FUNCTION_ARRAY_DATA_SIZE; // remove id,fcn,size & crc bytes(only when fcn = 3)
        
        if (size_data == *ptBUFFER ){
            ptBUFFER++;
            memcpy(data, ptBUFFER, size_data);
            return size_data; //TRUE
        }
        return HANDLER_G155_FCN_NOT_DATA_SIZE ;
        
    }
    
    if (*ptBUFFER == HANDLER_G155_FUNCTION_MULTIPLE_WRITE_DATA_VALUE ){
        
        ptBUFFER = size_data_value + HANDLER_G155_FCN_SIZE_INDEX_OFSET ; //locate to size index
        
        size = wSizeFrame;
        
        size_data = size - HANDLER_G155_FUNCTION_WRITE_ARRAY_DATA_SIZE; // remove id(1),fcn(1),register1(2),register2(2),size(1) & crc (2) bytes (onlye when fcn = 10)
        
        if (size_data == *ptBUFFER ){
           
            ptBUFFER = size_data_value + HANDLER_G155_FCN_MAC_INDEX_OFSET ;  //locate to mac index
            memcpy(data, ptBUFFER, size_data);
            return size_data; //TRUE  
        }
        return HANDLER_G155_FCN_NOT_DATA_SIZE ;
        
    }
    
    return HANDLER_G155_NOT_DATA_SIZE;
}

BYTE Handler_IdCheck_G155(BYTE* id_data){

    BYTE temp;
    
    
    BYTE *ptBUFFER = id_data + HANDLER_G155_FCN_INDEX_OFFSET;  //locate to fcn index
    
    if (*ptBUFFER == HANDLER_G155_FLAG_DATA_VALUE || *ptBUFFER == HANDLER_G155_FUNCTION_DATA_VALUE ){
      
        ptBUFFER = id_data;
    
        if (*ptBUFFER > HANDLER_G155_LIMIT_MIN_ID || *ptBUFFER < HANDLER_G155_LIMIT_MAX_ID){ ////min-max ID
        
        
        temp = (BYTE)*ptBUFFER;
        return temp; //TRUE 
        
        }
        
        return FALSE;       
    }
    
    if (*ptBUFFER == HANDLER_G155_FUNCTION_MULTIPLE_WRITE_DATA_VALUE ){
        
        ptBUFFER = id_data;
        
        if (*ptBUFFER == HANDLER_G155_LIMIT_MIN_ID || *ptBUFFER < HANDLER_G155_LIMIT_MAX_ID){ 
        
        temp = (BYTE)*ptBUFFER;
        return temp; //TRUE 
        
        }  
        
    }
   
    return FALSE;
      
}

WORD API_G155_Recieve_handler( BYTE * buffer, WORD  buffersize, METER_DESCRIPTOR_PTR meterDescriptor, BYTE * commandCallBack){   
    
    WORD crc;    
    BYTE  fcn;
    BYTE  flag;
    BYTE  size;
    BYTE  id;
    BYTE state;
    BYTE data[G155_MAX_DATA_SIZE] = {0};
    
    //Execute Recieve handler functions 
    
    crc  = wfnCRC_CALC_G155((BYTE *) buffer, buffersize, 0xFFFF);   
    size = Handler_SizeCheck_G155( (BYTE*) buffer, buffersize, data);
    id   = Handler_IdCheck_G155((BYTE*) buffer);
    flag = Handler_FlagCheck_G155( (BYTE*) buffer, buffersize,(BYTE*) &state);
    fcn  = Handler_FcnCheck_G155( (BYTE*) buffer);
    
    //Pre-Validation (Recieve Data)
    
    if(crc != HANDLER_G155_CRC_OK)
        return HANDLER_G155_CRC_FAILURE_ERROR_CODE;
    if(size <= HANDLER_G155_SIZE_OK)
        return HANDLER_G155_SIZE_FAILURE_ERROR_CODE;
    if(id < HANDLER_G155_ID_OK)
        return HANDLER_G155_ID_FAILURE_ERROR_CODE;
    if(  (flag == HANDLER_G155_NOT_A_FLAG && fcn == HANDLER_G155_NOT_A_FCN ) 
          || (flag == HANDLER_G155_IS_A_FLAG && (fcn  == HANDLER_G155_IS_A_FCN || fcn  == HANDLER_G155_IS_A_MULTIPLE_FCN) ) 
       )
        return HANDLER_G155_FLAG_AND_FUNCTION_FAILURE_ERROR_CODE;
    
    //Fill Struct if pre-validation success
    
    g155_control.dataSize = size;
    g155_control.modbusId = id;
    g155_control.flag = flag;
    g155_control.fcn = fcn;
    memcpy(g155_control.data,data,size);
    
    MeterDescriptor_Setup(meterDescriptor, g155_control.modbusId, NULL, 0, G155_TYPE);    
    * commandCallBack = HandlerG155_GetInvokeFunctionId(g155_control.fcn);
    
    return HANDLER_G155_NO_ERROR;
 }     


BYTE HandlerG155_GetInvokeFunctionId(BYTE command){
    
    switch(command){

        /*
         * case HANDLER_G155_METER_COMMAND:
         *      return METER_COMMON_COMMAND_FOR_SPECIFIC_PURPORSE;         
         */
            
        default:
            return NO_COMMAND_MTR;
    }
    
}

WORD API_G155_Meter_response_handler( BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE command, BYTE * response, WORD maxResponseLen, WORD * responseLen){

    /* Validating modbus id*/
    
     if (g155_control.modbusId != modbusId)

         return ERROR_NO_MATCH_MODBUS_ID;

    /*Validating numeration*/ 
     
     switch(command){

        case NO_COMMAND_MTR: // No command Meter
            
                 return NO_ERROR_NUMERATION;
         
        case Dis_MTR: // Disconnect Meter

             if ((g155_control.flag == HANDLER_G155_IS_A_FLAG) && (g155_control.dataSize == HANDLER_G155_FLAG_DATA_SIZE)){

                 memcpy(response, g155_control.data, g155_control.dataSize);//copy data & data size
                 *responseLen = g155_control.dataSize;

                 return NO_ERROR_NUMERATION;
             }
             break;
             
        case Con_MTR: // Connect Meter

             if ((g155_control.flag == HANDLER_G155_IS_A_FLAG) && (g155_control.dataSize == HANDLER_G155_FLAG_DATA_SIZE)){

                 memcpy(response, g155_control.data, g155_control.dataSize);//copy data & data size
                 *responseLen = g155_control.dataSize;

                 return NO_ERROR_NUMERATION;
             }
             break;
             
        case Res_MTR: // Reset Meter

             if ((g155_control.flag == HANDLER_G155_IS_A_FLAG) && (g155_control.dataSize == HANDLER_G155_FLAG_DATA_SIZE)){

                 memcpy(response, g155_control.data, g155_control.dataSize);//copy data & data size
                 *responseLen = g155_control.dataSize;

                 return NO_ERROR_NUMERATION;
             }
             break;     

        case READ_MODE: // Read Meter

             if ((g155_control.fcn == HANDLER_G155_IS_A_FCN) && (g155_control.dataSize == HANDLER_G155_FCN_MTR_READ_DATA_SIZE)){
                 
                 if(maxResponseLen >= sizeof(Data_Readings)){
                     
                    G155MeterHandler_ParseToDataReading((Data_Readings_Ptr) response, g155_control.data, g155_control.dataSize);
                    //memcpy(response, g155_control.data, g155_control.dataSize);//copy data & data size
                    *responseLen = sizeof(Data_Readings); //g155_control.dataSize;

                    return NO_ERROR_NUMERATION;
                 }
             }
             break;     
        
        case PSW_MTR: // Password Meter

             if ((g155_control.flag == HANDLER_G155_IS_A_FLAG) && (g155_control.dataSize == HANDLER_G155_FLAG_DATA_SIZE)){

                 memcpy(response, g155_control.data, g155_control.dataSize);//copy data & data size
                 *responseLen = g155_control.dataSize;

                 return NO_ERROR_NUMERATION;
             }
             break;
             
        case SEND_MAC_BROADCAST_MTR: // Send MAC Broadcast Meter

             if ((g155_control.fcn == HANDLER_G155_IS_A_MULTIPLE_FCN) && (g155_control.dataSize == HANDLER_G155_FCN_MAC_DATA_SIZE)){

                 memcpy(response, g155_control.data, g155_control.dataSize);//copy data & data size
                 *responseLen = g155_control.dataSize;

                 return NO_ERROR_NUMERATION;
             }
             break;
             
        case ASSIGN_MODBUS_ID_MTR: // Assign Modbus ID Meter

             if ((g155_control.flag == HANDLER_G155_IS_A_FLAG) && (g155_control.dataSize == HANDLER_G155_FLAG_DATA_SIZE)){

                 memcpy(response, g155_control.data, g155_control.dataSize);//copy data & data size
                 *responseLen = g155_control.dataSize;

                 return NO_ERROR_NUMERATION;
             }
             break;
             
        case REQUEST_SERIAL_NUMBER_MTR: // Request Serial Number Meter

             if ((g155_control.fcn == HANDLER_G155_IS_A_FCN) && (g155_control.dataSize == HANDLER_G155_FCN_MTR_SN_DATA_SIZE)){
                 
                 BYTE serial_number[Lenght_Meter_ID];
                 memset(serial_number, '0', sizeof(serial_number));
                 memcpy(serial_number + HANDLER_G155_SERIAL_NUMBER_OFFSET, g155_control.data, g155_control.dataSize);
                 memcpy(response, serial_number , sizeof(serial_number) );//copy data & data size
                 *responseLen = sizeof(serial_number);

                 return NO_ERROR_NUMERATION;
             }
             break;     
             
        default:
            
             return ERROR_NUMERATION;
     }


     return ERROR_NUMERATION;

}

void G155MeterHandler_ParseToDataReading( Data_Readings_Ptr dataReading, BYTE * data,  WORD dataLen){
    
    Data_Readings_G155 g155DataReading;
    
    if(dataLen != sizeof(g155DataReading))
        return;
    
    inverted_memcpy((BYTE*) &g155DataReading, data, dataLen);    
    memset(dataReading , 0xFF, sizeof(Data_Readings) );
    
    dataReading->ENERGY_ACT_A_Add       =   G155MeterHandler_DWORD_Parser(g155DataReading.ENERGY_ACT_Add);
    dataReading->VOLTAGE_A_Add          =   G155MeterHandler_WORD_Parser(g155DataReading.VOLTAGE_Add);
    dataReading->CURRENT_A_Add          =   G155MeterHandler_WORD_Parser(g155DataReading.CURRENT_Add);
    dataReading->ENERGY_REACT_TOTAL_Add =   G155MeterHandler_DWORD_Parser(g155DataReading.ENERGY_REACT_TOTAL_Add);
    dataReading->POWER_ACT_SYSTEM_Add   =   g155DataReading.POWER_ACT_SYSTEM_Add;
    dataReading->FLAGS_Add_LWEND        =   g155DataReading.FLAGS;   
    
    G155MeterHandler_PrintDataReading(dataReading);
}   

void G155MeterHandler_PrintDataReading(Data_Readings_Ptr dataReading){
    
    printf("[DATA READING INFORMATION]\n");
    printf("Active Energy (A): %i\n", dataReading->ENERGY_ACT_A_Add);
    printf("Voltage (A): %i\n", dataReading->VOLTAGE_A_Add);
    printf("Current (A): %i\n", dataReading->CURRENT_A_Add);
    printf("Reactive Energy (A): %i\n", dataReading->ENERGY_REACT_TOTAL_Add);
    printf("Active Power : %i\n", dataReading->POWER_ACT_SYSTEM_Add);
    printf("FLAGS : %i\n", dataReading->FLAGS_Add_LWEND);
}

#define HANDLER_G155_DWORD_INTEGER_ROTATE_VALUE                              (14)
#define HANDLER_G155_DWORD_RATION_VALUE                                      (1000)
#define HANDLER_G155_DWORD_DECIMAL_CONSTANT_VALUE                            (61)
#define HANDLER_G155_DWORD_DECIMAL_MASK_VALUE                                (0x00003FFF)

#define HANDLER_G155_WORD_RATION_VALUE                                       (100)
#define HANDLER_G155_WORD_DECIMAL_CONSTANT_VALUE                             (39)

DWORD G155MeterHandler_DWORD_Parser(DWORD value){
    
    DWORD newValue;
    
    newValue =  ((value >> HANDLER_G155_DWORD_INTEGER_ROTATE_VALUE) * HANDLER_G155_DWORD_RATION_VALUE) + 
                    ((( value & HANDLER_G155_DWORD_DECIMAL_MASK_VALUE) * HANDLER_G155_DWORD_DECIMAL_CONSTANT_VALUE) / HANDLER_G155_DWORD_RATION_VALUE);
    
    return newValue;
}

WORD G155MeterHandler_WORD_Parser(WORD value){
    
    WORD newValue;
    
    newValue =  (( value  * HANDLER_G155_WORD_DECIMAL_CONSTANT_VALUE ) / HANDLER_G155_WORD_RATION_VALUE );
    
    return newValue;
}