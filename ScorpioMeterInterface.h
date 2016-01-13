/* 
 * File:   ScorpioMeterInterface.h
 * Author: fascio
 *
 * Created on 12 de enero de 2016, 12:59
 */

#ifndef __SCORPIO_METER_INTERFACE_H__
#define	__SCORPIO_METER_INTERFACE_H__

//******************************************************************************
// Includes
//******************************************************************************
#include "Utility.h"
#include "MeterCommons.h"
#include "ComSerialInterface.h"

//******************************************************************************
// Defines
//******************************************************************************

// Valid meter type
#define MONO_TYPE                                                           (0x81)
#define BI_TYPE                                                             (0x82)
#define TRI_TYPE                                                            (0x83)

#define SCORPIO_METER_INTERFACE_COMMAND_FUNCTION_HEADER_SIZE                (2)
#define SCORPIO_METER_INTERFACE_REGISTER_ADDRESS_HEADER_SIZE                (2)
#define SCORPIO_METER_INTERFACE_REGISTER_AMOUNT_HEADER_SIZE                 (2)
#define SCORPIO_METER_INTERFACE_CRC_HEADER_SIZE                             (2)
#define SCORPIO_METER_INTERFACE_MAX_FRAME_BUFFER_SIZE                       (50)

#define SCORPIO_METER_INTERFACE_WRITE_COMMAND_FUNCTION                      (0x0010)
#define SCORPIO_METER_INTERFACE_READ_COMMAND_FUNCTION                       (0x0003)

#define SCORPIO_METER_INTERFACE_CRC_BASE                                    (0xFFFF)
#define SCORPIO_METER_INTERFACE_LOCAL_BUFFER_SIZE                           (10)

//******************************************************************************
// SCORPIO METER INTERFACE REGISTER ADDRESSES
//******************************************************************************
#define SCORPIO_METER_INTERFACE_LINKER_NUMBER_REGISTER_ADDRESS              (0x0002)
#define SCORPIO_METER_INTERFACE_CUSTOMER_SERIAL_NUMBER_REGISTER_ADDRESS     (0x0004)

#define SCORPIO_METER_INTERFACE_PASSWORD_REGISTER_ADDRESS                   (0x0017)
#define SCORPIO_METER_INTERFACE_PASSWORD_REGISTER_ADDRESS_SIZE                   (2)
#define SCORPIO_METER_INTERFACE_PASSWORD                                    (0xC300)

#define SCORPIO_METER_INTERFACE_PASSWORD_REPLAY                             (0x0001)

//******************************************************************************
// Metering Register Address
//******************************************************************************

#define SCORPIO_METER_INTERFACE_KH_REGISTER_ADDRESS                         (0x0019)
#define SCORPIO_METER_INTERFACE_KH_REGISTER_ADDRESS_SIZE                        (1)

#define SCORPIO_METER_INTERFACE_ID_PROGRAM_REGISTER_ADDRESS                 (0x001A)
#define SCORPIO_METER_INTERFACE_ID_PROGRAM_REGISTER_ADDRESS_SIZE                 (1)

#define SCORPIO_METER_INTERFACE_FW_VERSION_REGISTER_ADDRESS                 (0x001B)
#define SCORPIO_METER_INTERFACE_FW_VERSION_REGISTER_ADDRESS_SIZE                 (1)

#define SCORPIO_METER_INTERFACE_TEST_TIME_REGISTER_ADDRESS                  (0x001C)
#define SCORPIO_METER_INTERFACE_TEST_TIME_REGISTER_ADDRESS_SIZE                  (1)

#define SCORPIO_METER_INTERFACE_REMAINING_TEST_TIME_REGISTER_ADDRESS        (0x001D)
#define SCORPIO_METER_INTERFACE_REMAINING_TEST_TIME_REGISTER_ADDRESS_SIZE        (1)

#define SCORPIO_METER_INTERFACE_RESET_TIME_REGISTER_ADDRESS                 (0x001E)
#define SCORPIO_METER_INTERFACE_RESET_TIME_REGISTER_ADDRESS_SIZE                 (2)

