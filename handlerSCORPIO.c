#include <string.h>
#include "handlerSCORPIO.h"

SCORPIO_STRUCT scorpio_control;

boolean wfnCRC_CALC_SCORPIO(BYTE *ptFRAME, WORD wSizeFrame, WORD wCRCStart){
    
    WORD temp;
    BYTE c, flag;
    BYTE *ptBUFFER = ptFRAME;
    BYTE frameBuffer[BufferSize] = {0};
    BYTE size;
    BYTE lowByteCRC;
    BYTE highByteCRC;
    WORD AddBytesCRC;
    int count = 0;
    int index;
    
    size = wSizeFrame-HANDLER_SCORPIO_CRC; //remove 2 bytes from CRC
    
    for (index = 0; index < size; index++){
        
        ptBUFFER = ptFRAME + index;
        
        frameBuffer[index] = *ptBUFFER; //fill buffer without CRC
        count = count + 1;
        
    }
   
    index =0; // restart index
 
    //Make CRC....
    
    for (wSizeFrame;count>0;count--){
        temp = (WORD) frameBuffer[index]; // *ptFRAME
        temp &= 0x00FF;
        wCRCStart = wCRCStart ^ temp;
        for (c=0;c<8;c++){
            flag = wCRCStart & 0x01;
            wCRCStart = wCRCStart>>1;
            if(flag != 0) wCRCStart = wCRCStart ^ 0xA001; 
        }
        index++;
    }
 
    //Locate CRC in the frame...
    
    size = wSizeFrame;
    highByteCRC = ptFRAME[size - hByteCRC]; //locate to hig address from CRC in the frame...
    lowByteCRC  = ptFRAME[size - lByteCRC]; //locate to low address from CRC in the frame...
    
    //Add CRC...
    
    AddBytesCRC = highByteCRC;
    AddBytesCRC = AddBytesCRC << 8; //first byte make high value from CRC
    AddBytesCRC += lowByteCRC;      // add low value from CRC
    

    //Check CRC ...
    
    if(AddBytesCRC == wCRCStart || AddBytesCRC == REPLY_DATA_CRC ) {
        
        return FALSE;
    }   
    
    return TRUE;
    
}

