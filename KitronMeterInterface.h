/* 
 * File:   KitronMeterInterface.h
 * Author: fascio
 *
 * Created on 4 de febrero de 2016, 13:51
 */

#ifndef __KITRON_METER_INTERFACE_H__
#define	__KITRON_METER_INTERFACE_H__

//******************************************************************************
// Includes
//******************************************************************************
#include "Utility.h"
#include "MeterCommons.h"
#include "ComSerialInterface.h"

//******************************************************************************
// Defines
//******************************************************************************
#define KITRON_METER_INTERFACE_MODBUS_ID_HEADER_SIZE                        (1)
#define KITRON_METER_INTERFACE_COMMAND_FUNCTION_HEADER_SIZE                 (1)
#define KITRON_METER_INTERFACE_REGISTER_ADDRESS_HEADER_SIZE                 (2)
#define KITRON_METER_INTERFACE_REGISTER_AMOUNT_HEADER_SIZE                  (2)
#define KITRON_METER_INTERFACE_CRC_HEADER_SIZE                              (2)

#define KITRON_METER_INTERFACE_READ_COMMAND_FUNCTION                      (0x03)
#define KITRON_METER_INTERFACE_CRC_BASE                                   (0xFFFF)

#define KITRON_METER_INTERFACE_MAX_FRAME_BUFFER_SIZE                        (50)

//******************************************************************************
// KITRON METER INTERFACE REGISTER ADDRESSES
//******************************************************************************

#define KITRON_METER_INTERFACE_TOTAL_IMPORT_ACTIVE_ENERGY_REGISTER_ADDRESS          (0x4010) //kWh
#define KITRON_METER_INTERFACE_TOTAL_IMPORT_ACTIVE_ENERGY_REGISTER_ADDRESS_SIZE         (1)

#define KITRON_METER_INTERFACE_TOTAL_IMPORT_REACTIVE_ENERGY_REGISTER_ADDRESS        (0x4110) //kvarh
#define KITRON_METER_INTERFACE_TOTAL_IMPORT_REACTIVE_ENERGY_REGISTER_ADDRESS_SIZE       (1)

#define KITRON_METER_INTERFACE_TOTAL_EXPORT_ACTIVE_ENERGY_REGISTER_ADDRESS          (0x4020) //kWh
#define KITRON_METER_INTERFACE_TOTAL_EXPORT_ACTIVE_ENERGY_REGISTER_ADDRESS_SIZE         (1) 

#define KITRON_METER_INTERFACE_TOTAL_EXPORT_REACTIVE_ENERGY_REGISTER_ADDRESS        (0x4120) //kvarh
#define KITRON_METER_INTERFACE_TOTAL_EXPORT_REACTIVE_ENERGY_REGISTER_ADDRESS_SIZE       (1) 

#define KITRON_METER_INTERFACE_VOLTAGE_PHASE_A_REGISTER_ADDRESS         (0x6611)
#define KITRON_METER_INTERFACE_VOLTAGE_PHASE_A_REGISTER_ADDRESS_SIZE         (1)

#define KITRON_METER_INTERFACE_VOLTAGE_PHASE_B_REGISTER_ADDRESS         (0x6612)
#define KITRON_METER_INTERFACE_VOLTAGE_PHASE_B_REGISTER_ADDRESS_SIZE         (1)

#define KITRON_METER_INTERFACE_VOLTAGE_PHASE_C_REGISTER_ADDRESS         (0x6613)
#define KITRON_METER_INTERFACE_VOLTAGE_PHASE_C_REGISTER_ADDRESS_SIZE         (1)

#define KITRON_METER_INTERFACE_ALL_VOLTAGE_PHASE_REGISTER_ADDRESS           KITRON_METER_INTERFACE_VOLTAGE_PHASE_A_REGISTER_ADDRESS
#define KITRON_METER_INTERFACE_ALL_VOLTAGE_PHASE_REGISTER_ADDRESS_SIZE     (KITRON_METER_INTERFACE_VOLTAGE_PHASE_A_REGISTER_ADDRESS_SIZE +\
                                                                            KITRON_METER_INTERFACE_VOLTAGE_PHASE_B_REGISTER_ADDRESS_SIZE +\
                                                                            KITRON_METER_INTERFACE_VOLTAGE_PHASE_C_REGISTER_ADDRESS_SIZE)

#define KITRON_METER_INTERFACE_CURRENT_PHASE_A_REGISTER_ADDRESS         (0x6621)
#define KITRON_METER_INTERFACE_CURRENT_PHASE_A_REGISTER_ADDRESS_SIZE         (1)

