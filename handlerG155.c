#include <string.h>
#include "handlerG155.h"

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

WORD API_G155_Recieve_handler(BYTE* buffer, WORD buffersize){   
    
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
    return HANDLER_G155_NO_ERROR;
 } 


WORD API_G155_Meter_response_handler( BYTE modbusId, BYTE numeration, BYTE * response, WORD * responseLen){

    /* Validating modbus id*/
    
     if (g155_control.modbusId != modbusId)

         return ERROR_NO_MATCH_MODBUS_ID;

    /*Validating numeration*/ 
     
     switch(numeration){

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

                 memcpy(response, g155_control.data, g155_control.dataSize);//copy data & data size
                 *responseLen = g155_control.dataSize;

                 return NO_ERROR_NUMERATION;
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

                 memcpy(response, g155_control.data, g155_control.dataSize);//copy data & data size
                 *responseLen = g155_control.dataSize;

                 return NO_ERROR_NUMERATION;
             }
             break;     
             
        default:
            
             return ERROR_NUMERATION;
     }


     return ERROR_NUMERATION;

}

BYTE hadler_API_modbus(BYTE * buffer, WORD bufferSize){
    BYTE error_code;

    error_code = API_G155_Recieve_handler(buffer, bufferSize);

    if (error_code == HANDLER_G155_NO_ERROR){

        //Metercontrol_SetDataAvailable(TRUE);
    }
}