BYTE Handler_Flag_Addr_PassWord_Check_SCORPIO(BYTE * ptbFrame, WORD wSizeFrame, BYTE * flag_state){
  
    BYTE * ptBUFFER = ptbFrame + HANDLER_SCORPIO_FLAG_BUFFER_OFFSET;        
    
    if( *ptBUFFER == HANDLER_SCORPIO_FLAG_DATA_VALUE){ //0X10
            
        ptBUFFER = ptbFrame + HANDLER_SCORPIO_FLAG_ADDR_PASSWORD_HIGH_OFFSET;
        //================================FLAG_ADDR_PASSWORD=====================================        
        if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_ADDR_PASSWORD_HIGH_VALUE ){ //0x17
        
            ptBUFFER = ptbFrame + HANDLER_SCORPIO_FLAG_ADDR_PASSWORD_LOW_OFFSET;
            
            if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_ADDR_PASSWORD_LOW_VALUE) { //0x02
                
                ptBUFFER = ptbFrame + HANDLER_SCORPIO_FLAG_ADDR_HIGH_1_VALUE_OFFSET;
                
                
                if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_ADDR_HIGH_1_PTR){ //C3
                    
                    
                    ptBUFFER = ptbFrame + HANDLER_SCORPIO_FLAG_ADDR_HIGH_2_VALUE_OFFSET;
                    
                    if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_ADDR_HIGH_2_00_PTR){ //00
                        
                        ptBUFFER = ptbFrame + HANDLER_SCORPIO_FLAG_ADDR_LOW_1_VALUE_OFFSET;
                        
                        
                        if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_ADDR_LOW_1_00_PTR){ //00
                            
                            ptBUFFER = ptbFrame + HANDLER_SCORPIO_FLAG_ADDR_LOW_2_VALUE_OFFSET;
                            
                            if (*ptBUFFER == HANDLER_SCORPIO_FLAG_ADDR_LOW_2_RELAY_PTR || *ptBUFFER == HANDLER_SCORPIO_FLAG_ADDR_LOW_2_IRMODE_PTR 
                               || *ptBUFFER == HANDLER_SCORPIO_FLAG_ADDR_LOW_2_3PHMODE_PTR || *ptBUFFER == HANDLER_SCORPIO_FLAG_ADDR_LOW_2_HMMODE_PTR 
                               || *ptBUFFER == HANDLER_SCORPIO_FLAG_ADDR_LOW_2_VOLTAGEMODE_PTR || *ptBUFFER == HANDLER_SCORPIO_FLAG_ADDR_LOW_2_ACCRESET_PTR 
                               || *ptBUFFER == HANDLER_SCORPIO_FLAG_ADDR_LOW_2_FORCERESET_PTR || *ptBUFFER == HANDLER_SCORPIO_FLAG_ADDR_LOW_2_STPMCRESET_PTR ){
                                
                                return HANDLER_SCORPIO_FLAG_DATA_VALUE;//TRUE
                            }
                            
                            return HANDLER_SCORPIO_NO_FLAG_DATA_VALUE;//FALSE
                        }
                        
                        if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_ADDR_LOW_1_01_PTR){//01
                            
                            ptBUFFER = ptbFrame + HANDLER_SCORPIO_FLAG_ADDR_LOW_2_VALUE_OFFSET;
                            
                            if (*ptBUFFER == HANDLER_SCORPIO_FLAG_ADDR_LOW_2_DEMANDRESET_PTR){
                                
                                return HANDLER_SCORPIO_FLAG_DATA_VALUE;//TRUE
                            }
                            
                            return HANDLER_SCORPIO_NO_FLAG_DATA_VALUE;//FALSE
                            
                        }
                        
                        if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_ADDR_LOW_1_02_PTR){//02
                            
                            ptBUFFER = ptbFrame + HANDLER_SCORPIO_FLAG_ADDR_LOW_2_VALUE_OFFSET;
                            
                            if (*ptBUFFER == HANDLER_SCORPIO_FLAG_ADDR_LOW_2_UPGRADEPROCESS_PTR){
                                
                                return HANDLER_SCORPIO_FLAG_DATA_VALUE;//TRUE
                            }
                            
                            return HANDLER_SCORPIO_NO_FLAG_DATA_VALUE;//FALSE
                        }
                        
                        if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_ADDR_LOW_1_40_PTR){//40
                            
                            ptBUFFER = ptbFrame + HANDLER_SCORPIO_FLAG_ADDR_LOW_2_VALUE_OFFSET;
                            
                            if (*ptBUFFER == HANDLER_SCORPIO_FLAG_ADDR_LOW_2_BOOTLOADERREGULAR_PTR){
                                
                                return HANDLER_SCORPIO_FLAG_DATA_VALUE;//TRUE
                            }
                            
                            return HANDLER_SCORPIO_NO_FLAG_DATA_VALUE;//FALSE
                        }
                        
                        return HANDLER_SCORPIO_NO_FLAG_DATA_VALUE;//FALSE
                        
                    }
                    
                    if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_ADDR_HIGH_2_ROLLING_DEMAND_PTR){ //01
                        
                        ptBUFFER = ptbFrame + HANDLER_SCORPIO_FLAG_ADDR_LOW_1_VALUE_OFFSET;
                        
                        if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_ADDR_LOW_1_ROLLING_DEMAND_PTR){ //00
                            
                            ptBUFFER = ptbFrame + HANDLER_SCORPIO_FLAG_ADDR_LOW_2_VALUE_OFFSET;
                            
                            if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_ADDR_LOW_2_ROLLING_DEMAND_PTR){ //00
                                
                                return HANDLER_SCORPIO_FLAG_DATA_VALUE;//TRUE
                            }
                            
                        }
                    }
                    
                    return HANDLER_SCORPIO_NO_FLAG_DATA_VALUE;//FALSE
                    
                }
                
                if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_ADDR_HIGH_1_BOOTLOADER_PTR){ //74
                    
                    ptBUFFER = ptbFrame + HANDLER_SCORPIO_FLAG_ADDR_HIGH_2_VALUE_OFFSET;
                    
                    if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_ADDR_HIGH_2_BOOTLOADER_PTR){ //CA
                        
                        ptBUFFER = ptbFrame + HANDLER_SCORPIO_FLAG_ADDR_LOW_1_VALUE_OFFSET;
                        
                        if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_ADDR_LOW_1_BOOTLOADER_PTR){ //F0
                            
                            ptBUFFER = ptbFrame + HANDLER_SCORPIO_FLAG_ADDR_LOW_2_VALUE_OFFSET;
                            
                            if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_ADDR_LOW_2_BOOTLOADER_PTR){ //2D
                                
                                return HANDLER_SCORPIO_FLAG_DATA_VALUE;//TRUE
                            }
                            
                        }
                        
                    }
                }
                
                return HANDLER_SCORPIO_NO_FLAG_DATA_VALUE;//FALSE
            }
            
            return HANDLER_SCORPIO_NO_FLAG_DATA_VALUE;//FALSE
        }
        
    }
    
    return HANDLER_SCORPIO_NO_FLAG_DATA_VALUE;//FALSE  
}


