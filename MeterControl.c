//******************************************************************************
// Includes
//******************************************************************************
#include "MeterControl.h"

//******************************************************************************
// Meter Control Variables
//******************************************************************************
METER_CONTROL meterControl;
QUEUE_LIST_PTR queueControlList = NULL;

//******************************************************************************
//                  LOCAL METER CONTROL STATE MACHINE
//******************************************************************************

// Meter state machine functions
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

// Meter state machine enummeration
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
} eMCDriverStates;

void (*const vfnaAddDelMeterDriverState[])(void) =
{
    vfnAddDelMeterIdleState,                        /* _ADDDELMETER_IDLE_STATE              */
    vfnAddDelMeterAddMeterState,                    /* _ADDDELMETER_ADD_MTR_STATE           */
    #ifdef ONLY_ZIGBEE_IPD
    vfnAddDelMeterSendReadingPLCState,
    vfnAddDelMeterWaitConfirmPLCState,
    #endif
    vfnAddDelMeterSendLinkerONState,                /* _ADDDELMETER_SEND_LINK_ON_STATE      */
    vfnAddDelMeterDelMeterState,                    /* _ADDDELMETER_DEL_MTR_STATE           */
    vfnAddDelMeterDesassociateMeterState,           /* _ADDDELMETER_DESASSOCIATE_MTR_STATE  */
    vfnAddDelMeterSendLinkerOFFState,               /* _ADDDELMETER_SEND_LINK_OFF_STATE     */
    vfnAddDelMeterSendReadState,                    /* _ADDDELMETER_SEND_READ_STATE         */
    vfnAddDelMeterSendPSWState,                     /* _ADDDELMETER_SEND_PSW_STATE          */
    vfnAddDelMeterSendCMDState,                     /* _ADDDELMETER_SEND_CMD_STATE          */
    #ifdef ANSI_C12_RUN
    vfnAddDelMeterSendANSIStatusState,
    vfnAddDelMeterSendANSIDataState,
    #endif
    vfnAddDelMeterSendMACBroadcastState,            /* _ADDDELMETER_SEND_MAC_BROADCAST_STATE*/
    vfnAddDelMeterAssignModbusIdState,              /* _ADDDELMETER_ASSIGN_MODBUS_ID_STATE  */
    vfnAddDelMeterRequestSerialNumberByModbusIdState,   /* _ADDDELMETER_REQUEST_SERIAL_NUMBER_BY_MODBUS_ID_STATE */
    vfnAddDelMeterEndState                          /* _ADDDELMETER_END_STATE               */
};

// Meter state machine variables
sSM _tAddDelMeterSM = { _ADDDELMETER_IDLE_STATE,    /* bActualState     */ 
                        _ADDDELMETER_IDLE_STATE,    /* bNextState       */ 
                        _ADDDELMETER_IDLE_STATE,    /* bPrevState       */ 
                        _ADDDELMETER_IDLE_STATE };  /* bErrorState      */ 

//******************************************************************************
//                  LOCAL METER CONTROL STATE MACHINE
//******************************************************************************

//******************************************************************************
// Meter Control Set and Get Functions
//******************************************************************************

void MeterControl_Setup(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE meterType, BYTE commandId, BOOL broadcastSent, WORD stabilizationTimeoutValue)
{       
    MeterDescriptor_SetModbusId(&meterControl.meterDescriptor, modbusId);
    MeterDescriptor_SetSerialNumber(&meterControl.meterDescriptor, serialNumber, serialNumberLen);
    MeterDescriptor_SetMeterType(&meterControl.meterDescriptor, meterType);
        
    meterControl.commandId = commandId;    
    meterControl.retries   = 0;
    meterControl.broadcastSent = broadcastSent;
    meterControl.answerRequired = FALSE;
    //!meterControl.dataAvailable = FALSE;
    MeterInterface_SetDataAvailable(meterType, FALSE);
    MeterControl_StopResponseTimeout();
    MeterControl_InitializeStabilizationTimeout(stabilizationTimeoutValue);
}