#define KITRON_METER_INTERFACE_CURRENT_PHASE_B_REGISTER_ADDRESS         (0x6622)
#define KITRON_METER_INTERFACE_CURRENT_PHASE_B_REGISTER_ADDRESS_SIZE         (1)

#define KITRON_METER_INTERFACE_CURRENT_PHASE_C_REGISTER_ADDRESS         (0x6623)
#define KITRON_METER_INTERFACE_CURRENT_PHASE_C_REGISTER_ADDRESS_SIZE         (1)

#define KITRON_METER_INTERFACE_ALL_CURRENT_PHASE_REGISTER_ADDRESS           KITRON_METER_INTERFACE_CURRENT_PHASE_A_REGISTER_ADDRESS
#define KITRON_METER_INTERFACE_ALL_CURRENT_PHASE_REGISTER_ADDRESS_SIZE     (KITRON_METER_INTERFACE_CURRENT_PHASE_A_REGISTER_ADDRESS_SIZE +\
                                                                            KITRON_METER_INTERFACE_CURRENT_PHASE_B_REGISTER_ADDRESS_SIZE +\
                                                                            KITRON_METER_INTERFACE_CURRENT_PHASE_C_REGISTER_ADDRESS_SIZE)

#define KITRON_METER_INTERFACE_TOTAL_INSTANTANEOUS_ACTIVE_POWER_REGISTER_ADDRESS        (0x6630)
#define KITRON_METER_INTERFACE_TOTAL_INSTANTANEOUS_ACTIVE_POWER_REGISTER_ADDRESS_SIZE       (1)

#define KITRON_METER_INTERFACE_ACTIVE_POWER_PHASE_A_REGISTER_ADDRESS    (0x6631)
#define KITRON_METER_INTERFACE_ACTIVE_POWER_PHASE_A_REGISTER_ADDRESS_SIZE    (1)

#define KITRON_METER_INTERFACE_ACTIVE_POWER_PHASE_B_REGISTER_ADDRESS    (0x6632)
#define KITRON_METER_INTERFACE_ACTIVE_POWER_PHASE_B_REGISTER_ADDRESS_SIZE    (1)

#define KITRON_METER_INTERFACE_ACTIVE_POWER_PHASE_C_REGISTER_ADDRESS    (0x6633)
#define KITRON_METER_INTERFACE_ACTIVE_POWER_PHASE_C_REGISTER_ADDRESS_SIZE    (1)

/*#define KITRON_METER_INTERFACE_ALL_ACTIVE_POWER_PHASE_REGISTER_ADDRESS      KITRON_METER_INTERFACE_TOTAL_INSTANTANEOUS_ACTIVE_POWER_REGISTER_ADDRESS
#define KITRON_METER_INTERFACE_ALL_ACTIVE_POWER_PHASE_REGISTER_ADDRESS_SIZE (KITRON_METER_INTERFACE_TOTAL_INSTANTANEOUS_ACTIVE_POWER_REGISTER_ADDRESS_SIZE +\
                                                                            KITRON_METER_INTERFACE_ACTIVE_POWER_PHASE_A_REGISTER_ADDRESS_SIZE +\
                                                                            KITRON_METER_INTERFACE_ACTIVE_POWER_PHASE_B_REGISTER_ADDRESS_SIZE +\
                                                                            KITRON_METER_INTERFACE_ACTIVE_POWER_PHASE_C_REGISTER_ADDRESS_SIZE)*/

#define KITRON_METER_INTERFACE_TOTAL_POWER_FACTOR_REGISTER_ADDRESS      (0x6650)
#define KITRON_METER_INTERFACE_TOTAL_POWER_FACTOR_REGISTER_ADDRESS_SIZE      (1)

#define KITRON_METER_INTERFACE_POWER_FACTOR_PHASE_A_REGISTER_ADDRESS    (0x6651)
#define KITRON_METER_INTERFACE_POWER_FACTOR_PHASE_A_REGISTER_ADDRESS_SIZE    (1)

#define KITRON_METER_INTERFACE_POWER_FACTOR_PHASE_B_REGISTER_ADDRESS    (0x6652)
#define KITRON_METER_INTERFACE_POWER_FACTOR_PHASE_B_REGISTER_ADDRESS_SIZE    (1)

#define KITRON_METER_INTERFACE_POWER_FACTOR_PHASE_C_REGISTER_ADDRESS    (0x6653)
#define KITRON_METER_INTERFACE_POWER_FACTOR_PHASE_C_REGISTER_ADDRESS_SIZE    (1)