BYTE Handler_Flag_Addr_System_Check_SCORPIO(BYTE * ptbFrame, WORD wSizeFrame, BYTE * flag_state){
      
    BYTE * ptBUFFER = ptbFrame + HANDLER_SCORPIO_FLAG_BUFFER_OFFSET;        
    
    if( *ptBUFFER == HANDLER_SCORPIO_FLAG_DATA_VALUE){ //0X10
            
        ptBUFFER = ptbFrame + HANDLER_SCORPIO_FLAG_ADDR_PASSWORD_HIGH_OFFSET;
        
        //=================================FLAG_ADDR_SYSTEM=========================================
        if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_ADDR_SYTEM_HIGH_VALUE ){ //0x23
            
            ptBUFFER = ptbFrame + HANDLER_SCORPIO_FLAG_ADDR_SYSTEM_OFFSET;
            
            if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_ADDR_SYTEM_LOW_VALUE){ //0X02
               
                ptBUFFER = ptbFrame + HANDLER_SCORPIO_FLAG_DATA_VALUE_HIGH_1_OFFSET ;
                
                
                if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_DATA_VALUE_00_HIGH_1){ //0x00
                    
                    
                    ptBUFFER = ptbFrame + HANDLER_SCORPIO_FLAG_DATA_VALUE_HIGH_2_OFFSET ;
                    
                    
                    if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_DATA_VALUE_00_HIGH_2){ //0x00
                        
                        ptBUFFER = ptbFrame + HANDLER_SCORPIO_FLAG_DATA_VALUE_LOW_1_OFFSET;
                        
                        
                        if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_DATA_VALUE_00_LOW_1){//0x00
                            
                            ptBUFFER = ptbFrame + HANDLER_SCORPIO_FLAG_DATA_VALUE_LOW_2_OFFSET;
                            
                            if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_RELAY_ON || *ptBUFFER == HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_RELAY_OFF
                                || *ptBUFFER == HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_IRMODE_REA || *ptBUFFER == HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_IRMODE_ACT 
                                || *ptBUFFER == HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_3PHMODE_DELTA || *ptBUFFER == HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_HMMODE_HM
                                || *ptBUFFER == HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_HMMODE_OM || *ptBUFFER == HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_VOLTAGEMODE_120
                                || *ptBUFFER == HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_VOLTAGEMODE_240 || *ptBUFFER == HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_ACC_RESET 
                                || *ptBUFFER == HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_FORCE_RESET || *ptBUFFER == HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_STPMC_RESET  ){
                                
                                return HANDLER_SCORPIO_FLAG_DATA_VALUE;//TRUE
                            }
                            
                            return HANDLER_SCORPIO_NO_FLAG_DATA_VALUE;//FALSE
                        }
                        
                        if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_DATA_VALUE_C0_LOW_1){//0xC0
                            
                            ptBUFFER = ptbFrame + HANDLER_SCORPIO_FLAG_DATA_VALUE_LOW_2_OFFSET;
                            
                            if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_UARTPARITY_EVEN){
                                
                                return HANDLER_SCORPIO_FLAG_DATA_VALUE;//TRUE
                                
                            }
                            
                            return HANDLER_SCORPIO_NO_FLAG_DATA_VALUE;//FALSE
                            
                        }
                        
                        if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_DATA_VALUE_40_LOW_1){//0x40
                            
                            ptBUFFER = ptbFrame + HANDLER_SCORPIO_FLAG_DATA_VALUE_LOW_2_OFFSET;
                            
                            if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_BOOTLOADER_REGULAR){
                                
                                return HANDLER_SCORPIO_FLAG_DATA_VALUE;//TRUE
                                
                            }
                            
                            return HANDLER_SCORPIO_NO_FLAG_DATA_VALUE;//FALSE
                            
                        }
                        if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_DATA_VALUE_01_LOW_1){//0x01
                            
                            ptBUFFER = ptbFrame + HANDLER_SCORPIO_FLAG_DATA_VALUE_LOW_2_OFFSET;
                            
                            if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_DEMAND_RESET){
                                
                                return HANDLER_SCORPIO_FLAG_DATA_VALUE;//TRUE
                                
                            }
                            
                            return HANDLER_SCORPIO_NO_FLAG_DATA_VALUE;//FALSE
                            
                        }
                        
                        if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_DATA_VALUE_02_LOW_1){//0x02
                            
                            ptBUFFER = ptbFrame + HANDLER_SCORPIO_FLAG_DATA_VALUE_LOW_2_OFFSET;
                            
                            if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_UPGRADE_PROCESS){
                                
                                return HANDLER_SCORPIO_FLAG_DATA_VALUE;//TRUE
                                
                            }
                            
                            return HANDLER_SCORPIO_NO_FLAG_DATA_VALUE;//FALSE
                            
                        }
                        
                        return HANDLER_SCORPIO_NO_FLAG_DATA_VALUE;//FALSE
                    }
                    
                    if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_DATA_VALUE_01_HIGH_2){ //0x01
                        
                        ptBUFFER = ptbFrame + HANDLER_SCORPIO_FLAG_DATA_VALUE_LOW_1_OFFSET;
                        
                        if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_DATA_VALUE_00_LOW_1){ //0x00
                            
                            ptBUFFER = ptbFrame + HANDLER_SCORPIO_FLAG_DATA_VALUE_LOW_2_OFFSET;
                            
                            if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_DEMAND_INTERVAL_3M 
                                || ptBUFFER == HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_DEMAND_INTERVAL_15M){
                                
                                return HANDLER_SCORPIO_FLAG_DATA_VALUE;//TRUE
                            }
                        }
                        
                        
                    }
                    
                    return HANDLER_SCORPIO_NO_FLAG_DATA_VALUE;//FALSE
                    
                }
                
                return HANDLER_SCORPIO_NO_FLAG_DATA_VALUE;//FALSE
            }
            
            return HANDLER_SCORPIO_NO_FLAG_DATA_VALUE;//FALSE
        }
        
        if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_ADDR_SN_CFE_HIGH_PTR ){ //0x04
            
            ptBUFFER = ptbFrame + HANDLER_SCORPIO_FLAG_ADDR_PASSWORD_LOW_OFFSET;
            
            if ( *ptBUFFER == HANDLER_SCORPIO_FLAG_ADDR_SN_CFE_LOW_PTR){ //0x08
                
                return HANDLER_SCORPIO_FLAG_DATA_VALUE;//TRUE
            }
        }
        
        return HANDLER_SCORPIO_NO_FLAG_DATA_VALUE;//FALSE
    }
    
    return HANDLER_SCORPIO_NO_FLAG_DATA_VALUE;//FALSE
}


