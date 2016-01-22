#include <ctype.h>

#include "Meters_Table.h"
#include "SystemQueue.h"

Meters_Table            Meters_Table1;
sSM _tAddDelMeterSM = {0,0,0,0};
BYTE macLongAddrByteInverse[MAC_SIZE] = {0x24, 0xda, 0xb6, 0x0a, 0x02, 0x00, 0x07, 0x0c};
BYTE macLongAddrByte[MAC_SIZE];
/*Add_Del State Machine*/
void vfnAddDelMeterIdleState(void);
void vfnAddDelMeterAddMeterState(void);
void vfnAddDelMeterSendReadingPLCState(void);
void vfnAddDelMeterWaitConfirmPLCState(void);
void vfnAddDelMeterSendLinkerONState(void);
void vfnAddDelMeterDelMeterState(void);
void vfnAddDelMeterDesassociateMeterState(void);
void vfnAddDelMeterSendLinkerOFFState(void);
void vfnAddDelMeterSendReadState(void);
void vfnAddDelMeterSendPSWState(void);
void vfnAddDelMeterSendCMDState(void);
void vfnAddDelMeterSendMACBroadcastState(void);
void vfnAddDelMeterAssignModbusIdState(void);
void vfnAddDelMeterRequestSerialNumberByModbusIdState(void);
void vfnAddDelMeterEndState(void);

/*Add_Del State Machine*/
typedef enum
{
    _ADDDELMETER_IDLE_STATE = 0,
    _ADDDELMETER_ADD_MTR_STATE,
    #ifdef ONLY_ZIGBEE_IPD
    _ADDDELMETER_SEND_READING_PLC_STATE,
    _ADDDELMETER_WAIT_CONFIRM_PLC_STATE,
    #endif
    _ADDDELMETER_SEND_LINK_ON_STATE,
    _ADDDELMETER_DEL_MTR_STATE,
    _ADDDELMETER_DESASSOCIATE_MTR_STATE,
    _ADDDELMETER_SEND_LINK_OFF_STATE,
    _ADDDELMETER_SEND_READ_STATE,
    _ADDDELMETER_SEND_PSW_STATE,
    _ADDDELMETER_SEND_CMD_STATE,
    #ifdef ANSI_C12_RUN
    _ADDDELMETER_SEND_ANSI_STATUS_STATE,
    _ADDDELMETER_SEND_ANSI_DATA_STATE,
    #endif
    _ADDDELMETER_SEND_MAC_BROADCAST_STATE,
    _ADDDELMETER_ASSIGN_MODBUS_ID_STATE,
    _ADDDELMETER_REQUEST_SERIAL_NUMBER_BY_MODBUS_ID_STATE,
    _ADDDELMETER_END_STATE
}eIICDriverStates;

void (*const vfnaAddDelMeterDriverState[])(void) =
{
    vfnAddDelMeterIdleState,
    vfnAddDelMeterAddMeterState,
    #ifdef ONLY_ZIGBEE_IPD
    vfnAddDelMeterSendReadingPLCState,
    vfnAddDelMeterWaitConfirmPLCState,
    #endif
    vfnAddDelMeterSendLinkerONState,
    vfnAddDelMeterDelMeterState,
    vfnAddDelMeterDesassociateMeterState,
    vfnAddDelMeterSendLinkerOFFState,
    vfnAddDelMeterSendReadState,
    vfnAddDelMeterSendPSWState,
    vfnAddDelMeterSendCMDState,
    #ifdef ANSI_C12_RUN
    vfnAddDelMeterSendANSIStatusState,
    vfnAddDelMeterSendANSIDataState,
    #endif
    vfnAddDelMeterSendMACBroadcastState,
    vfnAddDelMeterAssignModbusIdState,
    vfnAddDelMeterRequestSerialNumberByModbusIdState,
    vfnAddDelMeterEndState
};

void vfnAddDelMeterDriver(void){
    vfnaAddDelMeterDriverState[_tAddDelMeterSM.bActualState]();
}

void vfnAddDelMeterIdleState(void){
    
}

void vfnAddDelMeterAddMeterState(void){
    
}

void vfnAddDelMeterSendReadingPLCState(void){
    
}

void vfnAddDelMeterWaitConfirmPLCState(void){
    
}

void vfnAddDelMeterSendLinkerONState(void){
    
    MeterControl_SendCommand( LINK_ADDING_MTR,                                            /*  command                     */
                            macLongAddrByte,                                               /*  data                        */
                            SHORT_MAC_SIZE,                                                  /*  dataLen                     */
                            TRUE,                                               /*  answerRequired              */
                            _1000_MSEC_,                                         /*  timeoutValue                */
                            METER_CONTROL_DEFAULT_NUMBER_OF_RETRIES,            /*  maxNumberOfRetries          */
                            0,                                                  /*  stabilizationTimeoutValue   */
                            _ADDDELMETER_END_STATE);                       /*  nextState                   */    
}

