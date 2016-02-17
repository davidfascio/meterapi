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

#define KITRON_METER_INTERFACE_OBIS_SERIAL_NUMBER_OFFSET                    (4)
#define KITRON_METER_INTERFACE_MAX_DATA_SET_BUFFER_SIZE                     (50)
#define KITRON_METER_INTERFACE_MAX_FRAME_BUFFER_SIZE                       ((50) +\
                                                                            KITRON_METER_INTERFACE_MAX_DATA_SET_BUFFER_SIZE )

//******************************************************************************
//                      O B I S  P R O T O C O L
//******************************************************************************
#define KITRON_METER_INTERFACE_OBIS_START_CHARACTER                         (0x2F)  //   '/'
#define KITRON_METER_INTERFACE_OBIS_START_CHARACTER_SIZE                    (1)     //   '/'

#define KITRON_METER_INTERFACE_OBIS_REQUEST_COMMAND_CHARACTER               (0x3F)  //   '?'
#define KITRON_METER_INTERFACE_OBIS_REQUEST_COMMAND_CHARACTER_SIZE          (1)     //   '?'

#define KITRON_METER_INTERFACE_OBIS_DEVICE_ADDRESS_MAX_SIZE                 (32)
#define KITRON_METER_INTERFACE_OBIS_DATA_SET_MAX_SIZE                       (32)

#define KITRON_METER_INTERFACE_OBIS_END_CHARACTER                           (0x21)  //   '!'
#define KITRON_METER_INTERFACE_OBIS_END_CHARACTER_SIZE                      (1)     //   '!'

#define KITRON_METER_INTERFACE_OBIS_COMPLETION_CHARACTERS                   (0x0D0A) //   'CR' 'LF'
#define KITRON_METER_INTERFACE_OBIS_COMPLETION_CHARACTERS_SIZE              (2)

#define KITRON_METER_INTERFACE_OBIS_ACK_CHARACTER                           (0x06)  // 'ACK'
#define KITRON_METER_INTERFACE_OBIS_ACK_CHARACTER_SIZE                      (1)     // 'ACK'

#define KITRON_METER_INTERFACE_OBIS_NACK_CHARACTER                          (0x15)  // 'NACK'
#define KITRON_METER_INTERFACE_OBIS_NACK_CHARACTER_SIZE                     (1)     // 'NACK'

#define KITRON_METER_INTERFACE_OBIS_NULL_CHARACTER                          (0x00)  // 'NULL'

#define KITRON_METER_INTERFACE_OBIS_START_OF_HEADER_CHARACTER               (0x01)  // 'SOH'
#define KITRON_METER_INTERFACE_OBIS_START_OF_HEADER_CHARACTER_SIZE          (1)     // 'SOH'

#define KITRON_METER_INTERFACE_OBIS_FRAME_START_CHARACTER                   (0x02)  // 'STX'
#define KITRON_METER_INTERFACE_OBIS_FRAME_START_CHARACTER_SIZE              (1)     // 'STX'

#define KITRON_METER_INTERFACE_OBIS_FRAME_END_CHARACTER                     (0x03)  // 'ETX'
#define KITRON_METER_INTERFACE_OBIS_FRAME_END_CHARACTER_SIZE                (1)     // 'ETX'

#define KITRON_METER_INTERFACE_OBIS_END_OF_TEXT_CHARACTER                   (0x04)  // 'EOT'
#define KITRON_METER_INTERFACE_OBIS_END_OF_TEXT_CHARACTER_SIZE              (1)     // 'EOT'

#define KITRON_METER_INTERFACE_OBIS_FRONT_BOUNDARY_CHARACTER                (0x28)  // '('
#define KITRON_METER_INTERFACE_OBIS_FRONT_BOUNDARY_CHARACTER_SIZE           (1)     // '('

#define KITRON_METER_INTERFACE_OBIS_SEPARATOR_CHARACTER                     (0x2A)  // '*'
#define KITRON_METER_INTERFACE_OBIS_SEPARATOR_CHARACTER_SIZE                (1)     // '*'

