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

#define KITRON_METER_TYPE                                               (0x93)
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

// OBIS
#define KITRON_METER_HANDLER_OBIS_BCC_HEADER_SIZE                            (1)
#define KITRON_METER_HANDLER_OBIS_START_CHARACTER_HEADER_SIZE                (1)
#define KITRON_METER_HANDLER_OBIS_MANUFACTURES_IDENTIFICATION_HEADER_SIZE    (3)
#define KITRON_METER_HANDLER_OBIS_BAUD_RATE_IDENTIFICATION_HEADER_SIZE       (1)
#define KITRON_METER_HANDLER_OBIS_COMMAND_MESSAGE_IDENTIFIER_HEADER_SIZE     (1)
#define KITRON_METER_HANDLER_OBIS_COMMAND_TYPE_IDENTIFIER_HEADER_SIZE        (1)
#define KITRON_METER_HANDLER_OBIS_BOUNDARY_CHARACTER_HEADER_SIZE             (1)


#define KITRON_METER_HANDLER_OBIS_START_CHARACTER                          (0x2F)
#define KITRON_METER_HANDLER_OBIS_START_OF_HEADER_CHARACTER                (0x01)
#define KITRON_METER_HANDLER_OBIS_FRAME_START_CHARACTER                    (0x02)
#define KITRON_METER_HANDLER_OBIS_FIRST_COMPLETION_CHARACTER               (0x0D) // CR

#define KITRON_METER_HANDLER_OBIS_FRONT_BOUNDARY_CHARACTER                (0x28)  // '('
#define KITRON_METER_HANDLER_OBIS_REAR_BOUNDARY_CHARACTER                 (0x29)  // ')'

//******************************************************************************
// KITRON METER HANDLER ERROR CODES
//******************************************************************************

#define KITRON_METER_HANDLER_NO_ERROR_CODE                                  (0)
#define KITRON_METER_HANDLER_WRONG_CRC_ERROR_CODE                           (-1)
#define KITRON_METER_HANDLER_INVALID_MODBUS_ID_ERROR_CODE                   (-2)
#define KITRON_METER_HANDLER_COMMAND_FUNCTION_NOT_SUPPORTED_ERROR_CODE      (-3)
#define KITRON_METER_HANDLER_WRONG_DATA_SIZE_ERROR_CODE                     (-4)

#define KITRON_METER_HANDLER_OBIS_WRONG_BCC_ERROR_CODE                      (-5)
#define KITRON_METER_HANDLER_OBIS_MESSAGE_IS_NOT_SUPPORTED_ERROR_CODE       (-6)
#define KITRON_METER_HANDLER_OBIS_NULL_PTR_ERROR_CODE                       (-7)
#define KITRON_METER_HANDLER_OBIS_IS_NOT_IDENTIFICATION_MESSAGE_ERROR_CODE  (-8)
#define KITRON_METER_HANDLER_OBIS_IS_NOT_COMMAND_MESSAGE_ERROR_CODE         (-9)
#define KITRON_METER_HANDLER_OBIS_IS_NOT_DATA_MESSAGE_ERROR_CODE            (-10)
#define KITRON_METER_HANDLER_OBIS_WRONG_BOUNDARY_CHARACTER_ERROR_SIZE       (-11)

//******************************************************************************
// Kitron Meter Handler Data types
//******************************************************************************
#define KITRON_HANDLER_OBIS_MANUFACTURES_IDENTIFICATION_MAX_SIZE             (3)
#define KITRON_HANDLER_OBIS_IDENTIFICATION_MAX_SIZE                         (16)
#define KITRON_HANDLER_OBIS_DATA_MESSAGE_MAX_SIZE                           (50)

typedef struct {
    BYTE manufacturesIdentification[KITRON_HANDLER_OBIS_MANUFACTURES_IDENTIFICATION_MAX_SIZE];
    WORD manufacturesIdentificationLen;
    BYTE baudrateIdentification;
    BYTE identification[KITRON_HANDLER_OBIS_IDENTIFICATION_MAX_SIZE]; 
    WORD identificationLen;    
} KITRON_METER_HANDLER_OBIS_IDENTIFICATION, * KITRON_METER_HANDLER_OBIS_IDENTIFICATION_PTR;

typedef struct{
    BYTE dataSet[KITRON_HANDLER_OBIS_DATA_MESSAGE_MAX_SIZE];
    WORD dataSetLen;
} KITRON_METER_HANDLER_OBIS_DATA_MESSAGE, * KITRON_METER_HANDLER_OBIS_DATA_MESSAGE_PTR;

typedef struct{
    BYTE commandMessageIdentifier;
    BYTE commandTypeIdentifier;
    KITRON_METER_HANDLER_OBIS_DATA_MESSAGE dataMessage;    
} KITRON_METER_HANDLER_OBIS_COMMAND_MESSAGE, * KITRON_METER_HANDLER_OBIS_COMMAND_MESSAGE_PTR;

typedef struct{
    KITRON_METER_HANDLER_OBIS_IDENTIFICATION identification;    
    KITRON_METER_HANDLER_OBIS_COMMAND_MESSAGE commandMessage;
}KITRON_METER_HANDLER_OBIS, * KITRON_METER_HANDLER_OBIS_PTR;


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

void KitronMeterHandler_ModbusSetup(BYTE modbusId, BYTE functionCommand, BYTE dataSize, BYTE * data);

WORD API_KitronMeterHandler_ReceiveHandler( BYTE * buffer, WORD  buffersize, METER_DESCRIPTOR_PTR meterDescriptor, BYTE * commandCallBack);
WORD API_KitronMeterHandler_ModbusReceiveHandler( BYTE * buffer, WORD  buffersize, METER_DESCRIPTOR_PTR meterDescriptor, BYTE * commandCallBack);

WORD API_KitronMeterHandler_OBISReceiveHandler( BYTE * buffer, WORD  buffersize, METER_DESCRIPTOR_PTR meterDescriptor, BYTE * commandCallBack);
WORD KitronMeterHandler_OBISReceiveIdentificationMessage(BYTE * buffer, WORD buffersize);
WORD KitronMeterHandler_OBISReceiveCommandMessage(BYTE * buffer, WORD buffersize);
WORD KitronMeterHandler_OBISReceiveDataMessage(BYTE * buffer, WORD buffersize);

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