void vfnAddDelMeterDelMeterState(void){
    
}

void vfnAddDelMeterDesassociateMeterState(void){
    
}

void vfnAddDelMeterSendLinkerOFFState(void){
    
    BYTE dumbShortMacAddress[SHORT_MAC_SIZE];
    
    memset(dumbShortMacAddress, 0xFF, SHORT_MAC_SIZE);
    
    MeterControl_SendCommand( LINK_DELETING_MTR,                                            /*  command                     */
                            dumbShortMacAddress,                                               /*  data                        */
                            SHORT_MAC_SIZE,                                                  /*  dataLen                     */
                            TRUE,                                               /*  answerRequired              */
                            _1000_MSEC_,                                         /*  timeoutValue                */
                            METER_CONTROL_DEFAULT_NUMBER_OF_RETRIES,            /*  maxNumberOfRetries          */
                            0,                                                  /*  stabilizationTimeoutValue   */
                            _ADDDELMETER_END_STATE);                       /*  nextState                   */    
}

void vfnAddDelMeterSendReadState(void){
    
    MeterControl_SendCommand( READ_MODE,                                          /*  command                     */
                            NULL,                                               /*  data                        */
                            0,                                                  /*  dataLen                     */
                            TRUE,                                               /*  answerRequired              */
                            _2000_MSEC_,                                         /*  timeoutValue                */
                            METER_CONTROL_DEFAULT_NUMBER_OF_RETRIES,            /*  maxNumberOfRetries          */
                            0,                                                  /*  stabilizationTimeoutValue   */
                            _ADDDELMETER_END_STATE);                            /*  nextState                   */
    
}

void vfnAddDelMeterSendPSWState(void){
    
    BYTE commandId =   MeterControl_GetCommandId();  
    
    MeterControl_SendCommand( PSW_MTR,                                            /*  command                     */
                            &commandId,                                               /*  data                        */
                            sizeof(commandId),                                                  /*  dataLen                     */
                            (MeterControl_IsBroadcastSent() ? FALSE : TRUE),                                               /*  answerRequired              */
                            _1000_MSEC_,                                         /*  timeoutValue                */
                            METER_CONTROL_DEFAULT_NUMBER_OF_RETRIES,            /*  maxNumberOfRetries          */
                            0,                                                  /*  stabilizationTimeoutValue   */
                            _ADDDELMETER_SEND_CMD_STATE);                       /*  nextState                   */
}

void vfnAddDelMeterSendCMDState(void){
    
    MeterControl_SendCommand( MeterControl_GetCommandId(),                                                /*  command                     */
                            NULL,                                                                       /*  data                        */
                            0,                                                                          /*  dataLen                     */
                            (MeterControl_IsBroadcastSent() ? FALSE : TRUE),                                                                   /*  answerRequired              */
                            _1000_MSEC_,                                                                                                       /*  timeoutValue                */
                            METER_CONTROL_DEFAULT_NUMBER_OF_RETRIES,                                                                           /*  maxNumberOfRetries          */
                            (MeterControl_IsBroadcastSent() ? 0 : MeterInterface_GetStabilizationTimeoutValue(MeterControl_GetMeterType())),   /*  stabilizationTimeoutValue   */
                            (MeterControl_IsBroadcastSent() ? _ADDDELMETER_END_STATE : _ADDDELMETER_SEND_READ_STATE ));                        /*  nextState                   */
}

void vfnAddDelMeterSendMACBroadcastState(void){
    
    MeterControl_SendCommandByIdentificator(  BROADCAST_MDB,                          /*  modbusId                    */                                            
                                            NULL,                                   /*  serialNumber                */
                                            0,                                      /*  serialNumberLen             */
                                            SEND_MAC_BROADCAST_MTR,                 /*  command                     */
                                            macLongAddrByteInverse,                 /*  data                        */
                                            sizeof(macLongAddrByteInverse),         /*  dataLen                     */
                                            FALSE,                                  /*  answerRequired              */
                                            0,                                      /*  timeoutValue                */
                                            0,                                      /*  maxNumberOfRetries          */
                                            _100_MSEC_ ,                            /*  stabilizationTimeoutValue   */
                                            _ADDDELMETER_ASSIGN_MODBUS_ID_STATE);   /*  nextState                   */   
}