#define KITRON_METER_INTERFACE_OBIS_REAR_BOUNDARY_CHARACTER                 (0x29)  // ')'
#define KITRON_METER_INTERFACE_OBIS_REAR_BOUNDARY_CHARACTER_SIZE            (1)     // ')'


// Protocol mode C and E with baud rate changeover:
// The "acknowledgement/option" is only used in this modes.
#define KITRON_METER_INTERFACE_OBIS_BAUDRATE_IDENTIFICATION_300_CHARACTER   (0x30)  // '0'
#define KITRON_METER_INTERFACE_OBIS_BAUDRATE_IDENTIFICATION_600_CHARACTER   (0x31)  // '1'
#define KITRON_METER_INTERFACE_OBIS_BAUDRATE_IDENTIFICATION_1200_CHARACTER  (0x32)  // '2'
#define KITRON_METER_INTERFACE_OBIS_BAUDRATE_IDENTIFICATION_2400_CHARACTER  (0x33)  // '3'
#define KITRON_METER_INTERFACE_OBIS_BAUDRATE_IDENTIFICATION_4800_CHARACTER  (0x34)  // '4'
#define KITRON_METER_INTERFACE_OBIS_BAUDRATE_IDENTIFICATION_9600_CHARACTER  (0x35)  // '5'
#define KITRON_METER_INTERFACE_OBIS_BAUDRATE_IDENTIFICATION_19200_CHARACTER (0x36)  // '6'

#define KITRON_METER_INTERFACE_OBIS_BAUDRATE_IDENTIFICATION_CHARACTER_SIZE                 (1)     // 

// Protocol Control Characters
#define KITRON_METER_INTERFACE_OBIS_PROTOCOL_CONTROL_NORMAL_PROCEDURE_CHARACTER     (0x30) // '0'
#define KITRON_METER_INTERFACE_OBIS_PROTOCOL_CONTROL_SECONDARY_PROCEDURE_CHARACTER  (0x31) // '1'
#define KITRON_METER_INTERFACE_OBIS_PROTOCOL_CONTROL_HDLC_PROCEDURE_CHARACTER       (0x32) // '2'

#define KITRON_METER_INTERFACE_OBIS_PROTOCOL_CONTROL_CHARACTER_SIZE                 (1)     // 

// Mode Control Characters
#define KITRON_METER_INTERFACE_OBIS_MODE_CONTROL_READ_DATA_CHARACTER        (0x30) // '0'
#define KITRON_METER_INTERFACE_OBIS_MODE_CONTROL_PROGRAMMING_END_CHARACTER  (0x31) // '1'
#define KITRON_METER_INTERFACE_OBIS_MODE_CONTROL_BINARY_MODE_CHARACTER      (0x32) // '2'

#define KITRON_METER_INTERFACE_OBIS_MODE_CONTROL_CHARACTER_SIZE                 (1)     // 

// Command message identifiers
#define KITRON_METER_INTERFACE_OBIS_COMMAND_MESSAGE_PASSWORD_CHARACTER      'P' //
#define KITRON_METER_INTERFACE_OBIS_COMMAND_MESSAGE_WRITE_CHARACTER         'W' //
#define KITRON_METER_INTERFACE_OBIS_COMMAND_MESSAGE_READ_CHARACTER          'R' //
#define KITRON_METER_INTERFACE_OBIS_COMMAND_MESSAGE_EXECUTE_CHARACTER       'E' //
#define KITRON_METER_INTERFACE_OBIS_COMMAND_MESSAGE_EXIT_CHARACTER          'B' //

#define KITRON_METER_INTERFACE_OBIS_COMMAND_MESSAGE_CHARACTER_SIZE          (1) //

// Command type identifiers for Password Command
#define KITRON_METER_INTERFACE_OBIS_COMMAND_TYPE_DATA_IS_OPERAND_FOR_SECURE_ALGORITHM_CHARACTER      (0x30) // '0'
#define KITRON_METER_INTERFACE_OBIS_COMMAND_TYPE_DATA_IS_OPERAND_FOR_COMPARISON_CHARACTER            (0x31) // '1'
#define KITRON_METER_INTERFACE_OBIS_COMMAND_TYPE_DATA_IS_RESULT_OF_SECURE_ALGORITHM_CHARACTER        (0x32) // '2'