BYTE Handler_FcnCheck_SCORPIO(BYTE* function_data){

    
    BYTE *ptBUFFER = function_data + HANDLER_SCORPIO_FCN_INDEX_HIGH_OFFSET;
    
    if (*ptBUFFER == HANDLER_SCORPIO_FUNCTION_ID_HIGH_VALUE ){
        
        ptBUFFER = function_data + HANDLER_SCORPIO_FCN_INDEX_LOW_OFFSET;
        
        if (*ptBUFFER == HANDLER_SCORPIO_FUNCTION_ID_LOW_VALUE ){
            
            return HANDLER_SCORPIO_FUNCTION_ID_LOW_VALUE;//TRUE
          
        }
    }
    if (*ptBUFFER == HANDLER_SCORPIO_ESPECIAL_FUNCTION_TYPEMETER_MONOPH  
        || *ptBUFFER == HANDLER_SCORPIO_ESPECIAL_FUNCTION_TYPEMETER_BIPH 
        || *ptBUFFER == HANDLER_SCORPIO_ESPECIAL_FUNCTION_TYPEMETER_TRIPH
        || *ptBUFFER == HANDLER_SCORPIO_ESPECIAL_FUNCTION_DELETE_METER){
                
        return HANDLER_SCORPIO_FUNCTION_PUSHBUTTON_ID_VALUE; //FUNCION ESPECIAL DEBIDA AL PUSH-BUTTON
    }
    
    ptBUFFER = function_data + HANDLER_SCORPIO_ESPFCN_TYPE_REPLYDATA_HIGH_OFFSET;
    
    if (*ptBUFFER == HANDLER_SCORPIO_ESPFCN_TYPE_REPLYDATA_HIGH_VALUE ){
        
        ptBUFFER = function_data + HANDLER_SCORPIO_ESPFCN_TYPE_REPLYDATA_LOW_OFFSET;
        
        if (*ptBUFFER == HANDLER_SCORPIO_ESPFCN_TYPE_REPLYDATA_LOW_VALUE ){
            
        
            return HANDLER_SCORPIO_FUNCTION_PUSHBUTTON_ID_VALUE; //FUNCION ESPECIAL DEBIDA AL PUSH-BUTTON
            
        }
        
         return HANDLER_SCORPIO_NO_FCN_DATA_VALUE;
        
    }
    
    
    return HANDLER_SCORPIO_NO_FCN_DATA_VALUE;
   
}