void vfnAddDelMeterAssignModbusIdState(void){
    
    BYTE modbusId;
    
    modbusId = findAvailableModbusId();
    
    if (modbusId == METER_TABLE_NO_METER_ID_FOUND){
        
        MeterControl_SetStateMachine(_ADDDELMETER_END_STATE);
    }
    
    MeterControl_SendCommandByIdentificator(  BROADCAST_MDB_ESP,                                      /*  modbusId                    */
                                            NULL,                                                   /*  serialNumber                */
                                            0,                                                      /*  serialNumberLen             */
                                            ASSIGN_MODBUS_ID_MTR,                                   /*  command                     */
                                            &modbusId,                                              /*  data                        */
                                            sizeof(modbusId),                                       /*  dataLen                     */
                                            FALSE,                                                  /*  answerRequired              */
                                            0,                                                      /*  timeoutValue                */
                                            0,                                                      /*  maxNumberOfRetries          */
                                            _100_MSEC_ ,                                            /*  stabilizationTimeoutValue   */
                                            _ADDDELMETER_REQUEST_SERIAL_NUMBER_BY_MODBUS_ID_STATE); /*  nextState                   */
}

void vfnAddDelMeterRequestSerialNumberByModbusIdState(void){
    
    BYTE modbusId;
    
    modbusId = findAvailableModbusId();
    
    if (modbusId == METER_TABLE_NO_METER_ID_FOUND){
        
        MeterControl_SetStateMachine(_ADDDELMETER_END_STATE);
    }
    
    MeterControl_SendCommandByIdentificator(  modbusId,                                   /*  modbusId                    */
                                            NULL,                                       /*  serialNumber                */
                                            0,                                          /*  serialNumberLen             */
                                            REQUEST_SERIAL_NUMBER_MTR,                  /*  command                     */
                                            NULL,                                       /*  data                        */
                                            0,                                          /*  dataLen                     */
                                            TRUE,                                       /*  answerRequired              */
                                            _1000_MSEC_ ,                                /*  timeoutValue                */
                                            METER_CONTROL_DEFAULT_NUMBER_OF_RETRIES,    /*  maxNumberOfRetries          */
                                            0 ,                                         /*  stabilizationTimeoutValue   */
                                            _ADDDELMETER_END_STATE);                    /*  nextState                   */
}

void vfnAddDelMeterEndState(void){
    
    if(MeterControl_GetStabilizationTimeout() == METER_TIMEOUT_INITIALIZED)
        return;
    
    API_MeterTable_SetCommandMeterBusy(FALSE);
    
    if ( API_MeterControl_QueueInfoCheck() == METER_TABLE_METER_NO_ERROR_CODE)
        return;    
        
    vfnEventClear(ADD_DEL_MTR_EVENT);
    vfnEventDisable(ADD_DEL_MTR_EVENT);
    
}

//******************************************************************************
// Meter Table Function
//******************************************************************************

BYTE MeterTable_GetMeterTypeByMeterId(BYTE meterId){
    
    BYTE index;
    BYTE meterTypeFromTable;
    BYTE meterTypeFromAPI;
    
    if (meterId > NUM_MAX_METERS)
        return METER_TABLE_METER_ID_ERROR_CODE;
  
    index = 0;
    meterTypeFromTable = Meters_Table1.Meter_DEV[meterId].Type;    
    
    while(TRUE){
        
        meterTypeFromAPI = MeterInterface_GetMeterTypeByIndex(index);
        
        if( meterTypeFromTable == meterTypeFromAPI)
            break;
        
        if( (meterTypeFromAPI == METER_INTERFACE_NO_METER_TYPE_ERROR_CODE) ||
            (meterTypeFromAPI == METER_INTERFACE_METER_TYPE_INDEX_OVERFLOW_ERROR_CODE))
            break;
        
        index++;
    }
    
    return meterTypeFromAPI;     
}

BYTE MeterTable_GetModbusIdByMeterId(BYTE meterId){
    
    if (meterId > NUM_MAX_METERS)
        return METER_TABLE_METER_ID_ERROR_CODE;
    
    return (Meters_Table1.Meter_DEV[meterId].Signature) - SIGNATURE_MTR + METER_TABLE_OFFSET_SIGNATURE_MTR;
}

BYTE MeterTable_GetSerialNumberByMeterId(BYTE meterId, BYTE * serialNumber, BYTE serialNumberLen){
    
    if (meterId > NUM_MAX_METERS)
        return 0;
    
    if(serialNumberLen < Lenght_Meter_ID)
        return 0;
    
    memset(serialNumber, 0, serialNumberLen);
    memcpy(serialNumber, Meters_Table1.Meter_DEV[meterId].Serial_Num, Lenght_Meter_ID);        
    return Lenght_Meter_ID;    
}