/*#define KITRON_METER_INTERFACE_ALL_POWER_FACTOR_PHASE_REGISTER_ADDRESS      KITRON_METER_INTERFACE_TOTAL_POWER_FACTOR_REGISTER_ADDRESS
#define KITRON_METER_INTERFACE_ALL_POWER_FACTOR_PHASE_REGISTER_ADDRESS_SIZE (KITRON_METER_INTERFACE_TOTAL_POWER_FACTOR_REGISTER_ADDRESS_SIZE +\
                                                                            KITRON_METER_INTERFACE_POWER_FACTOR_PHASE_A_REGISTER_ADDRESS_SIZE +\
                                                                            KITRON_METER_INTERFACE_POWER_FACTOR_PHASE_B_REGISTER_ADDRESS_SIZE +\
                                                                            KITRON_METER_INTERFACE_POWER_FACTOR_PHASE_C_REGISTER_ADDRESS_SIZE)*/

#define KITRON_METER_INTERFACE_TOTAL_INSTANTANEOUS_APPARENT_POWER_REGISTER_ADDRESS    (0x6660)
#define KITRON_METER_INTERFACE_TOTAL_INSTANTANEOUS_APPARENT_POWER_REGISTER_ADDRESS_SIZE    (1)

#define KITRON_METER_INTERFACE_APPARENT_POWER_PHASE_A_REGISTER_ADDRESS      (0x6661)
#define KITRON_METER_INTERFACE_APPARENT_POWER_PHASE_A_REGISTER_ADDRESS_SIZE     (1)

#define KITRON_METER_INTERFACE_APPARENT_POWER_PHASE_B_REGISTER_ADDRESS      (0x6662)
#define KITRON_METER_INTERFACE_APPARENT_POWER_PHASE_B_REGISTER_ADDRESS_SIZE     (1)

#define KITRON_METER_INTERFACE_APPARENT_POWER_PHASE_C_REGISTER_ADDRESS      (0x6663)
#define KITRON_METER_INTERFACE_APPARENT_POWER_PHASE_C_REGISTER_ADDRESS_SIZE     (1)

/*#define KITRON_METER_INTERFACE_ALL_APPARENT_POWER_PHASE_REGISTER_ADDRESS      KITRON_METER_INTERFACE_TOTAL_INSTANTANEOUS_APPARENT_POWER_REGISTER_ADDRESS
#define KITRON_METER_INTERFACE_ALL_APPARENT_POWER_PHASE_REGISTER_ADDRESS_SIZE (KITRON_METER_INTERFACE_TOTAL_INSTANTANEOUS_APPARENT_POWER_REGISTER_ADDRESS_SIZE +\
                                                                               KITRON_METER_INTERFACE_APPARENT_POWER_PHASE_A_REGISTER_ADDRESS_SIZE +\
                                                                               KITRON_METER_INTERFACE_APPARENT_POWER_PHASE_B_REGISTER_ADDRESS_SIZE +\
                                                                               KITRON_METER_INTERFACE_APPARENT_POWER_PHASE_C_REGISTER_ADDRESS_SIZE)*/
//******************************************************************************
// G155 Meter Interface API
//******************************************************************************
BOOL KitronMeterInterface_BuildFrame(   BYTE modbusId,                                        
                                        BYTE commandFunction,
                                        WORD registerAddress, 
                                        WORD registerAmount, 
                                        BYTE * data, 
                                        BYTE dataLen, 
                                        BYTE * frame, 
                                        BYTE * frameLen);

void KitronMeterInterface_SendFrame( BYTE modbusId,                                     
                                     BYTE commandFunction,
                                     WORD registerAddress, 
                                     WORD registerAmount, 
                                     BYTE * data, 
                                     BYTE dataLen );

void KitronMeterInterface_ReadImportedActiveEnergyData(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen);
void KitronMeterInterface_ReadExportedActiveEnergyData(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen);
void KitronMeterInterface_ReadImportedReactiveEnergyData(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen);
void KitronMeterInterface_ReadExportedReactiveEnergyData(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen);

void KitronMeterInterface_ReadAllVoltageData(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen);
void KitronMeterInterface_ReadAllCurrentData(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen);
void KitronMeterInterface_ReadPowerFactorData(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen);
void KitronMeterInterface_ReadActivePowerData(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen);

//void KitronMeterInterface_ReadAllApparentPowerData(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen);

extern COMMAND_ID_FUNCTION KitronMeterInterface_CommandIdFunctionList [];



#endif	/* __KITRON_METER_INTERFACE_H__ */

