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

//******************************************************************************
// KITRON METER HANDLER DEFINES
//******************************************************************************

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

//******************************************************************************
// KITRON METER HANDLER ERROR CODES
//******************************************************************************

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
    float TotalImportedActiveEnergy;
} KitronMeter_ImportedActiveEnergy_DataReading, * KitronMeter_ImportedActiveEnergy_DataReading_Ptr;

#define KITRON_METER_HANDLER_IMPORTED_ACTIVE_ENERGY_DATA_SIZE  (sizeof(KitronMeter_ImportedActiveEnergy_DataReading))


typedef struct{
    float TotalExportedActiveEnergy;
} KitronMeter_ExportedActiveEnergy_DataReading, * KitronMeter_ExportedActiveEnergy_DataReading_Ptr;

#define KITRON_METER_HANDLER_EXPORTED_ACTIVE_ENERGY_DATA_SIZE  (sizeof(KitronMeter_ExportedActiveEnergy_DataReading))


typedef struct{
    float TotalImportedReactiveEnergy;
} KitronMeter_ImportedReactiveEnergy_DataReading, * KitronMeter_ImportedReactiveEnergy_DataReading_Ptr;

#define KITRON_METER_HANDLER_IMPORTED_REACTIVE_ENERGY_DATA_SIZE  (sizeof(KitronMeter_ImportedReactiveEnergy_DataReading))


typedef struct{
    float TotalExportedReactiveEnergy;
} KitronMeter_ExportedReactiveEnergy_DataReading, * KitronMeter_ExportedReactiveEnergy_DataReading_Ptr;

#define KITRON_METER_HANDLER_EXPORTED_REACTIVE_ENERGY_DATA_SIZE  (sizeof(KitronMeter_ExportedReactiveEnergy_DataReading))


typedef struct{
    float VoltagePhaseC;
    float VoltagePhaseB;
    float VoltagePhaseA;
}KitronMeter_Voltage_DataReading, * KitronMeter_Voltage_DataReading_Ptr;

#define KITRON_METER_HANDLER_VOLTAGE_DATA_SIZE  (sizeof(KitronMeter_Voltage_DataReading))


typedef struct{
    float CurrentPhaseC;
    float CurrentPhaseB;
    float CurrentPhaseA;
}KitronMeter_Current_DataReading, *KitronMeter_Current_DataReading_Ptr;

#define KITRON_METER_HANDLER_CURRENT_DATA_SIZE  (sizeof(KitronMeter_Current_DataReading))


typedef struct{
    //!float ActivePowerPhaseC;
    //!float ActivePowerPhaseB;
    //!float ActivePowerPhaseA;
    float TotalInstantaneousActivePower;
}KitronMeter_ActivePower_DataReading, *KitronMeter_ActivePower_DataReading_Ptr;

#define KITRON_METER_HANDLER_ACTIVE_POWER_DATA_SIZE  (sizeof(KitronMeter_ActivePower_DataReading))


typedef struct{
    //!float PowerFactorPhaseC;
    //!float PowerFactorPhaseB;
    //!float PowerFactorPhaseA;
    float TotalPowerFactor;
}KitronMeter_PowerFactor_DataReading, *KitronMeter_PowerFactor_DataReading_Ptr;

#define KITRON_METER_HANDLER_POWER_FACTOR_DATA_SIZE  (sizeof(KitronMeter_PowerFactor_DataReading))

/*typedef struct{
    float ApparentPowerPhaseC;
    float ApparentPowerPhaseB;
    float ApparentPowerPhaseA;
    float TotalInstantaneousApparentPower;
}KitronMeter_ApparentPower_DataReading, * KitronMeter_ApparentPower_DataReading_Ptr;

#define KITRON_METER_HANDLER_APPARENT_POWER_DATA_SIZE  (sizeof(KitronMeter_ApparentPower_DataReading))*/