BYTE Handler_Size_Data_Check_SCORPIO(BYTE* size_data_value, WORD wSizeFrame, BYTE* data){
    
    BYTE size;
    BYTE size_data;
    BYTE affected_registers;
    BYTE quantity_affected_registers;
    
    BYTE *ptBUFFER = size_data_value + HANDLER_SCORPIO_FCN_INDEX_HIGH_OFFSET;  //locate to fcn high index
    
    if (*ptBUFFER == HANDLER_SCORPIO_FUNCTION_ID_HIGH_VALUE ){
        
        ptBUFFER = size_data_value + HANDLER_SCORPIO_FCN_INDEX_LOW_OFFSET; //locate to fcn low index
        
        if (*ptBUFFER == HANDLER_SCORPIO_FUNCTION_ID_LOW_VALUE ){
            
            ptBUFFER = size_data_value + HANDLER_SCORPIO_FCN_SIZE_INDEX_OFFSET; //locate to fcn size index
            
            size = wSizeFrame;
        
            size_data = size - HANDLER_SCORPIO_FUNCTION_DATA_SIZE; // remove NS CFE (16 bytes), ID FCN (2 bytes), SIZE (1 byte), CRC (2 bytes) = 21 bytes
        
            if (size_data == *ptBUFFER ){
           
            ptBUFFER++;
            memcpy(data, ptBUFFER, size_data);
            return size_data;//TRUE
            
            }
            return NO_CORRECT_SIZE;
        }
        
        if (*ptBUFFER == HANDLER_SCORPIO_FLAG_DATA_VALUE ){
            
            size = wSizeFrame;
            
            if (size == HANDLER_SCORPIO_FLAG_SIZE){
                
                ptBUFFER = size_data_value + HANDLER_SCORPIO_FlAG_DATA_INDEX_OFFSET;  //locate to flag data index offset
                memcpy(data,ptBUFFER, HANDLER_SCORPIO_FLAG_DATA_SIZE);
                return HANDLER_SCORPIO_FLAG_DATA_SIZE;//TRUE
            
            }
            if (size == HANDLER_SCORPIO_FLAG_WRITE_SIZE){
            
                ptBUFFER = size_data_value + HANDLER_SCORPIO_FlAG_WRITE_DATA_SIZE_INDEX_OFFSET; //locate to flag write data size of affected registers index
                affected_registers = *ptBUFFER;
                quantity_affected_registers = affected_registers * CONSTANT_AFFECTED_REGISTERS;
                ptBUFFER++; //locate to affected register index         
                memcpy(data,ptBUFFER,quantity_affected_registers);        
                return quantity_affected_registers; //TRUE   
                
            }
               return NO_CORRECT_SIZE; 
        }
        
        return NO_CORRECT_SIZE;
    }
    
    if (*ptBUFFER == HANDLER_SCORPIO_ESPECIAL_FUNCTION_TYPEMETER_MONOPH  
        || *ptBUFFER == HANDLER_SCORPIO_ESPECIAL_FUNCTION_TYPEMETER_BIPH 
        || *ptBUFFER == HANDLER_SCORPIO_ESPECIAL_FUNCTION_TYPEMETER_TRIPH
        || *ptBUFFER == HANDLER_SCORPIO_ESPECIAL_FUNCTION_DELETE_METER){
            
        size = wSizeFrame;
        
        if (size == HANDLER_SCORPIO_FCN_PUSHBUTTON_SIZE ){
            
            ptBUFFER = size_data_value + HANDLER_SCORPIO_FCN_INDEX_HIGH_OFFSET;  //locate to METER TYPE index offset
            memcpy(data,ptBUFFER, HANDLER_SCORPIO_FLAG_PUSHBUTTON_SPACE_SIZE);
            return HANDLER_SCORPIO_FLAG_PUSHBUTTON_SPACE_SIZE;//TRUE
            //return size;
            
        }
        return NO_CORRECT_SIZE;
    }
           
    ptBUFFER = size_data_value + HANDLER_SCORPIO_ESPFCN_TYPE_REPLYDATA_HIGH_OFFSET;
    
    if (*ptBUFFER == HANDLER_SCORPIO_ESPFCN_TYPE_REPLYDATA_HIGH_VALUE ){
        
        ptBUFFER = size_data_value + HANDLER_SCORPIO_ESPFCN_TYPE_REPLYDATA_LOW_OFFSET;
        
        if (*ptBUFFER == HANDLER_SCORPIO_ESPFCN_TYPE_REPLYDATA_LOW_VALUE ){
            
        
            size = wSizeFrame;
        
            if (size == HANDLER_SCORPIO_ESPFCN_REPLY_DATA_SIZE){
            
                ptBUFFER = size_data_value + HANDLER_SCORPIO_FCN_INDEX_HIGH_OFFSET;  //locate to reply data index offset
                memcpy(data,ptBUFFER, HANDLER_SCORPIO_FLAG_REPLY_DATA_SPACE_SIZE);
                return HANDLER_SCORPIO_FLAG_REPLY_DATA_SPACE_SIZE ;//TRUE
                //return size;
            
            }
             return NO_CORRECT_SIZE;
        }
    }
    return NO_CORRECT_SIZE;
}