#define SCORPIO_METER_INTERFACE_OFF_TIME_REGISTER_ADDRESS                   (0x0020)
#define SCORPIO_METER_INTERFACE_OFF_TIME_REGISTER_ADDRESS_SIZE                   (2)

#define SCORPIO_METER_INTERFACE_INTERRUPTION_COUNTER_REGISTER_ADDRESS       (0x0022)
#define SCORPIO_METER_INTERFACE_INTERRUPTION_COUNTER_REGISTER_ADDRESS_SIZE       (1)

#define SCORPIO_METER_INTERFACE_SYSTEM_FLAGS_REGISTER_ADDRESS               (0x0023)
#define SCORPIO_METER_INTERFACE_SYSTEM_FLAGS_REGISTER_ADDRESS_SIZE               (2)

#define SCORPIO_METER_INTERFACE_ACTIVE_POWER_REGISTER_ADDRESS               (0x0025)
#define SCORPIO_METER_INTERFACE_ACTIVE_POWER_REGISTER_ADDRESS_SIZE               (2)

#define SCORPIO_METER_INTERFACE_REACTIVE_POWER_REGISTER_ADDRESS             (0x0027)
#define SCORPIO_METER_INTERFACE_REACTIVE_POWER_REGISTER_ADDRESS_SIZE             (2)

#define SCORPIO_METER_INTERFACE_NEG_ACTIVE_POWER_REGISTER_ADDRESS           (0x0029)
#define SCORPIO_METER_INTERFACE_NEG_ACTIVE_POWER_REGISTER_ADDRESS_SIZE           (2)

//******************************************************************************
// Metering Value Defines
//******************************************************************************
#define SCORPIO_METER_INTERFACE_SYSTEM_FLAGS_RELAY_TURN_ON                  (0x00000001)
#define SCORPIO_METER_INTERFACE_SYSTEM_FLAGS_RELAY_TURN_OFF                 (0x00000000)

#define SCORPIO_METER_INTERFACE_METERING_REGISTER_ADDRESS                   SCORPIO_METER_INTERFACE_KH_REGISTER_ADDRESS
#define SCORPIO_METER_INTERFACE_METERING_REGISTER_ADDRESS_SIZE

//******************************************************************************
// Data types
//******************************************************************************

/*
typedef struct{
    
    BYTE serialNumber;
    WORD commandFunction;
    BYTE registerAddress;
    BYTE registerAmount;
    BYTE retryCounter;
    BYTE timeout;
    BYTE answerRequired;
} SCORPIO_METER_INTERFACE, * SCORPIO_METER_INTERFACE_PTR;
*/
//******************************************************************************
// G155 Meter Interface API
//******************************************************************************
BOOL ScorpioMeterInterface_BuildFrame(  BYTE * serialNumber,
                                        WORD serialNumberLen,
                                        WORD commandFunction,
                                        BYTE registerAddress, 
                                        BYTE registerAmount, 
                                        BYTE * data, 
                                        BYTE dataLen, 
                                        BYTE * frame, 
                                        BYTE * frameLen);

void ScorpioMeterInterface_SendFrame(BYTE * serialNumber,
                                     WORD serialNumberLen,
                                     WORD commandFunction,
                                     BYTE registerAddress, 
                                     BYTE registerAmount, 
                                     BYTE * data, 
                                     BYTE dataLen );


void ScorpioMeterInterface_SendPassword(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen);
void ScorpioMeterInterface_Disconnect(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen);
void ScorpioMeterInterface_Connect(BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE * data, WORD dataLen);

WORD API_Scorpio_Meter_response_handler( BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE command, BYTE * response, WORD * responseLen);
WORD API_Scorpio_Recieve_handler( BYTE * buffer, WORD  buffersize, METER_DESCRIPTOR_PTR meterDescriptor, BYTE * commandCallBack);

extern COMMAND_ID_FUNCTION ScorpioMeterInterface_CommandIdFunctionList [];

#endif	/* __SCORPIO_METER_INTERFACE_H__ */