void MeterControl_SetStateMachine(BYTE actualState){
    
    _tAddDelMeterSM.bActualState = actualState;
    _tAddDelMeterSM.bNextState   = _ADDDELMETER_END_STATE;
    
    vfnEventEnable(ADD_DEL_MTR_EVENT);
    vfnEventPost(ADD_DEL_MTR_EVENT);
}

BYTE MeterControl_GetNextStateMachine(void){
    
    return _tAddDelMeterSM.bNextState;
}

BYTE findAvailableMeterId(void){
    
    BYTE index = 0;
    
    /*while ( ( Meters_Table1.Meter_DEV[index++].Signature != METER_TABLE_EMPTY_LOCATION) &&
            ( index < NUM_MAX_METERS));*/
    
    for(index = 0; Meters_Table1.Meter_DEV[index].Signature != METER_TABLE_EMPTY_LOCATION; index++){
        
        if (index >= NUM_MAX_METERS)
            break;
    }       
    
    if(index == NUM_MAX_METERS)
        return METER_TABLE_NO_METER_ID_FOUND;
    
    return index;
}

BYTE findAvailableModbusId(void){
    
    BYTE modbusId = findAvailableMeterId();
    return ++modbusId;
}

BOOL MeterTable_IsValidSerialNumber(BYTE * serialNumber, WORD serialNumberLen){
    
    BYTE index;
    
    if(serialNumberLen != Lenght_Meter_ID)
        return FALSE;
    
    for (index = 0; 
        (isalnum(* serialNumber) && (index < serialNumberLen));
        index++, serialNumber++);
    
    if (index < serialNumberLen)
        return FALSE;
            
     
    return TRUE;
}

BYTE MeterTable_FindMeterBySerialNumber(BYTE * serialNumber, WORD serialNumberLen){
    
    BYTE index;
    
    for (index = 0;
         (memcmp(Meters_Table1.Meter_DEV[index].Serial_Num, serialNumber, serialNumberLen) && (index < NUM_MAX_METERS));
         index++);
    
    if(index == NUM_MAX_METERS)
        return METER_TABLE_SERIAL_NUMBER_NOT_FOUND;
    
    return index;    
}

BYTE MeterTable_AddNewMeter(BYTE meterId, BYTE meterType,BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen){
    
    Meter_Eneri * meter_eneri_ptr = &Meters_Table1.Meter_DEV[meterId];
    
    /*if(bfnIIC_MEM24_1025_Write(serialNumber,
            Meter_Table_ADD+((meterId)*serialNumberLen),
            serialNumberLen) == FALSE)
        
        return METER_TABLE_COULD_NOT_SAVE_SERIAL_NUMBER_IN_NVM;*/
    
    memcpy(meter_eneri_ptr->Serial_Num, serialNumber, serialNumberLen);
    meter_eneri_ptr->Type = meterType;
    memset(meter_eneri_ptr->MACAdd_Display.v, METER_TABLE_EMPTY_VALUE, MAC_SIZE);
    meter_eneri_ptr->Signature = modbusId + SIGNATURE_MTR - METER_TABLE_OFFSET_SIGNATURE_MTR;
    
    printf("Adding new meter\n");
    return METER_TABLE_METER_NO_ERROR_CODE;
}

BYTE MeterTable_UpdateMeter(BYTE meterId, BYTE meterType, BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen){
    
    Meter_Eneri * meter_eneri_ptr = &Meters_Table1.Meter_DEV[meterId];
    
    if (memcmp(meter_eneri_ptr->Serial_Num, serialNumber, serialNumberLen))
        return METER_TABLE_SERIAL_NUMBER_NOT_MATCHED_ERROR_CODE;
    
    meter_eneri_ptr->Type = meterType;
    meter_eneri_ptr->Signature = modbusId + SIGNATURE_MTR - METER_TABLE_OFFSET_SIGNATURE_MTR;
    
    printf("Update meter\n");
    return METER_TABLE_METER_NO_ERROR_CODE;    
}

BYTE MeterTable_DeleteMeter(BYTE meterId, BYTE meterType,BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen){
    
    Meter_Eneri * meter_eneri_ptr = &Meters_Table1.Meter_DEV[meterId];
    
    /*if(bfnIIC_MEM24_1025_Write(serialNumber,
            Meter_Table_ADD+((meterId)*serialNumberLen),
            serialNumberLen) == FALSE)
        
        return METER_TABLE_COULD_NOT_SAVE_SERIAL_NUMBER_IN_NVM;*/
    
    memset(meter_eneri_ptr, METER_TABLE_EMPTY_VALUE, sizeof(Meter_Eneri));        
    printf("Deleting Meter\n");
    return METER_TABLE_METER_NO_ERROR_CODE;
}