#define KITRON_METER_INTERFACE_OBIS_COMMAND_TYPE_CHARACTER_SIZE                                      (1)    //

// Command type identifiers for Write Command
#define KITRON_METER_INTERFACE_OBIS_COMMAND_TYPE_WRITE_ASCII_CODE_DATA_CHARACTER                    (0x31) // '1'

// Command type identifiers for Read Command
#define KITRON_METER_INTERFACE_OBIS_COMMAND_TYPE_READ_ASCII_CODE_DATA_CHARACTER                     (0x31) // '1'

// Command type identifiers for Break Command
#define KITRON_METER_INTERFACE_OBIS_COMMAND_TYPE_COMPLETE_SIGN_OFF_CHARACTER                        (0x30) // '0'
#define KITRON_METER_INTERFACE_OBIS_COMMAND_TYPE_COMPLETE_SIGN_OFF_FOR_BATTERY_CHARACTER            (0x31) // '1'


// SYSTEMATICS FOR ELECTRIC ENERGY

// Medium Group Value
#define KITRON_METER_INTERFACE_OBIS_ABSTRACT_OBJECTS_MEDIUM_CHARACTER                   (0x30)  // '0'
#define KITRON_METER_INTERFACE_OBIS_ELECTRICITY_MEDIUM_CHARACTER                        (0x31)  // '1'
#define KITRON_METER_INTERFACE_OBIS_SEPARATOR_MEDIUM_CHARACTER                          (0x2D)  // '-'

#define KITRON_METER_INTERFACE_OBIS_MEDIUM_CHARACTER_SIZE                               (1)     // (1)

// Channel Group Value
#define KITRON_METER_INTERFACE_OBIS_NO_CHANNEL_SPECIFIED_CHARACTER                      (0x30)  // '0'
#define KITRON_METER_INTERFACE_OBIS_ELECTRICITY_CHANNEL_CHARACTER                       (0x31)  // '1'
#define KITRON_METER_INTERFACE_OBIS_SEPARATOR_CHANNEL_CHARACTER                         (0x3A)  // ':'

#define KITRON_METER_INTERFACE_OBIS_SEPARATOR_CHANNEL_CHARACTER_SIZE                    (1)     // (2)
#define KITRON_METER_INTERFACE_OBIS_CHANNEL_CHARACTER_MAX_SIZE                          (2)     // (2)


// Metering Group Value
#define KITRON_METER_INTERFACE_OBIS_GENERAL_PURPOSE_OBJECTS_METERING_TYPE_CHARACTER     (0x30)  // '0'

#define KITRON_METER_INTERFACE_OBIS_METERING_TYPE_CHARACTER_MAX_SIZE                    (2)     // (2)

// Measuring Variable Group Value
#define KITRON_METER_INTERFACE_OBIS_NO_MEASURING_VARIABLE_SPECIFIED_CHARACTER           (0x30)  // '0'

#define KITRON_METER_INTERFACE_OBIS_MEASURING_VARIABLE_CHARACTER_MAX_SIZE               (2)     // (2)

// Tariff Rates Group Value
#define KITRON_METER_INTERFACE_OBIS_TOTAL_TARIFF_RATE_CHARACTER                         (0x30)  // '0'

#define KITRON_METER_INTERFACE_OBIS_TARIFF_RATES_CHARACTER_MAX_SIZE                     (2)     // (2)

// Billing Periods(Historical Values) Group Value
#define KITRON_METER_INTERFACE_OBIS_HISTORICAL_VALUES_CHARACTER_SIZE                    (2)     // (2)


#define KITRON_METER_INTERFACE_OBIS_DOT_SEPARATOR_CHARACTER                             (0x2E)  // '.'
#define KITRON_METER_INTERFACE_OBIS_DOT_SEPARATOR_CHARACTER_SIZE                        (1)


//******************************************************************************
//                      M O D B U S  P R O T O C O L
//******************************************************************************
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
// KITRON Meter Interface API
//******************************************************************************
BOOL KitronMeterInterface_OBISBuildRequestMessageFrame( BYTE startCharacter, 
                                                        BYTE requestCommand, 
                                                        BYTE * deviceAddress,
                                                        WORD deviceAddressLen,
                                                        BYTE endCharacter,
                                                        WORD completionCharacter,
                                                        BYTE * frame,
                                                        WORD * frameLen);

