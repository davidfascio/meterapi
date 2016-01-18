/* 
 * File:   handler.h
 * Author: Salvador García
 *
 * Created on 27 de noviembre de 2015, 10:56
 */

#ifndef HANDLER_H
#define	HANDLER_H
//******************************************************************************
// General defines
//#ifndef TRUE
//#define TRUE                                                    (1)
//#endif /* TRUE*/
//
//#ifndef FALSE
//#define FALSE                                                   (0)
//#endif /*FALSE*/
//
//typedef unsigned short int      WORD;                           /* 16-bit unsigned */
//typedef unsigned char           BYTE;                           /* 8-bit unsigned  */
//typedef unsigned char           boolean;                        /* TRUE or FALSE*/
//typedef signed char             CHAR;                           /* 8-bit signed  */

#include "GenericTypeDefs.h"
#include "MeterCommons.h"
//******************************************************************************
// API Defines
#define HANDLER_G155_WINDOW                                     (50)
#define G155_MAX_DATA_SIZE                                      (200)
#define HANDLER_G155_CRC_OK                                     (0)
#define HANDLER_G155_SIZE_OK                                    (0)
#define HANDLER_G155_ID_OK                                      (0)
#define HANDLER_G155_IS_A_FLAG                                  (0x06)
#define HANDLER_G155_IS_A_FCN                                   (0x03)
#define HANDLER_G155_IS_A_MULTIPLE_FCN                          (0x10)
#define HANDLER_G155_NOT_A_FLAG                                 (0)
#define HANDLER_G155_NOT_A_FCN                                  (0)
#define HANDLER_G155_NOT_A_MULTIPLE_FCN                         (0)

// Command Id defines
/*#define NO_COMMAND_MTR                                          (0)
#define Dis_MTR                                                 (1)
#define Con_MTR                                                 (2)
#define Res_MTR                                                 (3)
#define READ_MODE                                               (4)
#define PSW_MTR                                                 (5)
#define SEND_MAC_BROADCAST_MTR                                  (6)
#define ASSIGN_MODBUS_ID_MTR                                    (7)
#define REQUEST_SERIAL_NUMBER_MTR                               (8)*/

// Error Codes
#define NO_ERROR_NUMERATION                                     (1)   
#define HANDLER_G155_NO_ERROR                                   (0)
#define HANDLER_G155_CRC_FAILURE_ERROR_CODE                     (-1)
#define HANDLER_G155_SIZE_FAILURE_ERROR_CODE                    (-2)
#define HANDLER_G155_ID_FAILURE_ERROR_CODE                      (-3)
#define HANDLER_G155_FLAG_AND_FUNCTION_FAILURE_ERROR_CODE       (-4)
#define ERROR_NO_MATCH_MODBUS_ID                                (-5)
#define ERROR_NUMERATION                                        (-6)

// Buffer Offset Defines
#define HANDLER_G155_FLAG_BUFFER_OFFSET                         (1)
#define HANDLER_G155_FLAG_ADDR_BUFFER_OFFSET                    (HANDLER_G155_FLAG_BUFFER_OFFSET + 2)
#define HANDLER_G155_FLAG_STATE_BUFFER_OFFSET                   (HANDLER_G155_FLAG_ADDR_BUFFER_OFFSET + 2)
#define HANDLER_G155_FCN_INDEX_OFFSET                           (1)
#define HANDLER_G155_FCN_REGISTER1_INDEX_OFFSET                 (HANDLER_G155_FCN_INDEX_OFFSET + 2) // 3 JUMPS
#define HANDLER_G155_FCN_REGISTER2_INDEX_OFFSET                 (HANDLER_G155_FCN_REGISTER1_INDEX_OFFSET + 2) // 5 JUMPS
#define HANDLER_G155_FCN_SIZE_INDEX_OFSET                       (HANDLER_G155_FCN_REGISTER2_INDEX_OFFSET + 1) // 6 JUMPS
#define HANDLER_G155_FCN_MAC_INDEX_OFSET                        (HANDLER_G155_FCN_SIZE_INDEX_OFSET + 1) // 7 JUMPS
#define HANDLER_G155_SIZE_INDEX_OFFSET                          (HANDLER_G155_FCN_INDEX_OFFSET + 1)
#define HANDLER_G155_CALIBRATION_ADDR_HIG_VALUE_OFFSET          (HANDLER_G155_FLAG_ADDR_BUFFER_OFFSET + 1) //4 JUMPS
#define HANDLER_G155_CALIBRATION_ADDR_LOW_VALUE_OFFSET          (HANDLER_G155_CALIBRATION_ADDR_HIG_VALUE_OFFSET + 1) //5 JUMPS
#define HANDLER_G155_PASSWORD_ADDR_HIGH_VALUE_OFFSET            (HANDLER_G155_FLAG_ADDR_BUFFER_OFFSET + 1) //4 JUMPS
#define HANDLER_G155_PASSWORD_ADDR_LOW_VALUE_OFFSET             (HANDLER_G155_PASSWORD_ADDR_HIGH_VALUE_OFFSET + 1 ) //5 JUMPS