BYTE MeterTable_AddNewMeterBySerialNumber(BYTE meterType, BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen){

    BYTE meterId;
    
    if(!MeterTable_IsValidSerialNumber(serialNumber, serialNumberLen))
        return METER_TABLE_SERIAL_NUMBER_ERROR_CODE;
    
    meterId = MeterTable_FindMeterBySerialNumber(serialNumber, serialNumberLen);
    
    if( meterId == METER_TABLE_SERIAL_NUMBER_NOT_FOUND)
    {        
        meterId = findAvailableMeterId();
        
        if(meterId == METER_TABLE_NO_METER_ID_FOUND)
            return METER_TABLE_NO_METER_ID_FOUND;
        
        return MeterTable_AddNewMeter(meterId, meterType, modbusId, serialNumber, serialNumberLen);
    }
    
    return MeterTable_UpdateMeter(meterId, meterType, modbusId, serialNumber, serialNumberLen);    
    //return METER_TABLE_METER_ALREADY_EXISTS;
}

BYTE MeterTable_DeleteMeterBySerialNumber(BYTE meterType, BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen){
    
    BYTE meterId;
    
    if(!MeterTable_IsValidSerialNumber(serialNumber, serialNumberLen))
        return METER_TABLE_SERIAL_NUMBER_ERROR_CODE;
    
    meterId = MeterTable_FindMeterBySerialNumber(serialNumber, serialNumberLen);
    
    if( meterId == METER_TABLE_SERIAL_NUMBER_NOT_FOUND)
        return METER_TABLE_SERIAL_NUMBER_NOT_FOUND;
        
    return MeterTable_DeleteMeter(meterId, meterType, modbusId, serialNumber, serialNumberLen);
}

BYTE MeterTable_SaveMeasurementBySerialNumber(BYTE meterType, BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, Data_Readings_Ptr measurement){
 
    BYTE meterId;
    WORD crc_measurement;
    Readings_Eneri * readings_eneri_ptr;
    
    if(!MeterTable_IsValidSerialNumber(serialNumber, serialNumberLen))
        return METER_TABLE_SERIAL_NUMBER_ERROR_CODE;
    
    meterId = MeterTable_FindMeterBySerialNumber(serialNumber, serialNumberLen);
    
    if( meterId == METER_TABLE_SERIAL_NUMBER_NOT_FOUND)
        return METER_TABLE_SERIAL_NUMBER_NOT_FOUND;
    
    if(measurement == NULL)
        return METER_TABLE_MEASUREMENT_ERROR_CODE;
    
    readings_eneri_ptr = &Meters_Table1.Readings[meterId];    
    memcpy(&readings_eneri_ptr->Reading, measurement, sizeof(Data_Readings));
    
    //readings_eneri_ptr->Reading.TIME_STAMP_Add = ZCL_Callback_GetCurrentTimeInSeconds();
    
    crc_measurement = wfnCRC_CALC((BYTE *) &readings_eneri_ptr->Reading, sizeof(Data_Readings), METER_TABLE_MEASUREMENT_CRC_BASE);
    
    inverted_memcpy((BYTE *) readings_eneri_ptr->CRC, (BYTE *) &crc_measurement, sizeof(crc_measurement));
    
    return METER_TABLE_METER_NO_ERROR_CODE;
}
//******************************************************************************
// API Meter Control Function
//******************************************************************************
BYTE MeterControl_ResponseHandler(BYTE meterType, BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE command)
{
    
    BYTE response[METER_TABLE_MAX_RESPONSE_SIZE];
    WORD maxResponseLen = sizeof(response);
    WORD responseLen;
    BYTE error_code;
    
    METER_COMMAND_ID_FUNCTION_API_PTR meterCommandIdFunctionAPI_ptr = MeterInterface_GetMeterCommandIdFunctionAPI(meterType);
    
    error_code = meterCommandIdFunctionAPI_ptr->meterHandler_ResponseProcessCallback(modbusId, serialNumber, serialNumberLen, command, response, maxResponseLen, &responseLen );
    
    if(error_code)
        return error_code;
    //! error_code needs to be processed here
    
    switch(command){
        
        case REQUEST_SERIAL_NUMBER_MTR:
            
            // Adding Meter Process
            return MeterTable_AddNewMeterBySerialNumber(meterType, modbusId, response,responseLen);
        
        case LINK_ADDING_MTR:            
            return MeterTable_AddNewMeterBySerialNumber(meterType, modbusId, serialNumber, serialNumberLen);
            
        case LINK_DELETING_MTR:
            
            return MeterTable_DeleteMeterBySerialNumber(meterType, modbusId, serialNumber, serialNumberLen);
            
        case READ_MODE:
            
            // It needs to Save Metering Data into Table
            if(responseLen != sizeof(Data_Readings))
                return METER_TABLE_MEASUREMENT_ERROR_CODE;
            
            return MeterTable_SaveMeasurementBySerialNumber(meterType, modbusId, serialNumber, serialNumberLen, (Data_Readings_Ptr) response);
            
        default:
            break;
    }
    
    return METER_TABLE_METER_NO_ERROR_CODE;
}