BYTE Handler_Serial_Number_Size_Check_SCORPIO(BYTE* id_data, BYTE* data){

    BYTE *ptBUFFER = id_data;
    int offset = 0;
    int count = 0;
    
    
    while(*ptBUFFER == 0){
        
        ptBUFFER = id_data + offset;
        count++;                        //zero counts
        offset++;                       //move index frame
    }
    
    if (count < sizeSNSCORPIO ){
        
        ptBUFFER = id_data;  //locate to Serial Number index
        memcpy(data, ptBUFFER, sizeSNSCORPIO);
        return sizeSNSCORPIO; //TRUE      
    }
    
    else{
        
        return NO_SERIAL_NUMBER_SIZE;
    } 
      
}

WORD API_SCORPIO_Recieve_handler(BYTE* buffer, WORD buffersize, METER_DESCRIPTOR_PTR meterDescriptor, BYTE * commandCallBack){
    
    WORD  crc;    
    BYTE  fcn;
    BYTE  flag_password;
    BYTE  flag_system;
    BYTE  size;
    BYTE  sn_size;
    BYTE state;
    BYTE data[SCORPIO_MAX_DATA_SIZE] = {0};
    BYTE serialNumber[sizeSNSCORPIO];
    
    //Execute Recieve handler functions 
    
    crc             = wfnCRC_CALC_SCORPIO((BYTE *) buffer, buffersize, 0xFFFF);   
    size            = Handler_Size_Data_Check_SCORPIO( (BYTE*) buffer, buffersize, data);
    sn_size         = Handler_Serial_Number_Size_Check_SCORPIO((BYTE*) buffer, serialNumber);
    flag_password   = Handler_Flag_Addr_PassWord_Check_SCORPIO( (BYTE*) buffer, buffersize,(BYTE*) &state);
    flag_system     = Handler_Flag_Addr_System_Check_SCORPIO( (BYTE*) buffer, buffersize,(BYTE*) &state);
    fcn             = Handler_FcnCheck_SCORPIO( (BYTE*) buffer);
    
    //Pre-Validation (Recieve Data)
    
    if(crc != HANDLER_SCORPIO_CRC_OK)
        return HANDLER_SCORPIO_CRC_FAILURE_ERROR_CODE;
    if(size < HANDLER_SCORPIO_SIZE_OK)
        return HANDLER_SCORPIO_SIZE_FAILURE_ERROR_CODE;
    if((sn_size > HANDLER_SCORPIO_MAX_SN_SIZE) || (sn_size < HANDLER_SCORPIO_MIN_SN_SIZE) )
        return HANDLER_SCORPIO_ID_FAILURE_ERROR_CODE;
    if((flag_password == HANDLER_SCORPIO_IS_A_FLAG_PASSWORD && flag_system == HANDLER_SCORPIO_IS_A_FLAG_SYSTEM && (fcn == HANDLER_SCORPIO_IS_A_FCN || fcn == HANDLER_SCORPIO_IS_A_PUSHBUTTON_FCN))
       || (flag_password == HANDLER_SCORPIO_IS_A_FLAG_PASSWORD && flag_system == HANDLER_SCORPIO_IS_A_FLAG_SYSTEM && fcn  == HANDLER_SCORPIO_NOT_A_FCN)
       || (flag_password == HANDLER_SCORPIO_NOT_A_FLAG_PASSWORD && flag_system == HANDLER_SCORPIO_IS_A_FLAG_SYSTEM && (fcn == HANDLER_SCORPIO_IS_A_FCN || fcn == HANDLER_SCORPIO_IS_A_PUSHBUTTON_FCN))
       || (flag_password == HANDLER_SCORPIO_IS_A_FLAG_PASSWORD && flag_system == HANDLER_SCORPIO_NOT_A_FLAG_SYSTEM && (fcn == HANDLER_SCORPIO_IS_A_FCN || fcn == HANDLER_SCORPIO_IS_A_PUSHBUTTON_FCN)) 
       || (flag_password == HANDLER_SCORPIO_NOT_A_FLAG_PASSWORD && flag_system == HANDLER_SCORPIO_NOT_A_FLAG_SYSTEM && fcn  == HANDLER_SCORPIO_NOT_A_FCN))
        return HANDLER_SCORPIO_FLAG_AND_FUNCTION_FAILURE_ERROR_CODE;
    
    //Fill Struct if pre-validation success
    
    scorpio_control.dataSize = size;
    memcpy(scorpio_control.data,data,size);
    scorpio_control.serialNumberLen = sn_size;
    memcpy(scorpio_control.serialNumber,serialNumber,sn_size);
    scorpio_control.flag_password = flag_password;
    scorpio_control.flag_system = flag_system;
    scorpio_control.fcn = fcn;
    
    // Filling Meter Descriptor
    
    MeterDescriptor_Setup(meterDescriptor, 0, scorpio_control.serialNumber, scorpio_control.serialNumberLen, SCORPIO_METER_TYPE);    
    * commandCallBack = HandlerScorpio_GetInvokeFunctionId(scorpio_control.fcn);
    
    return HANDLER_SCORPIO_NO_ERROR;
 }

