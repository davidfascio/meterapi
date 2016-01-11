#include "MeterInterface.h"

METER_COMMAND_ID_FUNCTION_API meterCommandIdFunctionAPIList [] = 
{
    {   G155_TYPE,                                              /*  Meter Type                                           */
        0x00,                                                   /*  Broadcast Id                                         */
        NULL,                                                   /*  SerialNumber Broadcast                               */
        0,                                                      /*  SerialNumber Broadcast Len                           */
        _2200_MSEC_,                                            /*  Stabilization Time Value                             */
        G155MeterInterface_CommandIdFunctionList,               /*  Pointer to Command Id Function List per Meter Type   */
        API_G155_Meter_response_handler,                        /*  Pointer to Meter Response Handler                             */
        API_G155_Recieve_handler},                              /*  Pointer to Meter Receive  Handler                             */
    
   
        
    METER_COMMAND_ID_FUNCTION_API_NULL    
};

#define METER_COMMAND_ID_FUNCTION_API_LIST_SIZE                 ( sizeof(meterCommandIdFunctionAPIList) / sizeof(METER_COMMAND_ID_FUNCTION_API) )

WORD MeterInterface_GetMeterTypeByIndex(BYTE index){
    
    METER_COMMAND_ID_FUNCTION_API_PTR meterCommandIdFunctionAPI_ptr = meterCommandIdFunctionAPIList;
    
    if(index >= METER_COMMAND_ID_FUNCTION_API_LIST_SIZE)
        return METER_INTERFACE_METER_TYPE_INDEX_OVERFLOW_ERROR_CODE;
    
    meterCommandIdFunctionAPI_ptr = meterCommandIdFunctionAPI_ptr + index;
    
    if (meterCommandIdFunctionAPI_ptr->meterType == METER_INTERFACE_NO_METER_TYPE)
        return METER_INTERFACE_NO_METER_TYPE_ERROR_CODE;
    
    return meterCommandIdFunctionAPI_ptr->meterType;
}

METER_COMMAND_ID_FUNCTION_API_PTR MeterInterface_GetMeterCommandIdFunctionAPI(BYTE meterType){    
    
    METER_COMMAND_ID_FUNCTION_API_PTR meterCommandIdFunctionAPI_ptr = meterCommandIdFunctionAPIList;
    
    for ( ; meterCommandIdFunctionAPI_ptr->meterType != METER_INTERFACE_NO_METER_TYPE; ){
     
        if(meterCommandIdFunctionAPI_ptr->meterType == meterType)
            break;
        
        meterCommandIdFunctionAPI_ptr++;
    }   
    
    if(meterCommandIdFunctionAPI_ptr->meterType == METER_INTERFACE_NO_METER_TYPE)
        return NULL;
    
    return meterCommandIdFunctionAPI_ptr;
}

COMMAND_ID_FUNCTION_PTR MeterInterface_GetCommandIdFunction(METER_COMMAND_ID_FUNCTION_API_PTR meterCommandIdFunctionAPI, BYTE command){    
    
    COMMAND_ID_FUNCTION_PTR commandIdFunction_ptr;
    
    if (meterCommandIdFunctionAPI == NULL)
        return NULL;
    
    commandIdFunction_ptr = meterCommandIdFunctionAPI->commandIdFunctionList;
    
    for( ; commandIdFunction_ptr->commandId != NO_COMMAND_MTR; ){
        
        if(commandIdFunction_ptr->commandId == command)
            break;
        
        commandIdFunction_ptr++;
    }
    
    if(commandIdFunction_ptr->commandId == NO_COMMAND_MTR)
        return NULL;
    
    return commandIdFunction_ptr;    
}

WORD MeterInterface_GetStabilizationTimeoutValue(BYTE meterType){
    
    METER_COMMAND_ID_FUNCTION_API_PTR meterCommandIdFunctionAPI_ptr = 
                        MeterInterface_GetMeterCommandIdFunctionAPI(meterType);
    
    if(meterCommandIdFunctionAPI_ptr == NULL)
        return 0;
    
    return meterCommandIdFunctionAPI_ptr->stabilizationTimeoutValue;
}