void MeterControl_ReceiveHandler(void){
    
    BYTE index = 0;
    BYTE meterType;
    METER_COMMAND_ID_FUNCTION_API_PTR meterCommandIdFunctionAPI_ptr = NULL;    
    BYTE * buffer_ptr;
    WORD buffersize;
    BYTE error_code;
    METER_DESCRIPTOR meterDescriptor;
    BYTE commandCallBack;
    
    buffer_ptr = ComSerialInterface_GetBuffer();    
    buffersize = ComSerialInterface_GetBufferSize();
    
    if ((buffersize == 0) || (buffer_ptr == NULL))
        return;
    
    while(TRUE){
        
        meterType = MeterInterface_GetMeterTypeByIndex(index);   
        
        if( (meterType == METER_INTERFACE_NO_METER_TYPE_ERROR_CODE) ||
            (meterType == METER_INTERFACE_METER_TYPE_INDEX_OVERFLOW_ERROR_CODE))
            break;
        
        meterCommandIdFunctionAPI_ptr = MeterInterface_GetMeterCommandIdFunctionAPI(meterType);
        error_code = meterCommandIdFunctionAPI_ptr->meterHandler_ReceiveProcessCallback(buffer_ptr, buffersize, &meterDescriptor, &commandCallBack );
        
        if(error_code == METER_TABLE_METER_NO_ERROR_CODE){
            
            MeterControl_SetDataAvailable(TRUE);
            API_MeterControl_ExcecuteCommandInvoke(&meterDescriptor, commandCallBack);
            //API_MeterTable_ExcecuteCommand( meterDescriptor.modbusId, meterDescriptor.serialNumber, meterDescriptor.serialNumberLen,commandCallBack, meterType);
            
            break;
        }            
        
        index++;
    } 
    
    printf("Arrived Data: ");
    ComSerialInterface_PrintData(buffer_ptr, buffersize);
    ComSerialInterface_CleanBuffer();
}

void MeterControl_SendCommand(    BYTE command ,
                                BYTE * data, 
                                WORD dataLen, 
                                BOOL answerRequired,
                                WORD timeoutValue, 
                                BYTE maxRetries,
                                WORD stabilizationTimeoutValue, 
                                BYTE nextState)
{

    BYTE modbusId;
    BYTE serialNumber[Lenght_Meter_ID];
    BYTE serialNumberLen;
    
    modbusId = MeterControl_GetModbusId();
    //modbusId = MeterTable_GetModbusIdByMeterId(MeterControl_GetMeterId());
    serialNumberLen = MeterControl_GetSerialNumber(serialNumber, sizeof(serialNumber));
    //serialNumberLen = MeterTable_GetSerialNumberByMeterId(MeterControl_GetMeterId(), serialNumber, sizeof(serialNumber));
    
    MeterControl_SendCommandByIdentificator(  modbusId,
                                            serialNumber,
                                            serialNumberLen,
                                            command,
                                            data,
                                            dataLen,
                                            answerRequired,
                                            timeoutValue,
                                            maxRetries,
                                            stabilizationTimeoutValue,
                                            nextState);
    
}

