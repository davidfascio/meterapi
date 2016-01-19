/* 
 * File:   main.c
 * Author: fascio
 *
 * Created on 21 de agosto de 2015, 03:45 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "SystemEvents.h"
#include "EventsEngine.h"
#include "G155MeterInterface.h"
#include "Meters_Table.h"
#include "ComSerialInterface.h"


/*
 * 
 */
void vfnToogling (void);
void vfnGO_TO_READ_MTR_PeriodTask(void);
void vfnTime_Out_Meter_Response_OneShot(void);
void vfnTime_Out_Meter_Stabilize_OneShot(void);
void vfnUART_Char_Received_OneShot(void);

const sEvent taEvents[] =
{
    vfnTimeBasedEventsEngine,
    NULL, //vfnMainTask,
    NULL, //vfnI2CDriver,
    NULL, //vfnIIC_MEM24_1025Driver,
    vfnAddDelMeterDriver,
    NULL, //vfnBackUpDriver,
    NULL //vfnRLYBackUpStartDriver,
    #if IT_HAS_SHELL
    Shell_Task,
    vfn_Zigbee_Network_Connection_Check,
    #endif
};

const sPeriodicTimers taPeriodicTimers[] =
{
    {vfnToogling, _500_MSEC_ },
    //{vfnLED_TOGGLE_NWK_ON_PeriodTask,_500_MSEC_},
    {vfnGO_TO_READ_MTR_PeriodTask,_1000_MSEC_ },
    {NULL,_1000_MSEC_ }
};

const sOneShotTimer taOneShotTimers[]=
{
    vfnUART_Char_Received_OneShot,
    NULL, //SE_SendNLMELeaveRequest,
    NULL, //vfnMETER_TO_ASSOC_OneShot,
    vfnTime_Out_Meter_Response_OneShot,
    vfnTime_Out_Meter_Stabilize_OneShot,
    NULL, //vfnTime_Out_IIC_Page_OneShot,
    NULL, //vfnSend_Annunce_OneShot,
    NULL, //vfnSendReqTimeSyncOneShot,
    NULL, //vfnSendReportAlarmOneShot,
    NULL, //vfnRepeatAlarmOneShot,
    NULL, //vfnLED_TOGGLE_NWK_ON_OneShot,
    NULL, //vfnSend_Reading_OneShot,
    NULL, //vfnSend_Single_Reading_OneShot,
    NULL, //vfnButtonAnncePressed,
    NULL, //vfnPermitModeOneShot,
    NULL, //vfnDelAllDataOneShot,
    NULL, //vfnSendBaptismOneShot,
    NULL, //vfnSendBaptism2OneShot,
    NULL, //vfnSendIPD_Message_OneShot,
    NULL, //vfnTimeoutRLYOneShot,
    NULL, //vfnSendFragANSIOneShot,
    NULL, //vfnSendFragToIPDOneShot,
    NULL, //vfnSendFragToCoorOneShot,
    NULL, //vfnTimeoutAssociateOneShot,
    NULL, //vfnContinueReadingPLCOneShot,
    NULL, //vfnFragdataBootTimeoutOneShot,
    NULL, //vfnCheckResetOneShot,
    NULL, //vfnUART_Char_Testing_Received_OneShot,
    NULL, //vfnTime_Out_UART_Testing_Response_OneShot,
    NULL //vfnTime_Out_Zigbee_Network_Connection_OneShot
};

void vfnTime_Out_Meter_Stabilize_OneShot(void){
    
    MeterControl_ExpireStabilizationTimeout();
}

void vfnTime_Out_Meter_Response_OneShot(void){
    
    MeterControl_ExpireResponseTimeout();
    // For Debug proporse
    //MeterControl_SetDataAvailable(TRUE);    
}

void vfnUART_Char_Received_OneShot(void){
    
    MeterTable_ReceiveHandler();
}

