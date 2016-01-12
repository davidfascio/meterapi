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
    {vfnGO_TO_READ_MTR_PeriodTask,_5000_MSEC_ },
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
int main(int argc, char** argv) {

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
    printf("Meter Interface Demo:\n");    
    vfnEventsEngineInit();
    vfnEventEnable(TB_EVENT);                   //inicializa la base de tiempo
    ComSerialInterface_Init();
    
    // Fill
    Meters_Table1.Meter_DEV[0].Type = G155_TYPE;
    Meters_Table1.Meter_DEV[0].Signature = 'p' + 5;
    memset(Meters_Table1.Meter_DEV[0].Serial_Num,'0',Lenght_Meter_ID);
    
    vfnPeriodicTimerEnable(LED_TOGGLE_MAIN_PERTASK);
    vfnPeriodicTimerEnable(GO_TO_READ_MTR_PERTASK);    //Crea la rutina de leer medidores
    
    //MeterControl_Setup( METER_CONTROL_NO_METER_ID, G155_TYPE, REQUEST_SERIAL_NUMBER_MTR, 0);        
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
BYTE command[] = {Dis_MTR,Con_MTR,READ_MODE,REQUEST_SERIAL_NUMBER_MTR};

BYTE AppModbusId = 1;
void vfnGO_TO_READ_MTR_PeriodTask(void){
 
    API_MeterTable_SendCommand(0, command[counter]);
    
    
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