void MeterControl_SendCommandByIdentificator( BYTE modbusId, 
                                            BYTE * serialNumber, 
                                            WORD serialNumberLen, 
                                            BYTE command ,
                                            BYTE * data, 
                                            WORD dataLen, 
                                            BOOL answerRequired,
                                            WORD timeoutValue, 
                                            BYTE maxRetries,
                                            WORD stabilizationTimeoutValue, 
                                            BYTE nextState)
{
        
    COMMAND_ID_FUNCTION_PTR commandIdFunction_ptr;
    BYTE retries = MeterControl_GetRetries();
    BYTE meterType = MeterControl_GetMeterType();
    
    METER_COMMAND_ID_FUNCTION_API_PTR meterCommandIdFunctionAPI_ptr = 
            MeterInterface_GetMeterCommandIdFunctionAPI(meterType);        
    
    MeterControl_SetAnswerRequired(answerRequired);
    
    if(meterCommandIdFunctionAPI_ptr == NULL)
        return;
    
    if(MeterControl_GetStabilizationTimeout() == METER_TIMEOUT_INITIALIZED)
        return;
    
    if ((MeterControl_IsAnswerRequired() == TRUE) && (MeterControl_IsDataAvailable() == TRUE)) {
        
        if(MeterControl_ResponseHandler(meterType, modbusId, serialNumber, serialNumberLen, command) != 
                METER_TABLE_METER_NO_ERROR_CODE){
         
            MeterControl_SetDataAvailable(FALSE);
            return;
        }            
        MeterControl_SendNextCommand(stabilizationTimeoutValue, nextState);
        return;
    }
            
    if (((MeterControl_GetResponseTimeout() != METER_TIMEOUT_INITIALIZED) && (retries < maxRetries)) || 
         (MeterControl_IsAnswerRequired() == FALSE))
    {

        commandIdFunction_ptr = MeterInterface_GetCommandIdFunction(meterCommandIdFunctionAPI_ptr, command);

        if(commandIdFunction_ptr != NULL){

            commandIdFunction_ptr->CommandIdFunction_Callback(modbusId, serialNumber, serialNumberLen, data, dataLen);   

            if(MeterControl_IsAnswerRequired() == TRUE){

                MeterControl_InitializeResponseTimeout(timeoutValue);
                retries++;
                MeterControl_SetRetries(retries);

            } else {

                MeterControl_SendNextCommand(stabilizationTimeoutValue, nextState);
            }

            return;
        }
     
        MeterControl_ErrorReset();        
    }
    
    if ( (MeterControl_GetResponseTimeout() == METER_TIMEOUT_EXPIRED) && (retries == maxRetries))
    {
        MeterControl_ErrorReset();
    }
    
    return;
}

void MeterControl_SendNextCommand(WORD stabilizationTimeoutValue, BYTE nextState ) {
              
    MeterControl_Reset(stabilizationTimeoutValue);    
    MeterControl_SetStateMachine( nextState);
    
    printf("Command Processed Successfully\n");
}

void MeterControl_ErrorReset(void){
    
    MeterControl_SetStateMachine(_ADDDELMETER_END_STATE);
    MeterControl_Clear();
    printf("Meter Error Response\n");
}

BYTE API_MeterControl_SendCommand(BYTE meterId, BYTE commandId){
    
    BYTE meterType;
    BYTE index;
    BYTE modbusId;
    WORD serialNumberLen;
    BYTE serialNumber[Lenght_Meter_ID];
    
    BYTE broadcastId;
    BYTE broadcastSerialNumber[Lenght_Meter_ID];
    WORD broadcastSerialNumberLen;
    
    if(meterId != METER_TABLE_BROADCAST_METER_ID){
        
        meterType = MeterTable_GetMeterTypeByMeterId(meterId);

        if ((meterType == METER_TABLE_METER_ID_ERROR_CODE) || 
            (meterType == METER_INTERFACE_NO_METER_TYPE_ERROR_CODE))
            return meterType;
        
        modbusId = MeterTable_GetModbusIdByMeterId(meterId);
        serialNumberLen = MeterTable_GetSerialNumberByMeterId(meterId, serialNumber, sizeof(serialNumber));
        
        return API_MeterControl_ExcecuteCommand(modbusId, serialNumber, serialNumberLen, commandId, meterType, FALSE);    
    }
    
    index = 0;
    while(TRUE){
        
        meterType = MeterInterface_GetMeterTypeByIndex(index);   
        
        if( (meterType == METER_INTERFACE_NO_METER_TYPE_ERROR_CODE) ||
            (meterType == METER_INTERFACE_METER_TYPE_INDEX_OVERFLOW_ERROR_CODE))
            break;
        
        broadcastId = MeterInterface_GetBroadcastId(meterType);
        broadcastSerialNumberLen = MeterInterface_GetBroadcastSerialNumber(meterType, broadcastSerialNumber, sizeof(broadcastSerialNumber));
        
        API_MeterControl_ExcecuteCommand(broadcastId, broadcastSerialNumber, broadcastSerialNumberLen, commandId, meterType, TRUE);    
        index++;
    }
    
    if(index == 0)
        return meterType;
    
    return METER_TABLE_METER_NO_ERROR_CODE;
}

QUEUE_LIST_PTR queueControlList = NULL;

BOOL API_MeterControl_IsCommandMeterBusy(void){

    return sFlags.CMD_Meter_Busy;
}

void API_MeterTable_SetCommandMeterBusy(BOOL state){

    sFlags.CMD_Meter_Busy = state;
}