void vfnTimer(void){
    
    //Sleep(1);
    usleep(100);
    /*clock_t now, then;
    long pause = CLOCKS_PER_SEC /1000;
    
    now = then = clock();
    
    while((now - then) < pause)
        now = clock();
    */
    FAST_EVENT_SET(TB_EVENT);
}

extern Meters_Table            Meters_Table1;
extern BYTE macLongAddrByteInverse[MAC_SIZE];
extern BYTE macLongAddrByte[MAC_SIZE];

int main(int argc, char** argv) {
    int check_flag;
    WORD  handler;
    
    METER_DESCRIPTOR meterDescriptor;
    BYTE commandCallback;
    
    BYTE delete_meter[] = {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x4E, 0x4B, 0x31, 0x31, 0x35, 0x31, 0x31, 0x30, 0x35, 0x36, 0x66, 0xCC, 0x6C};
    BYTE request_data[] = {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x4E, 0x4B, 0x31, 0x31, 0x35, 0x31, 0x31, 0x30, 0x35, 0x36, 0x81, 0x86, 0x2C};   
    BYTE reply_data[] = {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x4E, 0x4B, 0x31, 0x31, 0x35, 0x31, 0x31, 0x30, 0x35, 0x36, 0x00, 0x90, 0x03, 0xDB, 0x65};
    
    BYTE meter_adding_answer[] = {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x4E, 0x4B, 0x31, 0x31, 0x35, 0x31, 0x31, 0x30, 0x35, 0x36, 0x81, 0x86, 0x2C};
    BYTE meter_deleting_answer[] = {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x4E, 0x4B, 0x31, 0x31, 0x35, 0x31, 0x31, 0x30, 0x35, 0x36, 0x66, 0xCC, 0x6C};
    BYTE meter_password_answer[] = {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x4E, 0x4B, 0x31, 0x31, 0x35, 0x31, 0x31, 0x30, 0x35, 0x36, 0x00, 0x10, 0x00, 0x17, 0x00, 0x02, 0xC3, 0x00, 0x00, 0x01, 0x23, 0x3C};
    BYTE meter_disconnect_answer[] = {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x4E, 0x4B, 0x31, 0x31, 0x35, 0x31, 0x31, 0x30, 0x35, 0x36, 0x00, 0x10, 0x00, 0x23, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x64, 0xB4};
    BYTE meter_metering_answer[] = {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x4E, 0x4B, 0x31, 0x31, 0x35, 0x31, 0x31, 
                                    0x30, 0x35, 0x36, 0x00, 0x03, 0x98, 0x02, 0x71, 0xFF, 0xFF, 0x0C, 0x42, 0xAA, 
                                    0x1E, 0x00, 0xAE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x07, 
                                    0x2E, 0x00, 0xC0, 0x00, 0x42, 0xB7, 0xE8, 0x80, 0x41, 0xF4, 0x6A, 0x60, 0x42, 
                                    0xDB, 0x8B, 0x60, 0x3E, 0x65, 0x20, 0x00, 0x1E, 0x2B, 0xD9, 0x2E, 0x1E, 0x2B, 
                                    0x84, 0xCE, 0x42, 0x70, 0x0F, 0x01, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 
                                    0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFE, 0xFF, 0x3F, 0x80, 0x00, 0x00, 0xFF,
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x43, 0x92, 0x83, 0xE0, 0x00, 0x01,
                                    0x53, 0xD8, 0x42, 0xF4, 0x6A, 0x60, 0x1E, 0x22, 0xFD, 0x1C, 0x43, 0xDB, 0x8B,
                                    0x60, 0x00, 0x01, 0x52, 0xAC, 0x3F, 0x65, 0x20, 0x00, 0x1E, 0x22, 0xFD, 0x1C,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42,
                                    0xFC, 0x91, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x41, 0xF4,
                                    0x41, 0x00, 0x61, 0xD9};
    
    inverted_memcpy(macLongAddrByte, macLongAddrByteInverse, MAC_SIZE);
    
    /*BYTE modbusId,
                                    WORD commandFunction,
                                    BYTE registerAddress, 
                                    BYTE registerAmount, 
                                    BYTE * data, 
                                    BYTE dataLen, 
                                    BYTE * frame, 
                                    BYTE * frameLen
     */
    
    /*
    // PSW State
    BYTE modbusId = 0x02;
    WORD commandFunction = 0x06;
    BYTE registerAddress = 0x1c;
    BYTE registerAmount = 0x00;
    BYTE data[] = {KEY1_G155, KEY2_G155};
    BYTE dataLen = sizeof(data);
    */
    /*
    // CMD State
    BYTE modbusId = 0x01;
    WORD commandFunction = 0x06;
    BYTE registerAddress = 0x0c;
    BYTE registerAmount = 0x00;
    BYTE data[] = {0x00, 0x01};
    BYTE dataLen = sizeof(data);
    
    BYTE frame[20];
    BYTE frameLen;
    
    BYTE index;
    
    G155MeterInterface_BuildFrame(  modbusId,
                                    commandFunction, 
                                    registerAddress, 
                                    registerAmount, 
                                    data,
                                    dataLen,
                                    frame, 
                                    &frameLen);
    
    for (index = 0; index < frameLen; index++)
        printf("%02X ", frame[index]);
      */
    BYTE serialNumberDemo[]= "000000NK11511056";
    
    printf("Meter Interface Demo:\n");    
    vfnEventsEngineInit();
    vfnEventEnable(TB_EVENT);                   //inicializa la base de tiempo
    ComSerialInterface_Init();
    
    // Fill
    memset(&Meters_Table1, 0xFF, sizeof(Meters_Table));
    /*Meters_Table1.Meter_DEV[0].Type = MONO_TYPE;
    Meters_Table1.Meter_DEV[0].Signature = 'p' + 5;
    memcpy(Meters_Table1.Meter_DEV[0].Serial_Num,serialNumberDemo,Lenght_Meter_ID);    */
    vfnPeriodicTimerEnable(LED_TOGGLE_MAIN_PERTASK);
    vfnPeriodicTimerEnable(GO_TO_READ_MTR_PERTASK);    //Crea la rutina de leer medidores
    
    //MeterControl_Setup( METER_CONTROL_NO_METER_ID, G155_TYPE, REQUEST_SERIAL_NUMBER_MTR, 0);    
    //handler = API_SCORPIO_Recieve_handler(reply_data, sizeof(reply_data), &meterDescriptor, &commandCallback );
    //API_SCORPIO_Recieve_handler(meter_metering_answer, sizeof(meter_metering_answer), &meterDescriptor, &commandCallback );
    //API_MeterTable_ExcecuteCommandInvoke(&meterDescriptor, commandCallback);
    
    /*if(handler == HANDLER_SCORPIO_NO_ERROR){
        check_flag = 1;
    }
    else{
        check_flag = 0; 
    }*/
    
    while(TRUE){
        
        vfnEventsEngine();
        vfnTimer();        
        ComSerialInterface_Check();
        
        //API_MeterTable_ExcecuteNewBaptismProcess();
    }
    
    return (EXIT_SUCCESS);
}

BYTE counter;
//BYTE number []= {1,1,1,1,1};
BYTE command[] = {Dis_MTR,Con_MTR,Con_MTR, Dis_MTR};

BYTE AppModbusId = 1;
void vfnGO_TO_READ_MTR_PeriodTask(void){
 
    BYTE error_code;
    
    error_code = API_MeterTable_SendCommand(counter%2, command[counter]);
    
    if (error_code != METER_TABLE_METER_NO_ERROR_CODE)
        printf ("Meter Error Code: %d\n", error_code);
    
    counter++;
    if(counter == 4)
        counter = 0;
    
    API_MeterTable_ExcecuteBaptismProccess();
    //API_MeterTable_ExcecuteBaptismProccess();
    //G155MeterInterface_RequestSerialNumber(AppModbusId++, NULL, 0, NULL, 0);
    //(void)CMD_To_Scorpio ((BYTE)NULL,READ_MODE); 
}

void vfnToogling (void)
{
    printf("TICK\n");
}