void MeterControl_Reset(BYTE meterType, WORD stabilizationTimeoutValue){
    
    meterControl.retries = 0;
    meterControl.answerRequired = FALSE;
    //!meterControl.dataAvailable = FALSE;
    MeterInterface_SetDataAvailable(meterType, FALSE);
    MeterControl_StopResponseTimeout();
    MeterControl_InitializeStabilizationTimeout(stabilizationTimeoutValue);
}

void MeterControl_SetDataAvailable(BYTE meterType, BOOL dataAvailable){
    
    //!meterControl.dataAvailable = dataAvailable;
    MeterInterface_SetDataAvailable(meterType, dataAvailable);
}

BOOL MeterControl_IsDataAvailable(BYTE meterType){
    
    //!return meterControl.dataAvailable;
    return MeterInterface_IsDataAvailable(meterType);
}

void MeterControl_SetAnswerRequired(BOOL answerRequired){

    meterControl.answerRequired = answerRequired;
}

BOOL MeterControl_IsAnswerRequired(void){

    return meterControl.answerRequired;
}

void MeterControl_SetCommandId(BYTE commandId){
    
    meterControl.commandId = commandId;
}

BYTE MeterControl_GetCommandId(void){
    
    return meterControl.commandId;
}

BYTE MeterControl_GetMeterType(void){
    
    return MeterDescriptor_GetMeterType(&meterControl.meterDescriptor);            
}

BYTE MeterControl_GetRetries(void){
    
    return meterControl.retries;
}

void MeterControl_SetRetries(BYTE retries){
    
    meterControl.retries = retries;
}

BYTE MeterControl_GetModbusId(void){
    
    return MeterDescriptor_GetModbusId(&meterControl.meterDescriptor);  
}

WORD MeterControl_GetSerialNumber(BYTE * serialNumber, WORD serialNumberLen){
    
    return MeterDescriptor_GetSerialNumber(&meterControl.meterDescriptor, serialNumber, serialNumberLen);
}

METER_TIMEOUT MeterControl_GetStabilizationTimeout(void){
    
    return meterControl.stabilizationTimeout;
}

METER_TIMEOUT MeterControl_GetResponseTimeout(void){
    
    return meterControl.responseTimeout;
}

void MeterControl_Clear(BYTE meterType){
    
    memset(&meterControl, 0, sizeof(meterControl));
    MeterInterface_SetDataAvailable(meterType, FALSE);
}

void MeterControl_InitializeResponseTimeout(WORD timeoutValue){
    
    if(timeoutValue == 0)
        return MeterControl_StopResponseTimeout();
    
    vfnOneShotReload(TIME_OUT_METER_RESPONSE_ONESHOT, timeoutValue);
    meterControl.responseTimeout = METER_TIMEOUT_INITIALIZED;
}

void MeterControl_StopResponseTimeout(void){
    
    vfnOneShotDisable(TIME_OUT_METER_RESPONSE_ONESHOT);
    meterControl.responseTimeout = METER_TIMEOUT_STOP;
}

void MeterControl_ExpireResponseTimeout(void){
    vfnOneShotDisable(TIME_OUT_METER_RESPONSE_ONESHOT);
    meterControl.responseTimeout = METER_TIMEOUT_EXPIRED;
    print_warm("Meter response timeout expired");
}

void MeterControl_InitializeStabilizationTimeout(WORD timeoutValue){
    
    if(timeoutValue == 0)
        return MeterControl_StopStabilizationTimeout();
    
    vfnOneShotReload(TIME_OUT_METER_STABILIZE_ONESHOT, timeoutValue);
    meterControl.stabilizationTimeout = METER_TIMEOUT_INITIALIZED;
}

void MeterControl_StopStabilizationTimeout(void){
    vfnOneShotDisable(TIME_OUT_METER_STABILIZE_ONESHOT);
    meterControl.stabilizationTimeout = METER_TIMEOUT_STOP;
}