BYTE API_MeterControl_QueueInfoCheck(void){
    
    QUEUE_ELEMENT_PTR queueElement_ptr = NULL;
    METER_CONTROL queueInfo;
    
    if(queueControlList != NULL){
        
        queueElement_ptr = queueControlList->head;
        
        if(queueElement_ptr != NULL){
            
            if(queueElement_ptr->infoSize == sizeof(METER_CONTROL)){
                
                memcpy((BYTE *) &queueInfo, (BYTE *) queueElement_ptr->info, queueElement_ptr->infoSize);
                QueueList_RemoveElement(queueControlList);
                
                API_MeterControl_ExcecuteCommand(queueInfo.meterDescriptor.modbusId, queueInfo.meterDescriptor.serialNumber, queueInfo.meterDescriptor.serialNumberLen, queueInfo.commandId, queueInfo.meterDescriptor.meterType, queueInfo.broadcastSent);
                return METER_TABLE_METER_NO_ERROR_CODE;
            }
        }
    }        
    
    return METER_TABLE_EMPTY_QUEUE;
}

BYTE API_MeterControl_ExcecuteCommand(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE commandId, BYTE meterType, BOOL broadcastSent){
//BYTE API_MeterTable_ExcecuteCommand(BYTE meterId, BYTE commandId, BYTE meterType){
    
    METER_CONTROL queueInfo;
    BYTE error_code;
    
    if( API_MeterControl_IsCommandMeterBusy() == TRUE){
        
        if(queueControlList == NULL)
            queueControlList = QueueList_New();        
        
        queueInfo.commandId = commandId;
        queueInfo.broadcastSent = broadcastSent;
        MeterDescriptor_SetMeterType(&queueInfo.meterDescriptor, meterType);
        MeterDescriptor_SetModbusId(&queueInfo.meterDescriptor, modbusId);
        MeterDescriptor_SetSerialNumber(&queueInfo.meterDescriptor, serialNumber, serialNumberLen);        
        
        QueueList_AddElement(queueControlList, (BYTE *) &queueInfo, sizeof(queueInfo));        
        
        return METER_TABLE_COMMAND_METER_API_BUSY;
    }
    
    error_code = MeterControl_ExcecuteCommand(modbusId, serialNumber, serialNumberLen, commandId, meterType, broadcastSent);
    
    if(error_code == TRUE)
        API_MeterTable_SetCommandMeterBusy(TRUE);
    
    return error_code;
}

BYTE MeterControl_ExcecuteCommand(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE commandId, BYTE meterType, BOOL broadcastSent){
//BYTE MeterTable_ExcecuteCommand(BYTE meterId, BYTE commandId, BYTE meterType){
     
    BYTE nextState;    
    
    switch(commandId)
    {
        case Dis_MTR:
        case Con_MTR:
        case Res_MTR:
            
            nextState = _ADDDELMETER_SEND_PSW_STATE;
            break;
            
        case READ_MODE:
            nextState = _ADDDELMETER_SEND_READ_STATE;
            break;
            
        case SEND_MAC_BROADCAST_MTR:
            nextState = _ADDDELMETER_SEND_MAC_BROADCAST_STATE ;  
            break;
            
        case LINK_ADDING_MTR:
            nextState = _ADDDELMETER_SEND_LINK_ON_STATE;
            break;
            
        case LINK_DELETING_MTR:
            nextState = _ADDDELMETER_SEND_LINK_OFF_STATE;
            break;

        default:            
            return FALSE;
    }
    
    MeterControl_Setup(modbusId, serialNumber, serialNumberLen, meterType, commandId, broadcastSent, 0);        
    MeterControl_SetStateMachine(nextState);
    
    return TRUE;    
}

void API_MeterControl_ExcecuteBaptismProccess(void){
       
    API_MeterControl_ExcecuteCommand(0, NULL, 0, SEND_MAC_BROADCAST_MTR, G155_TYPE, FALSE);
}

void API_MeterControl_ExcecuteCommandInvoke( METER_DESCRIPTOR_PTR meterDescriptor, BYTE commandCallBack){
    
    printf("Serial Number: ");
    ComSerialInterface_PrintData(meterDescriptor->serialNumber, meterDescriptor->serialNumberLen);
    printf("\n");
    
    switch(commandCallBack){
        
        case LINK_ADDING_MTR:
        case LINK_DELETING_MTR:
            
            API_MeterControl_ExcecuteCommand(meterDescriptor->modbusId, meterDescriptor->serialNumber, meterDescriptor->serialNumberLen, commandCallBack, meterDescriptor->meterType, FALSE);
            break;
        
        default:
            return;
    }
}
/*
 * It is necessary to add modbusId, SerialNumber and SerialNumberLen 
 * into MeterControl struct.
 * 
 * In case that meter does not need to be calling by serial number or modbusId,
 * those values should be filled with NULL and ZERO values.
 */