BYTE HandlerScorpio_GetInvokeFunctionId(BYTE command){
    
    switch(command){
        
        case HANDLER_SCORPIO_IS_A_PUSHBUTTON_FCN: 
             return LINK_ADDING_MTR;
        
        //! case HANDLER_SCORPIO_DELETING_FCN: //0x66
        //!     return LINK_DELETING_MTR;
            
        default:
            return NO_COMMAND_MTR;
    }
    
}
WORD API_SCORPIO_Meter_response_handler( BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE command, BYTE * response, WORD * responseLen){

    /* Validating modbus id*/
    
     if (scorpio_control.serialNumber != serialNumber)

         return ERROR_NO_MATCH_SERIAL_NUMBER;
     
     if (scorpio_control.serialNumberLen != serialNumberLen)

         return ERROR_NO_MATCH_SERIAL_NUMBER_LENGTH; 

    /*Validating numeration*/ 
     
     switch(command){

        case NO_COMMAND_MTR: // No command Meter
            
                 return NO_ERROR_NUMERATION;
         
        case Dis_MTR: // Disconnect Meter (flag)

             if ((scorpio_control.flag_password == HANDLER_SCORPIO_IS_A_FLAG_PASSWORD || scorpio_control.flag_system == HANDLER_SCORPIO_IS_A_FLAG_SYSTEM) && (scorpio_control.dataSize == HANDLER_SCORPIO_FLAG_DATA_SIZE)){

                 memcpy(response, scorpio_control.data, scorpio_control.dataSize);//copy data & data size
                 *responseLen = scorpio_control.dataSize;

                 return NO_ERROR_NUMERATION;
             }
             break;
             
        case Con_MTR: // Connect Meter (flag)

             if ((scorpio_control.flag_password == HANDLER_SCORPIO_IS_A_FLAG_PASSWORD || scorpio_control.flag_system == HANDLER_SCORPIO_IS_A_FLAG_SYSTEM) && (scorpio_control.dataSize == HANDLER_SCORPIO_FLAG_DATA_SIZE)){

                 memcpy(response, scorpio_control.data, scorpio_control.dataSize);//copy data & data size
                 *responseLen = scorpio_control.dataSize;

                 return NO_ERROR_NUMERATION;
             }
             break;
             
        case Res_MTR: // Reset Meter (flag)

             if ((scorpio_control.flag_password == HANDLER_SCORPIO_IS_A_FLAG_PASSWORD || scorpio_control.flag_system == HANDLER_SCORPIO_IS_A_FLAG_SYSTEM) && (scorpio_control.dataSize == HANDLER_SCORPIO_FLAG_DATA_SIZE)){

                 memcpy(response, scorpio_control.data, scorpio_control.dataSize);//copy data & data size
                 *responseLen = scorpio_control.dataSize;

                 return NO_ERROR_NUMERATION;
             }
             break;     

        case READ_MODE: // Read Meter (function)

             if ((scorpio_control.fcn == HANDLER_SCORPIO_FUNCTION_ID_LOW_VALUE) && (scorpio_control.dataSize == HANDLER_SCORPIO_FCN_MTR_READ_DATA_SIZE)){

                 memcpy(response, scorpio_control.data, scorpio_control.dataSize);//copy data & data size
                 *responseLen = scorpio_control.dataSize;

                 return NO_ERROR_NUMERATION;
             }
             break;     
        
        case PSW_MTR: // Password Meter (flag)

             if ((scorpio_control.flag_password == HANDLER_SCORPIO_IS_A_FLAG_PASSWORD || scorpio_control.flag_system == HANDLER_SCORPIO_IS_A_FLAG_SYSTEM) && (scorpio_control.dataSize == HANDLER_SCORPIO_FLAG_DATA_SIZE)){

                 memcpy(response, scorpio_control.data, scorpio_control.dataSize);//copy data & data size
                 *responseLen = scorpio_control.dataSize;

                 return NO_ERROR_NUMERATION;
             }
             break;
             
        case SEND_MAC_BROADCAST_MTR: // Send MAC Broadcast Meter

             /*if ((g155_control.fcn == HANDLER_G155_IS_A_MULTIPLE_FCN) && (g155_control.dataSize == HANDLER_G155_FCN_MAC_DATA_SIZE)){

                 memcpy(response, g155_control.data, g155_control.dataSize);//copy data & data size
                 *responseLen = g155_control.dataSize;

                 return NO_ERROR_NUMERATION;
             }
             break;*/
             
        case ASSIGN_MODBUS_ID_MTR: // Assign Serial Number Meter

             if ((scorpio_control.flag_password == HANDLER_SCORPIO_IS_A_FLAG_PASSWORD || scorpio_control.flag_system == HANDLER_SCORPIO_IS_A_FLAG_SYSTEM) && (scorpio_control.dataSize == HANDLER_SCORPIO_FLAG_WRITE_SN_SIZE)){

                 memcpy(response, scorpio_control.data, scorpio_control.dataSize);//copy data & data size
                 *responseLen = scorpio_control.dataSize;

                 return NO_ERROR_NUMERATION;
             }
             break;
             
        case REQUEST_SERIAL_NUMBER_MTR: // Request Serial Number Meter

             if ((scorpio_control.fcn == HANDLER_SCORPIO_FUNCTION_ID_LOW_VALUE) && (scorpio_control.dataSize == HANDLER_SCORPIO_FCN_SN_READ_DATA_SIZE)){

                 memcpy(response, scorpio_control.data, scorpio_control.dataSize);//copy data & data size
                 *responseLen = scorpio_control.dataSize;

                 return NO_ERROR_NUMERATION;
             }
             break;      
             
        default:
            
             return HANDLER_SCORPIO_ERROR_NUMERATION;
     }


     return HANDLER_SCORPIO_ERROR_NUMERATION;

}