void KitronMeterInterface_OBISSendRequestMessageFrame( BYTE startCharacter, 
                                                        BYTE requestCommand, 
                                                        BYTE * deviceAddress,
                                                        WORD deviceAddressLen,
                                                        BYTE endCharacter,
                                                        WORD completionCharacter);

BOOL KitronMeterInterface_OBISBuildOptionSelectMessageFrame(BYTE acknowledgeCharacter,
                                                            BYTE protocolControlCharacter,
                                                            BYTE baudrateIndetification,
                                                            BYTE modeControlCharacter,
                                                            WORD completionCharacter,
                                                            BYTE * frame,
                                                            WORD * frameLen);

void KitronMeterInterface_OBISSendOptionSelectMessageFrame( BYTE acknowledgeCharacter,
                                                            BYTE protocolControlCharacter,
                                                            BYTE baudrateIndetification,
                                                            BYTE modeControlCharacter,
                                                            WORD completionCharacter);

BOOL KitronMeterInterface_OBISBuildCommandMessageFrame( BYTE startOfHeader,
                                                        BYTE commandMessageIdentifier,
                                                        BYTE commandTypeIdentifier,
                                                        BYTE frameStartCharacter,
                                                        BYTE * dataSet,
                                                        WORD dataSetLen,
                                                        BYTE frameEndCharacter,
                                                        BYTE * frame,
                                                        WORD * frameLen);
                                                        //BYTE blockCheckCharacter);

void KitronMeterInterface_OBISSendCommandMessageFrame( BYTE startOfHeader,
                                                        BYTE commandMessageIdentifier,
                                                        BYTE commandTypeIdentifier,
                                                        BYTE frameStartCharacter,
                                                        BYTE * dataSet,
                                                        WORD dataSetLen,
                                                        BYTE frameEndCharacter);

void KitronMeterInterface_OBISSendData(BYTE * frame, WORD frameLen);


BOOL KitronMeterInterface_ModbusBuildFrame(   BYTE modbusId,                                        
                                        BYTE commandFunction,
                                        WORD registerAddress, 
                                        WORD registerAmount, 
                                        BYTE * data, 
                                        BYTE dataLen, 
                                        BYTE * frame, 
                                        BYTE * frameLen);

void KitronMeterInterface_ModbusSendFrame( BYTE modbusId,                                     
                                     BYTE commandFunction,
                                     WORD registerAddress, 
                                     WORD registerAmount, 
                                     BYTE * data, 
                                     BYTE dataLen );

void KitronMeterInterface_ModbusSendData(BYTE * frame, WORD frameLen);

void KitronMeterInterface_OBISRequestMessage(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen);
void KitronMeterInterface_OBISOptionSelectMessage(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen);
void KitronMeterInterface_OBISReadMeterNumberMessage(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen);
void KitronMeterInterface_OBISBreakMessage(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen);

BOOL KitronMeterInterface_OBISGetDataSetMessage(BYTE  medium, 
                                                BYTE * channel, 
                                                WORD  channelLen, 
                                                BYTE * meteringType, 
                                                WORD meteringTypeLen, 
                                                BYTE * measuringVariable, 
                                                WORD measuringVariableLen, 
                                                BYTE * tariffRate,
                                                WORD tariffRateLen,
                                                BYTE * historicalValues, 
                                                WORD historicalValuesLen, 
                                                BYTE * data,
                                                WORD dataLen,
                                                BYTE * dataSet, 
                                                WORD * dataSetLen );


void KitronMeterInterface_OBISSendCommandReadMessage(   BYTE  medium, 
                                                        BYTE * channel, 
                                                        WORD  channelLen, 
                                                        BYTE * meteringType, 
                                                        WORD meteringTypeLen, 
                                                        BYTE * measuringVariable, 
                                                        WORD measuringVariableLen, 
                                                        BYTE * tariffRate,
                                                        WORD tariffRateLen,
                                                        BYTE * historicalValues, 
                                                        WORD historicalValuesLen);

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

