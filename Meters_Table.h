/* 
 * File:   Meters_Table.h
 * Author: fascio
 *
 * Created on 21 de agosto de 2015, 03:46 PM
 */

#ifndef __METERS_TABLE_H__
#define	__METERS_TABLE_H__

#include "GenericTypeDefs.h"
#include "MeterCommons.h"

#define NUM_MAX_METERS                                      24


#define METER_TABLE_METER_NO_ERROR_CODE                     (0)
#define METER_TABLE_METER_ID_ERROR_CODE                     (0xFE)
#define METER_TABLE_SERIAL_NUMBER_ERROR_CODE                (-1)
#define METER_TABLE_COULD_NOT_SAVE_SERIAL_NUMBER_IN_NVM     (-2)
#define METER_TABLE_METER_ALREADY_EXISTS                    (-3)
#define METER_TABLE_SERIAL_NUMBER_NOT_MATCHED_ERROR_CODE    (-4)
#define METER_TABLE_MEASUREMENT_ERROR_CODE                  (-7)

#define METER_TABLE_SERIAL_NUMBER_NOT_FOUND                 (0xFF)

#define METER_TABLE_OFFSET_SIGNATURE_MTR                            (1)
#define METER_TABLE_EMPTY_LOCATION                         (0xFF)
#define METER_TABLE_EMPTY_VALUE                             (0xFF)
#define METER_TABLE_NO_METER_ID_FOUND                      (0xFF)


#define METER_TABLE_BROADCAST_METER_ID                      0xFF
#define METER_TABLE_MEASUREMENT_CRC_BASE                    (0xFFFF)

typedef struct __attribute__((packed,aligned(1))) _Meter_Eneri
{
    BYTE Serial_Num[Lenght_Meter_ID];
    BYTE Type;
    LONG_ADDR MACAdd_Display;
    BYTE Signature;
} Meter_Eneri;

typedef struct _Readings_Eneri
{
    Data_Readings Reading;
    BYTE CRC[2];
} Readings_Eneri;

typedef struct _Meters_Table
{
    Meter_Eneri Meter_DEV[NUM_MAX_METERS];
    Readings_Eneri Readings[NUM_MAX_METERS];
} Meters_Table;

BYTE MeterTable_GetMeterTypeByMeterId(BYTE meterId);
BYTE MeterTable_GetModbusIdByMeterId(BYTE meterId);
BYTE MeterTable_GetSerialNumberByMeterId(BYTE meterId, BYTE * serialNumber, BYTE serialNumberLen);
BYTE MeterTable_FindAvailableMeterId(void);
BYTE MeterTable_FindAvailableModbusId(void);
BOOL MeterTable_IsValidSerialNumber(BYTE * serialNumber, WORD serialNumberLen);
BYTE MeterTable_FindMeterBySerialNumber(BYTE * serialNumber, WORD serialNumberLen);
BYTE MeterTable_AddNewMeter(BYTE meterId, BYTE meterType,BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen);
BYTE MeterTable_DeleteMeter(BYTE meterId, BYTE meterType,BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen);
BYTE MeterTable_UpdateMeter(BYTE meterId, BYTE meterType, BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen);
BYTE MeterTable_AddNewMeterBySerialNumber(BYTE meterType, BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen);
BYTE MeterTable_DeleteMeterBySerialNumber(BYTE meterType, BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen);
BYTE MeterTable_SaveMeasurementBySerialNumber(BYTE meterType, BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, Data_Readings_Ptr measurement);

#endif	/* __METERS_TABLE_H__ */