void MeterControl_ExpireStabilizationTimeout(void){
    vfnOneShotDisable(TIME_OUT_METER_STABILIZE_ONESHOT);
    meterControl.stabilizationTimeout = METER_TIMEOUT_EXPIRED;
    print_info("Meter statilization timeout expired");
}

void MeterControl_SetBroadcastSent(BOOL broadcastSent){

    meterControl.broadcastSent = broadcastSent;
}

BOOL MeterControl_IsBroadcastSent(void){
    
    return meterControl.broadcastSent;
}

//******************************************************************************
// Meter Control Functions
//******************************************************************************

void MeterControl_SendCommand(  BYTE command ,
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
    
    MeterControl_SendCommandByIdentificator(    modbusId,
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

void MeterControl_SendCommandByIdentificator(   BYTE modbusId, 
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
    
    if ((MeterControl_IsAnswerRequired() == TRUE) && (MeterControl_IsDataAvailable(meterType) == TRUE)) {
        
        if(API_MeterControl_ResponseHandler(meterType, modbusId, serialNumber, serialNumberLen, command) != 
                METER_TABLE_METER_NO_ERROR_CODE){
         
            MeterControl_SetDataAvailable(meterType, FALSE);
            return;
        }            
        MeterControl_SendNextCommand(meterType, stabilizationTimeoutValue, nextState);
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

                MeterControl_SendNextCommand(meterType, stabilizationTimeoutValue, nextState);
            }

            return;
        }
     
        MeterControl_ErrorReset(meterType);        
    }
    
    if ( (MeterControl_GetResponseTimeout() == METER_TIMEOUT_EXPIRED) && (retries == maxRetries))
    {
        MeterControl_ErrorReset(meterType);
    }
    
    return;
}

void MeterControl_SendNextCommand(BYTE meterType, WORD stabilizationTimeoutValue, BYTE nextState ) {
              
    MeterControl_Reset(meterType, stabilizationTimeoutValue);    
    MeterControl_SetStateMachine( nextState);    
    print_log("Command Processed Successfully");
}

void MeterControl_ErrorReset(BYTE meterType){
    
    MeterControl_SetStateMachine(_ADDDELMETER_END_STATE);
    MeterControl_Clear(meterType);
    print_error("Meter Error Response");
}

BYTE MeterControl_ExcecuteCommand(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE commandId, BYTE meterType, BOOL broadcastSent)
{   
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
            print_error("Excecute Command was not found");
            return FALSE;
    }
    
    
    print_info("Excecuting Command: ");
    
    
    MeterControl_Setup(modbusId, serialNumber, serialNumberLen, meterType, commandId, broadcastSent, 0);        
    MeterControl_SetStateMachine(nextState);
    
    MeterControl_Print();
    
    return TRUE;    
}

void MeterControl_Print( void ){
    
    print_message("Meter Type: %X, ",     meterControl.meterDescriptor.meterType);
    print_message("Modbus Id: %X, ",      meterControl.meterDescriptor.modbusId);
    print_message("Serial Number: %s, ",  meterControl.meterDescriptor.serialNumber);
    print_message("Command Id: %d, ",     meterControl.commandId);
    print_message("Is Broadcast Command: %d",     meterControl.broadcastSent);
}

//******************************************************************************
// API Meter Control Send Functions
//******************************************************************************

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
        meterType = MeterInterface_ValidateMeterType(meterType);

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
    
    return METER_CONTROL_NO_ERROR_CODE;
}

void API_MeterControl_ExcecuteBaptismProccess(void){
       
    API_MeterControl_ExcecuteCommand(0, NULL, 0, SEND_MAC_BROADCAST_MTR, G155_TYPE, FALSE);
}