/*typedef struct{
    KitronMeter_ApparentPower_DataReading   AppatentPower;
    KitronMeter_PowerFactor_DataReading     PowerFactor;
    KitronMeter_ActivePower_DataReading     ActivePower;
    KitronMeter_Current_DataReading         Current;
    KitronMeter_Voltage_DataReading         Voltage;
}KitronMeter_RealTimeData_DataReading, * KitronMeter_RealTimeData_DataReading_Ptr;

#define KITRON_METER_HANDLER_REAL_TIME_DATA_SIZE  (sizeof(KitronMeter_RealTimeData_DataReading))*/

//******************************************************************************
// Kitron Meter Handler API
//******************************************************************************

void KitronMeterHandler_Setup(BYTE modbusId, BYTE functionCommand, BYTE dataSize, BYTE * data);

WORD API_KitronMeterHandler_ReceiveHandler( BYTE * buffer, WORD  buffersize, METER_DESCRIPTOR_PTR meterDescriptor, BYTE * commandCallBack);
WORD API_KitronmeterHandler_ResponseHandler( BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE command, BYTE * response, WORD maxResponseLen, WORD * responseLen, BYTE * commandCallBack);
BYTE API_KitronMeterHandler_GetInvokeFunctionId(BYTE commandId);

void KitronMeterHandler_ParseImportedActiveEnergyToDataReading( Data_Readings_Ptr dataReading, BYTE * data, WORD dataLen);
void KitronMeterHandler_ParseExportedActiveEnergyToDataReading( Data_Readings_Ptr dataReading, BYTE * data, WORD dataLen);
void KitronMeterHandler_ParseImportedReactiveEnergyToDataReading( Data_Readings_Ptr dataReading, BYTE * data, WORD dataLen);
void KitronMeterHandler_ParseExportedReactiveEnergyToDataReading( Data_Readings_Ptr dataReading, BYTE * data, WORD dataLen);

void KitronMeterHandler_ParseVoltageToDataReading( Data_Readings_Ptr dataReading, BYTE * data, WORD dataLen);
void KitronMeterHandler_ParseCurrentToDataReading( Data_Readings_Ptr dataReading, BYTE * data, WORD dataLen);
void KitronMeterHandler_ParsePowerFactorToDataReading( Data_Readings_Ptr dataReading, BYTE * data, WORD dataLen);
void KitronMeterHandler_ParseActivePowerToDataReading( Data_Readings_Ptr dataReading, BYTE * data, WORD dataLen);
//!void KitronMeterHandler_ParseApparentPowerToDataReading( Data_Readings_Ptr dataReading, BYTE * data, WORD dataLen);

void KitronMeterHandler_PrintImportedActiveEnergyData(KitronMeter_ImportedActiveEnergy_DataReading_Ptr importedActiveEnergy);
void KitronMeterHandler_PrintExportedActiveEnergyData(KitronMeter_ExportedActiveEnergy_DataReading_Ptr exportedActiveEnergy);
void KitronMeterHandler_PrintImportedReactiveEnergyData(KitronMeter_ImportedReactiveEnergy_DataReading_Ptr importedReactiveEnergy);
void KitronMeterHandler_PrintExportedReactiveEnergyData(KitronMeter_ExportedReactiveEnergy_DataReading_Ptr exportedReactiveEnergy);

void KitronMeterHandler_PrintVoltageData(KitronMeter_Voltage_DataReading_Ptr voltage);
void KitronMeterHandler_PrintCurrentData(KitronMeter_Current_DataReading_Ptr current);
void KitronMeterHandler_PrintActivePowerData(KitronMeter_ActivePower_DataReading_Ptr activePower);
void KitronMeterHandler_PrintPowerFactorData(KitronMeter_PowerFactor_DataReading_Ptr powerFactor);
//!void KitronMeterHandler_PrintApparentPowerData(KitronMeter_ApparentPower_DataReading_Ptr apparentPower);

#endif	/* __KITRON_METER_HANDLER_H__ */