// Function type defines
#define HANDLER_G155_FLAG_ARRAY_DATA_SIZE                       (0x08)
#define HANDLER_G155_FUNCTION_ARRAY_DATA_SIZE                   (0x05)
#define HANDLER_G155_FUNCTION_WRITE_ARRAY_DATA_SIZE             (0x09)
#define HANDLER_G155_FUNCTION_DATA_VALUE                        (0x03)
#define HANDLER_G155_FUNCTION_MULTIPLE_WRITE_DATA_VALUE         (0x10)
#define HANDLER_G155_FLAG_DATA_VALUE                            (0x06)
#define HANDLER_G155_FLAG_ADDRESS_VALUE                         (0x0C)
#define HANDLER_G155_CALIBRATION_ADDRESS_VALUE                  (0x0D)
#define HANDLER_G155_PASSWORD_ADDRESS_VALUE                     (0x1C)
#define HANDLER_G155_MODBUSID_ADDRESS_VALUE                     (0x11)
#define HANDLER_G155_FLAG_RELAY_ON                              (0x00)
#define HANDLER_G155_FLAG_RELAY_OFF                             (0x01)
#define HANDLER_G155_FCN_MAC_DATA_SIZE                           (8)
#define HANDLER_G155_FLAG_DATA_SIZE                              (4)
#define HANDLER_G155_FLAG_NOT_DATA_SIZE                          (0)
#define HANDLER_G155_FCN_NOT_DATA_SIZE                           (0)
#define HANDLER_G155_NOT_DATA_SIZE                               (0)
#define HANDLER_G155_FCN_MTR_SN_DATA_SIZE                       (0x06)
#define HANDLER_G155_FCN_MTR_READ_DATA_SIZE                     (0x10)
#define HANDLER_G155_ACCUMULATED_RESET                          (0x04)
#define HANDLER_G155_KVARHR_PULSE                               (0x02)
#define HANDLER_G155_RESET                                      (0x08)
#define HANDLER_G155_CALIBRATION_ADDR_HIGH_VALUE                (0x42)
#define HANDLER_G155_CALIBRATION_ADDR_LOW_VALUE                 (0x0B)
#define HANDLER_G155_PASSWORD_ADDR_HIGH_VALUE                   (0xC3)
#define HANDLER_G155_PASSWORD_ADDR_RELAY_VALUE                  (0x01)
#define HANDLER_G155_PASSWORD_ADDR_ACCUMULATED_RESET_VALUE      (0x04)
#define HANDLER_G155_PASSWORD_ADDR_PULSE_VALUE                  (0x02)
#define HANDLER_G155_PASSWORD_ADDR_RESET_VALUE                  (0x08)
#define HANDLER_G155_FUNCTION_WRITE_CFE_VALUE                   (0x0E)
#define HANDLER_G155_FUNCTION_WRITE_MAC_VALUE                   (0x12)
#define HANDLER_G155_FUNCTION_WRITE_TIMESTAMP_VALUE             (0x03)
#define HANDLER_G155_FUNCTION_WRITE_NEWREGISTER_VALUE           (0x04)
#define HANDLER_G155_LIMIT_MIN_ID                               (0X00)
#define HANDLER_G155_LIMIT_MAX_ID                               (0XFE)

#define HANDLER_G155_SERIAL_NUMBER_OFFSET                       (10)

//******************************************************************************
// Struct data
typedef struct{
    BYTE id;
    BYTE function;
    BYTE data_size;
    BYTE data[HANDLER_G155_WINDOW];
    WORD crc;
}HANDLER_G155_STRUCT;


typedef struct{
    BYTE modbusId;
    BYTE dataSize;
    BYTE fcn;
    BYTE flag;
    BYTE data[G155_MAX_DATA_SIZE];
} G155_STRUCT, * G155_STRUCT_PTR ; 

//******************************************************************************
// API
WORD API_G155_Recieve_handler( BYTE * buffer, WORD  buffersize, METER_DESCRIPTOR_PTR meterDescriptor, BYTE * commandCallBack);
WORD API_G155_Meter_response_handler( BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE command, BYTE * response, WORD * responseLen);

WORD wfnCRC_CALC_G155(BYTE *ptFRAME, WORD wSizeFrame, WORD wCRCStart);
BYTE Handler_FlagCheck_G155(BYTE * ptbFrame, WORD wSizeFrame, BYTE * flag_state);
BYTE Handler_FcnCheck_G155(BYTE * function_data);
BYTE Handler_SizeCheck_G155(BYTE* size_data_value, WORD wSizeFrame, BYTE* data);
BYTE Handler_IdCheck_G155(BYTE* id_data);

DWORD G155MeterHandler_ParseActiveEnergy(DWORD value);

typedef struct __attribute__((packed,aligned(1))) _Data_Readings_G155
{
    WORD    FLAGS;
    DWORD   ENERGY_REACT_TOTAL_Add;
    WORD    POWER_ACT_SYSTEM_Add;
    WORD    CURRENT_Add;
    WORD    VOLTAGE_Add;
    DWORD   ENERGY_ACT_Add;
}Data_Readings_G155, * Data_Readings_G155_Ptr;

void G155MeterHandler_ParseToDataReading( Data_Readings_Ptr dataReading, Data_Readings_G155_Ptr g155DataReading);
DWORD G155MeterHandler_ParseActiveEnergy(DWORD value);
#endif	/* HANDLER_H */