BYTE API_MeterControl_ExcecuteCommand(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE commandId, BYTE meterType, BOOL broadcastSent)
{  
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
        
        print_debug("New Excecute Command was added into Queue List");
        
        return METER_CONTROL_API_METER_COMMAND_BUSY_ERROR_CODE;
    }
    
    error_code = MeterControl_ExcecuteCommand(modbusId, serialNumber, serialNumberLen, commandId, meterType, broadcastSent);
    
    if(error_code == TRUE){
        API_MeterTable_SetCommandMeterBusy(TRUE);
        return METER_CONTROL_NO_ERROR_CODE;
    }
    
    return METER_CONTROL_COMMAND_NOT_FOUND_ERROR_CODE;
}

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
                return METER_CONTROL_NO_ERROR_CODE;
            }
        }
    }        
    
    return METER_CONTROL_EMPTY_QUEUE_ERROR_CODE;
}

//******************************************************************************
// API Meter Control Handler Functions
//******************************************************************************

BYTE API_MeterControl_ResponseHandler(BYTE meterType, BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE command)
{
    
    BYTE response[METER_CONTROL_MAX_RESPONSE_SIZE];
    WORD maxResponseLen = sizeof(response);
    WORD responseLen;
    BYTE error_code;
    BYTE commandCallBack;
    METER_COMMAND_ID_FUNCTION_API_PTR meterCommandIdFunctionAPI_ptr = MeterInterface_GetMeterCommandIdFunctionAPI(meterType);
    
    error_code = meterCommandIdFunctionAPI_ptr->meterHandler_ResponseProcessCallback(modbusId, serialNumber, serialNumberLen, command, response, maxResponseLen, &responseLen, &commandCallBack );
    
    if(error_code)
        return error_code;
    //! error_code needs to be processed here
    
    switch(command){
        
        case REQUEST_SERIAL_NUMBER_MTR:
            
            // Adding Meter Process
            return MeterTable_AddNewMeterBySerialNumber(meterType, modbusId, response,responseLen);
        
        case LINK_ADDING_MTR:  
            modbusId = MeterTable_FindAvailableModbusId();
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

void API_MeterControl_ReceiveHandler(void){
    
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
    
    print_log("Arrived Data: ");
    ComSerialInterface_PrintData(buffer_ptr, buffersize);
    
    while(TRUE){
        
        meterType = MeterInterface_GetMeterTypeByIndex(index);   
        
        if( (meterType == METER_INTERFACE_NO_METER_TYPE_ERROR_CODE) ||
            (meterType == METER_INTERFACE_METER_TYPE_INDEX_OVERFLOW_ERROR_CODE))
            break;
        
        meterCommandIdFunctionAPI_ptr = MeterInterface_GetMeterCommandIdFunctionAPI(meterType);
        error_code = meterCommandIdFunctionAPI_ptr->meterHandler_ReceiveProcessCallback(buffer_ptr, buffersize, &meterDescriptor, &commandCallBack );
        
        if(error_code == METER_CONTROL_NO_ERROR_CODE){
            
            print_log("Data Available Cast as a MeterType: %X", meterType );
            MeterControl_SetDataAvailable(meterType, TRUE);
            API_MeterControl_ExcecuteCommandInvoke(&meterDescriptor, commandCallBack);
            //API_MeterTable_ExcecuteCommand( meterDescriptor.modbusId, meterDescriptor.serialNumber, meterDescriptor.serialNumberLen,commandCallBack, meterType);
            
            //!break;
        }            
        
        index++;
    } 
    
    ComSerialInterface_CleanBuffer();
}

void API_MeterControl_ExcecuteCommandInvoke( METER_DESCRIPTOR_PTR meterDescriptor, BYTE commandCallBack){
    
        
    switch(commandCallBack){
        
        case LINK_ADDING_MTR:
        case LINK_DELETING_MTR:
            
            API_MeterControl_ExcecuteCommand(meterDescriptor->modbusId, meterDescriptor->serialNumber, meterDescriptor->serialNumberLen, commandCallBack, meterDescriptor->meterType, FALSE);
            break;
        
        default:
            return;
    }
}

//******************************************************************************
// Local Meter Control State Machine Function Protoypes
//******************************************************************************

void vfnAddDelMeterDriver(void){
    
    vfnaAddDelMeterDriverState[_tAddDelMeterSM.bActualState]();
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

//******************************************************************************
//                  LOCAL METER CONTROL STATE MACHINE
//******************************************************************************

void vfnAddDelMeterIdleState(void){
    
    // DO NOTHING STATE MACHINE    
}

void vfnAddDelMeterAddMeterState(void){
    
    // DO NOTHING STATE MACHINE
    //! This state is not supported more, the new meter is adding by API_MeterControl_ResponseHandler
    //! API_MeterControl_ReceiveHandler and API_MeterControl_ExcecuteCommandInvoke.
}

void vfnAddDelMeterSendReadingPLCState(void){
    
}

void vfnAddDelMeterWaitConfirmPLCState(void){
    
}

void vfnAddDelMeterSendLinkerONState(void){
    
    MeterControl_SendCommand(   LINK_ADDING_MTR,                                /*  command                     */
                                macLongAddrByte,                                /*  data                        */
                                SHORT_MAC_SIZE,                                 /*  dataLen                     */
                                TRUE,                                           /*  answerRequired              */
                                _1000_MSEC_,                                    /*  timeoutValue                */
                                METER_CONTROL_DEFAULT_NUMBER_OF_RETRIES,        /*  maxNumberOfRetries          */
                                0,                                              /*  stabilizationTimeoutValue   */
                                _ADDDELMETER_END_STATE);                        /*  nextState                   */    
}

void vfnAddDelMeterDelMeterState(void){
    
    // DO NOTHING STATE MACHINE
    //! This state is not supported more, the new meter is adding by API_MeterControl_ResponseHandler
    //! API_MeterControl_ReceiveHandler and API_MeterControl_ExcecuteCommandInvoke.
}

void vfnAddDelMeterDesassociateMeterState(void){
    
}

void vfnAddDelMeterSendLinkerOFFState(void){
    
    BYTE dumbShortMacAddress[SHORT_MAC_SIZE];
    
    memset(dumbShortMacAddress, 0xFF, SHORT_MAC_SIZE);
    
    MeterControl_SendCommand(   LINK_DELETING_MTR,                              /*  command                     */
                                dumbShortMacAddress,                            /*  data                        */
                                SHORT_MAC_SIZE,                                 /*  dataLen                     */
                                TRUE,                                           /*  answerRequired              */
                                _1000_MSEC_,                                    /*  timeoutValue                */
                                METER_CONTROL_DEFAULT_NUMBER_OF_RETRIES,        /*  maxNumberOfRetries          */
                                0,                                              /*  stabilizationTimeoutValue   */
                                _ADDDELMETER_END_STATE);                        /*  nextState                   */    
}

void vfnAddDelMeterSendReadState(void){
    
    MeterControl_SendCommand(   READ_MODE,                                      /*  command                     */
                                NULL,                                           /*  data                        */
                                0,                                              /*  dataLen                     */
                                TRUE,                                           /*  answerRequired              */
                                _2000_MSEC_,                                    /*  timeoutValue                */
                                METER_CONTROL_DEFAULT_NUMBER_OF_RETRIES,        /*  maxNumberOfRetries          */
                                0,                                              /*  stabilizationTimeoutValue   */
                                _ADDDELMETER_END_STATE);                        /*  nextState                   */
    
}

void vfnAddDelMeterSendPSWState(void){
    
    BYTE commandId =   MeterControl_GetCommandId();  
    
    MeterControl_SendCommand(   PSW_MTR,                                        /*  command                     */
                                &commandId,                                     /*  data                        */
                                sizeof(commandId),                              /*  dataLen                     */
                                (MeterControl_IsBroadcastSent() ? FALSE : TRUE),/*  answerRequired              */
                                _1000_MSEC_,                                    /*  timeoutValue                */
                                METER_CONTROL_DEFAULT_NUMBER_OF_RETRIES,        /*  maxNumberOfRetries          */
                                _100_MSEC_,                                              /*  stabilizationTimeoutValue   */
                                _ADDDELMETER_SEND_CMD_STATE);                   /*  nextState                   */
}

void vfnAddDelMeterSendCMDState(void){
    
    MeterControl_SendCommand(   MeterControl_GetCommandId(),                                                                                    /*  command                     */
                                NULL,                                                                                                           /*  data                        */
                                0,                                                                                                              /*  dataLen                     */
                                (MeterControl_IsBroadcastSent() ? FALSE : TRUE),                                                                /*  answerRequired              */
                                _1000_MSEC_,                                                                                                    /*  timeoutValue                */
                                METER_CONTROL_DEFAULT_NUMBER_OF_RETRIES,                                                                        /*  maxNumberOfRetries          */
                                (MeterControl_IsBroadcastSent() ? 0 : MeterInterface_GetStabilizationTimeoutValue(MeterControl_GetMeterType())),/*  stabilizationTimeoutValue   */
                                (MeterControl_IsBroadcastSent() ? _ADDDELMETER_END_STATE : _ADDDELMETER_SEND_READ_STATE ));                     /*  nextState                   */
}

void vfnAddDelMeterSendMACBroadcastState(void){
    
    MeterControl_SendCommandByIdentificator(    BROADCAST_MDB,                          /*  modbusId                    */                                            
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
    
    modbusId = MeterTable_FindAvailableModbusId();
    
    if (modbusId == METER_TABLE_NO_METER_ID_FOUND){
        
        MeterControl_SetStateMachine(_ADDDELMETER_END_STATE);
    }
    
    MeterControl_SendCommandByIdentificator(    BROADCAST_MDB_ESP,                                      /*  modbusId                    */
                                                NULL,                                                   /*  serialNumber                */
                                                0,                                                      /*  serialNumberLen             */
                                                ASSIGN_MODBUS_ID_MTR,                                   /*  command                     */
                                                &modbusId,                                              /*  data                        */
                                                sizeof(modbusId),                                       /*  dataLen                     */
                                                FALSE,                                                  /*  answerRequired              */
                                                0,                                                      /*  timeoutValue                */
                                                0,                                                      /*  maxNumberOfRetries          */
                                                _500_MSEC_ ,                                            /*  stabilizationTimeoutValue   */
                                                _ADDDELMETER_REQUEST_SERIAL_NUMBER_BY_MODBUS_ID_STATE); /*  nextState                   */
}

void vfnAddDelMeterRequestSerialNumberByModbusIdState(void){
    
    BYTE modbusId;
    
    modbusId = MeterTable_FindAvailableModbusId();
    
    if (modbusId == METER_TABLE_NO_METER_ID_FOUND){
        
        MeterControl_SetStateMachine(_ADDDELMETER_END_STATE);
    }
    
    MeterControl_SendCommandByIdentificator(    modbusId,                                   /*  modbusId                    */
                                                NULL,                                       /*  serialNumber                */
                                                0,                                          /*  serialNumberLen             */
                                                REQUEST_SERIAL_NUMBER_MTR,                  /*  command                     */
                                                NULL,                                       /*  data                        */
                                                0,                                          /*  dataLen                     */
                                                TRUE,                                       /*  answerRequired              */
                                                _1000_MSEC_ ,                               /*  timeoutValue                */
                                                METER_CONTROL_DEFAULT_NUMBER_OF_RETRIES,    /*  maxNumberOfRetries          */
                                                0 ,                                         /*  stabilizationTimeoutValue   */
                                                _ADDDELMETER_END_STATE);                    /*  nextState                   */
}

void vfnAddDelMeterEndState(void){
    
    if(MeterControl_GetStabilizationTimeout() == METER_TIMEOUT_INITIALIZED)
        return;
    
    API_MeterTable_SetCommandMeterBusy(FALSE);
    
    if ( API_MeterControl_QueueInfoCheck() == METER_CONTROL_NO_ERROR_CODE)
        return;    
        
    vfnEventClear(ADD_DEL_MTR_EVENT);
    vfnEventDisable(ADD_DEL_MTR_EVENT);
    
}

//******************************************************************************
//                  LOCAL METER CONTROL STATE MACHINE
//******************************************************************************