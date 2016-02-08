/* 
 * File:   KitronMeterHandler.h
 * Author: fascio
 *
 * Created on 5 de febrero de 2016, 11:07
 */

#ifndef __KITRON_METER_HANDLER_H__
#define	__KITRON_METER_HANDLER_H__

//******************************************************************************
// Includes
//******************************************************************************

#include "GenericTypeDefs.h"
#include "MeterCommons.h"

#define KITRON_METER_TYPE                                               (0x85)
#define KITRON_METER_HANDLER_CRC_BASE                                   (0xFFFF)
#define KITRON_METER_HANDLER_READ_COMMAND_FUNCTION                      (0x03)
#define KITRON_MAX_DATA_SIZE                                            (50)

#define KITRON_METER_HANDLER_MODBUS_ID_HEADER_SIZE                           (1)
#define KITRON_METER_HANDLER_COMMAND_FUNCTION_HEADER_SIZE                    (1)
#define KITRON_METER_HANDLER_DATA_SIZE_HEADER_SIZE                           (1)
#define KITRON_METER_HANDLER_CRC_HEADER_SIZE                                 (2)
#define KITRON_METER_HANDLER_FILEDS_HEADER_SIZE                         (KITRON_METER_HANDLER_MODBUS_ID_HEADER_SIZE +\
                                                                        KITRON_METER_HANDLER_COMMAND_FUNCTION_HEADER_SIZE +\
                                                                        KITRON_METER_HANDLER_DATA_SIZE_HEADER_SIZE +\
                                                                        KITRON_METER_HANDLER_CRC_HEADER_SIZE)



#define KITRON_METER_HANDLER_CRC_OK                                          (0)
#define KITRON_METER_HANDLER_INVALID_MODBUS_ID                               (0)

#define KITRON_METER_HANDLER_NO_ERROR_CODE                                  (0)
#define KITRON_METER_HANDLER_WRONG_CRC_ERROR_CODE                           (-1)
#define KITRON_METER_HANDLER_INVALID_MODBUS_ID_ERROR_CODE                   (-2)
#define KITRON_METER_HANDLER_COMMAND_FUNCTION_NOT_SUPPORTED_ERROR_CODE      (-3)
#define KITRON_METER_HANDLER_WRONG_DATA_SIZE_ERROR_CODE                     (-4)


//******************************************************************************
// Kitron Meter Handler Data types
//******************************************************************************

typedef struct {
    BYTE modbusId;
    BYTE functionCommand;
    BYTE dataSize;
    BYTE data[KITRON_MAX_DATA_SIZE];    
} KITRON_METER_HANDLER, * KITRON_METER_HANDLER_PTR;

typedef struct{
    float VoltagePhaseC;
    float VoltagePhaseB;
    float VoltagePhaseA;
}KitronMeter_Voltage_DataReading, *KitronMeter_Voltage_DataReading_Ptr;

typedef struct{
    float CurrentPhaseC;
    float CurrentPhaseB;
    float CurrentPhaseA;
}KitronMeter_Current_DataReading, *KitronMeter_Current_DataReading_Ptr;

typedef struct{
    float ActivePowerPhaseC;
    float ActivePowerPhaseB;
    float ActivePowerPhaseA;
    float TotalInstantaneousActivePower;
}KitronMeter_ActivePower_DataReading, *KitronMeter_ActivePower_DataReading_Ptr;

typedef struct{
    float PowerFactorPhaseC;
    float PowerFactorPhaseB;
    float PowerFactorPhaseA;
    float TotalPowerFactor;
}KitronMeter_PowerFactor_DataReading, *KitronMeter_PowerFactor_DataReading_Ptr;

typedef struct{
    float ApparentPowerPhaseC;
    float ApparentPowerPhaseB;
    float ApparentPowerPhaseA;
    float TotalInstantaneousApparentPower;
}KitronMeter_ApparentPower_DataReading, * KitronMeter_ApparentPower_DataReading_Ptr;

typedef struct{
    KitronMeter_ApparentPower_DataReading   AppatentPower;
    KitronMeter_PowerFactor_DataReading     PowerFactor;
    KitronMeter_ActivePower_DataReading     ActivePower;
    KitronMeter_Current_DataReading         Current;
    KitronMeter_Voltage_DataReading         Voltage;
}KitronMeter_RealTimeData_DataReading, * KitronMeter_RealTimeData_DataReading_Ptr;

//******************************************************************************
// Kitron Meter Handler API
//******************************************************************************

void KitronMeterHandler_Setup(BYTE modbusId, BYTE functionCommand, BYTE dataSize, BYTE * data);

WORD API_KitronMeterHandler_ReceiveHandler( BYTE * buffer, WORD  buffersize, METER_DESCRIPTOR_PTR meterDescriptor, BYTE * commandCallBack);
WORD API_KitronmeterHandler_ResponseHandler( BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE command, BYTE * response, WORD maxResponseLen, WORD * responseLen);
BYTE API_KitronMeterHandler_GetInvokeFunctionId(BYTE commandId);


#endif	/* __KITRON_METER_HANDLER_H__ */

