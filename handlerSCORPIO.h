/* 
 * File:   handlerSCORPIO.h
 * Author: Salvador García
 *
 * Created on 8 de diciembre de 2015, 17:24
 */

#ifndef HANDLERSCORPIO_H
#define	HANDLERSCORPIO_H

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

#include "GenericTypeDefs.h"
#include "MeterCommons.h"

#define MONO_TYPE                                                           (0x81)
#define BI_TYPE                                                             (0x82)
#define TRI_TYPE                                                            (0x83)

#define SCORPIO_METER_TYPE                                                  (MONO_TYPE)

//******************************************************************************
// API Defines
#define HANDLER_SCORPIO_WINDOW                                     (250)
#define HANDLER_SCORPIO_CRC_OK                                     (0)
#define HANDLER_SCORPIO_MAX_SN_SIZE                                (16)
#define HANDLER_SCORPIO_MIN_SN_SIZE                                (0)
#define HANDLER_SCORPIO_SIZE_OK                                    (0)
#define HANDLER_SCORPIO_ID_OK                                      (1)
#define HANDLER_SCORPIO_IS_A_FLAG_PASSWORD                         (0x10)
#define HANDLER_SCORPIO_IS_A_FLAG_SYSTEM                           (0x10)
#define HANDLER_SCORPIO_IS_A_FCN                                   (0x03)
#define HANDLER_SCORPIO_IS_A_PUSHBUTTON_FCN                        (0x81)
#define HANDLER_SCORPIO_NOT_A_FLAG_PASSWORD                        (0)
#define HANDLER_SCORPIO_NOT_A_FLAG_SYSTEM                          (0)
#define HANDLER_SCORPIO_NOT_A_FCN                                  (0)
#define NO_SERIAL_NUMBER_SIZE                                      (0)

// Command Id defines
//#define NO_COMMAND_MTR                                             (0)
//#define Dis_MTR                                                    (1)
//#define Con_MTR                                                    (2)
//#define Res_MTR                                                    (3)
//#define READ_MODE                                                  (4)
//#define PSW_MTR                                                    (5)
//#define SEND_MAC_BROADCAST_MTR                                     (6)
//#define ASSIGN_MODBUS_ID_MTR                                       (7)
//#define REQUEST_SERIAL_NUMBER_MTR                                  (8)

// Error Codes
#define NO_ERROR_NUMERATION                                        (1)
#define HANDLER_SCORPIO_NO_ERROR                                   (0)
#define HANDLER_SCORPIO_CRC_FAILURE_ERROR_CODE                     (-1)
#define HANDLER_SCORPIO_SIZE_FAILURE_ERROR_CODE                    (-2)
#define HANDLER_SCORPIO_ID_FAILURE_ERROR_CODE                      (-3)
#define HANDLER_SCORPIO_FLAG_AND_FUNCTION_FAILURE_ERROR_CODE       (-4)
#define NO_CORRECT_SIZE                                            (-5)
#define ERROR_NO_MATCH_SERIAL_NUMBER                               (-6)
#define ERROR_NO_MATCH_SERIAL_NUMBER_LENGTH                        (-7)
#define HANDLER_SCORPIO_ERROR_NUMERATION                           (-8)

 
// Buffer Offset Defines
#define HANDLER_SCORPIO_FLAG_BUFFER_OFFSET                         (17) //17 JUMPS
#define HANDLER_SCORPIO_FLAG_ADDR_PASSWORD_HIGH_OFFSET             (HANDLER_SCORPIO_FLAG_BUFFER_OFFSET + 2) //19 JUMPS
#define HANDLER_SCORPIO_FLAG_ADDR_PASSWORD_LOW_OFFSET              (HANDLER_SCORPIO_FLAG_ADDR_PASSWORD_HIGH_OFFSET + 2) //21 JUMPS
#define HANDLER_SCORPIO_FLAG_ADDR_HIGH_1_VALUE_OFFSET              (HANDLER_SCORPIO_FLAG_ADDR_PASSWORD_LOW_OFFSET + 1) //22 JUMPS
#define HANDLER_SCORPIO_FLAG_ADDR_HIGH_2_VALUE_OFFSET              (HANDLER_SCORPIO_FLAG_ADDR_HIGH_1_VALUE_OFFSET + 1 ) //23 JUMPS
#define HANDLER_SCORPIO_FLAG_ADDR_LOW_1_VALUE_OFFSET               (HANDLER_SCORPIO_FLAG_ADDR_HIGH_2_VALUE_OFFSET + 1) //24 JUMPS
#define HANDLER_SCORPIO_FLAG_ADDR_LOW_2_VALUE_OFFSET               (HANDLER_SCORPIO_FLAG_ADDR_LOW_1_VALUE_OFFSET + 1) //25 JUMPS


#define HANDLER_SCORPIO_FLAG_ADDR_SYSTEM_OFFSET                    (HANDLER_SCORPIO_FLAG_ADDR_PASSWORD_HIGH_OFFSET + 2) //21 JUMPS
#define HANDLER_SCORPIO_FLAG_DATA_VALUE_HIGH_1_OFFSET              (HANDLER_SCORPIO_FLAG_ADDR_SYSTEM_OFFSET + 1) //22 JUMPS
#define HANDLER_SCORPIO_FLAG_DATA_VALUE_HIGH_2_OFFSET              (HANDLER_SCORPIO_FLAG_DATA_VALUE_HIGH_1_OFFSET + 1) //23 JUMPS
#define HANDLER_SCORPIO_FLAG_DATA_VALUE_LOW_1_OFFSET               (HANDLER_SCORPIO_FLAG_DATA_VALUE_HIGH_2_OFFSET + 1) //24 JUMPS
#define HANDLER_SCORPIO_FLAG_DATA_VALUE_LOW_2_OFFSET               (HANDLER_SCORPIO_FLAG_DATA_VALUE_LOW_1_OFFSET + 1) //25 JUMPS

#define HANDLER_SCORPIO_FCN_INDEX_HIGH_OFFSET                      (16) //16 JUMPS
#define HANDLER_SCORPIO_FCN_INDEX_LOW_OFFSET                       (HANDLER_SCORPIO_FCN_INDEX_HIGH_OFFSET + 1 ) //17 JUMPS
#define HANDLER_SCORPIO_FCN_SIZE_INDEX_OFFSET                      (HANDLER_SCORPIO_FCN_INDEX_LOW_OFFSET  + 1 ) //18 JUMPS
#define HANDLER_SCORPIO_FlAG_WRITE_DATA_SIZE_INDEX_OFFSET          (HANDLER_SCORPIO_FCN_SIZE_INDEX_OFFSET + 3 ) //21 JUMPS
#define HANDLER_SCORPIO_FlAG_DATA_INDEX_OFFSET                     (HANDLER_SCORPIO_FCN_SIZE_INDEX_OFFSET + 4 ) //22 JUMPS

#define HANDLER_SCORPIO_ESPFCN_TYPE_REPLYDATA_HIGH_OFFSET          (HANDLER_SCORPIO_FCN_INDEX_HIGH_OFFSET + 1) //17 JUMPS
#define HANDLER_SCORPIO_ESPFCN_TYPE_REPLYDATA_LOW_OFFSET           (HANDLER_SCORPIO_ESPFCN_TYPE_REPLYDATA_HIGH_OFFSET + 1) //18 JUMPS



//Buffers & Others defines..
#define HANDLER_SCORPIO_CRC                                                        (2)
#define CONSTANT_AFFECTED_REGISTERS                                (0x02)
#define BufferSize                                                 (500)
#define lByteCRC                                                   (1)
#define hByteCRC                                                   (2)
#define REPLY_DATA_CRC                                             (56165)
#define sizeSNSCORPIO                                              (16)
#define SCORPIO_MAX_DATA_SIZE                                      (200)



// Function type defines
//*************************DEFINE TYPE FLAGS************************************
#define HANDLER_SCORPIO_FLAG_DATA_VALUE                            (0x10)

#define HANDLER_SCORPIO_FLAG_ADDR_PASSWORD_HIGH_VALUE              (0x17)
#define HANDLER_SCORPIO_FLAG_ADDR_PASSWORD_LOW_VALUE               (0x02)
#define HANDLER_SCORPIO_FLAG_ADDR_HIGH_1_PTR                       (0xC3)

#define HANDLER_SCORPIO_FLAG_ADDR_HIGH_2_00_PTR                    (0x00)
#define HANDLER_SCORPIO_FLAG_ADDR_LOW_1_00_PTR                     (0x00)
#define HANDLER_SCORPIO_FLAG_ADDR_LOW_2_RELAY_PTR                  (0x01)
#define HANDLER_SCORPIO_FLAG_ADDR_LOW_2_IRMODE_PTR                 (0x02)
#define HANDLER_SCORPIO_FLAG_ADDR_LOW_2_3PHMODE_PTR                (0x04)
#define HANDLER_SCORPIO_FLAG_ADDR_LOW_2_HMMODE_PTR                 (0x08)
#define HANDLER_SCORPIO_FLAG_ADDR_LOW_2_VOLTAGEMODE_PTR            (0x10)
#define HANDLER_SCORPIO_FLAG_ADDR_LOW_2_ACCRESET_PTR               (0x20)
#define HANDLER_SCORPIO_FLAG_ADDR_LOW_2_FORCERESET_PTR             (0x40)
#define HANDLER_SCORPIO_FLAG_ADDR_LOW_2_STPMCRESET_PTR             (0x80)

#define HANDLER_SCORPIO_FLAG_ADDR_LOW_1_01_PTR                     (0x01)
#define HANDLER_SCORPIO_FLAG_ADDR_LOW_2_DEMANDRESET_PTR            (0x00)

#define HANDLER_SCORPIO_FLAG_ADDR_LOW_1_02_PTR                     (0x02)
#define HANDLER_SCORPIO_FLAG_ADDR_LOW_2_UPGRADEPROCESS_PTR         (0x00)

#define HANDLER_SCORPIO_FLAG_ADDR_LOW_1_40_PTR                     (0x40)
#define HANDLER_SCORPIO_FLAG_ADDR_LOW_2_BOOTLOADERREGULAR_PTR      (0x00)

#define HANDLER_SCORPIO_FLAG_ADDR_SN_CFE_HIGH_PTR                  (0x04)
#define HANDLER_SCORPIO_FLAG_ADDR_SN_CFE_LOW_PTR                   (0x08)

#define HANDLER_SCORPIO_FLAG_ADDR_HIGH_1_BOOTLOADER_PTR            (0x74)
#define HANDLER_SCORPIO_FLAG_ADDR_HIGH_2_BOOTLOADER_PTR            (0xCA)
#define HANDLER_SCORPIO_FLAG_ADDR_LOW_1_BOOTLOADER_PTR             (0xF0)
#define HANDLER_SCORPIO_FLAG_ADDR_LOW_2_BOOTLOADER_PTR             (0x2D)

#define HANDLER_SCORPIO_FLAG_ADDR_HIGH_2_ROLLING_DEMAND_PTR        (0x01)
#define HANDLER_SCORPIO_FLAG_ADDR_LOW_1_ROLLING_DEMAND_PTR         (0x00)
#define HANDLER_SCORPIO_FLAG_ADDR_LOW_2_ROLLING_DEMAND_PTR         (0x00)

#define HANDLER_SCORPIO_FLAG_ADDR_SYTEM_HIGH_VALUE                 (0x23)
#define HANDLER_SCORPIO_FLAG_ADDR_SYTEM_LOW_VALUE                  (0x02)

#define HANDLER_SCORPIO_FLAG_DATA_VALUE_00_HIGH_1                  (0x00)
#define HANDLER_SCORPIO_FLAG_DATA_VALUE_00_HIGH_2                  (0x00)
#define HANDLER_SCORPIO_FLAG_DATA_VALUE_00_LOW_1                   (0x00)
#define HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_RELAY_ON             (0x00)
#define HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_RELAY_OFF            (0x01)
#define HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_IRMODE_REA           (0x02)
#define HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_IRMODE_ACT           (0x00)
#define HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_3PHMODE_DELTA        (0x04)
#define HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_3PHMODE_WYE          (0x00)
#define HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_HMMODE_HM            (0x08)
#define HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_HMMODE_OM            (0x00)
#define HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_VOLTAGEMODE_120      (0x10)
#define HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_VOLTAGEMODE_240      (0x00)
#define HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_ACC_RESET            (0x20)
#define HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_FORCE_RESET          (0x40)
#define HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_STPMC_RESET          (0x80)

#define HANDLER_SCORPIO_FLAG_DATA_VALUE_C0_LOW_1                   (0xC0)
#define HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_UARTPARITY_EVEN      (0x00)

#define HANDLER_SCORPIO_FLAG_DATA_VALUE_40_LOW_1                   (0x40)
#define HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_BOOTLOADER_REGULAR   (0x00)

#define HANDLER_SCORPIO_FLAG_DATA_VALUE_01_LOW_1                   (0x01)
#define HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_DEMAND_RESET         (0x00)

#define HANDLER_SCORPIO_FLAG_DATA_VALUE_02_LOW_1                   (0x02)
#define HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_UPGRADE_PROCESS      (0x00)

#define HANDLER_SCORPIO_FLAG_DATA_VALUE_01_HIGH_2                  (0x01)
#define HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_DEMAND_INTERVAL_3M   (0x00)
#define HANDLER_SCORPIO_FLAG_DATA_LOW_2_VALUE_DEMAND_INTERVAL_15M  (0x00)

#define HANDLER_SCORPIO_FLAG_SIZE                                  (28)
#define HANDLER_SCORPIO_FLAG_WRITE_SIZE                            (40)

#define HANDLER_SCORPIO_FLAG_DATA_VALUE                            (0x10)
#define HANDLER_SCORPIO_NO_FLAG_DATA_VALUE                         (0)
#define HANDLER_SCORPIO_FLAG_DATA_SIZE                             (4)
#define HANDLER_SCORPIO_FLAG_REPLY_DATA_SPACE_SIZE                 (3)
#define HANDLER_SCORPIO_FLAG_PUSHBUTTON_SPACE_SIZE                 (0)
#define HANDLER_SCORPIO_FLAG_WRITE_SN_SIZE                         (0x10)

//****************************DEFINE TYPE FUNCTIONS*****************************

#define HANDLER_SCORPIO_FUNCTION_ID_HIGH_VALUE                     (0x00)
#define HANDLER_SCORPIO_FUNCTION_ID_LOW_VALUE                      (0x03)
#define HANDLER_SCORPIO_ESPFCN_TYPE_REPLYDATA_HIGH_VALUE           (0x90)
#define HANDLER_SCORPIO_ESPFCN_TYPE_REPLYDATA_LOW_VALUE            (0x03)

#define HANDLER_SCORPIO_ESPECIAL_FUNCTION_TYPEMETER_MONOPH         (0x81)
#define HANDLER_SCORPIO_ESPECIAL_FUNCTION_TYPEMETER_BIPH           (0x82)
#define HANDLER_SCORPIO_ESPECIAL_FUNCTION_TYPEMETER_TRIPH          (0x83)
#define HANDLER_SCORPIO_ESPECIAL_FUNCTION_DELETE_METER             (0x66)
#define HANDLER_SCORPIO_FUNCTION_PUSHBUTTON_ID_VALUE               (0x81)

#define HANDLER_SCORPIO_FCN_PUSHBUTTON_SIZE                         (19)
#define HANDLER_SCORPIO_ESPFCN_REPLY_DATA_SIZE                      (21)
#define HANDLER_SCORPIO_FUNCTION_DATA_SIZE                          (21)
#define HANDLER_SCORPIO_NO_FCN_DATA_VALUE                           (0)
#define HANDLER_SCORPIO_FCN_SN_READ_DATA_SIZE                     (0x10)
#define HANDLER_SCORPIO_FCN_MTR_READ_DATA_SIZE                    (0x94) /*(0xDC)*/

//******************************************************************************
// Struct data
typedef struct{
    BYTE id;
    BYTE function;
    BYTE data_size;
    BYTE data[HANDLER_SCORPIO_WINDOW];
    WORD crc;
}HANDLER_SCORPIO_STRUCT;


typedef struct{
    BYTE serialNumber[SCORPIO_MAX_DATA_SIZE];
    BYTE serialNumberLen;
    BYTE dataSize;
    BYTE fcn;
    BYTE flag_password;
    BYTE flag_system;
    BYTE data[SCORPIO_MAX_DATA_SIZE];
} SCORPIO_STRUCT, * SCORPIO_STRUCT_PTR ; 

//******************************************************************************
// API

boolean wfnCRC_CALC_SCORPIO(BYTE *ptFRAME, WORD wSizeFrame, WORD wCRCStart);
BYTE Handler_Flag_Addr_PassWord_Check_SCORPIO(BYTE * ptbFrame, WORD wSizeFrame, BYTE * flag_state);
BYTE Handler_Flag_Addr_System_Check_SCORPIO(BYTE * ptbFrame, WORD wSizeFrame, BYTE * flag_state);
BYTE Handler_FcnCheck_SCORPIO(BYTE * function_data);
BYTE Handler_Size_Data_Check_SCORPIO(BYTE* size_data_value, WORD wSizeFrame, BYTE* data);
BYTE Handler_Serial_Number_Size_Check_SCORPIO(BYTE* id_data, BYTE* data);
WORD API_SCORPIO_Recieve_handler(BYTE* buffer, WORD buffersize, METER_DESCRIPTOR_PTR meterDescriptor, BYTE * commandCallBack);
WORD API_SCORPIO_Meter_response_handler( BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, BYTE command, BYTE * response, WORD maxResponseLen, WORD * responseLen);

BYTE HandlerScorpio_GetInvokeFunctionId(BYTE command);

#define BETA_1              (1.00)
#define BETA_100            (100.00)
#define BETA_1000           (1000.00)
#define MANTISA_SIZE        23
#define EXP_MAX             128
#define MANTISA_MASK        0x007fffff
#define NULL_DWORD_DATA     0xFFFFFFFF
#define NULL_WORD_DATA      0xFFFF

void ScorpioMeterHandler_ParseToDataReading( Data_Readings_Ptr dataReading, BYTE * data,  WORD dataLen);
void ScorpioMeterHandler_PrintDataReading(Data_Readings_Ptr dataReading);

typedef struct __attribute__((packed,aligned(1))) _Data_Readings2
{
    DWORD   ENERGY_ACT_C_Add;
    DWORD   ENERGY_ACT_B_Add;
    DWORD   ENERGY_ACT_A_Add;
    DWORD   POWER_Aparent_SYSTEM_Add;
    DWORD   POWER_React_SYSTEM_Add;
    DWORD   POWER_ACT_SYSTEM_Add;
    DWORD   Time1;
    DWORD   ROLLER_DEMAND_Add1;
    DWORD   Time2;
    DWORD   ROLLER_DEMAND_Add2;
    DWORD   Time3;
    DWORD   ROLLER_DEMAND_Add3;
    DWORD   Time4;
    DWORD   ROLLER_DEMAND_Add4;
    DWORD   POWER_FACTOR_Add1;
    DWORD   POWER_FACTOR_Add2;
    DWORD   POWER_FACTOR_Add3;
    DWORD   POWER_FACTOR_Add;
    DWORD   CURRENT_C_Add;
    DWORD   CURRENT_B_Add;
    DWORD   CURRENT_A_Add;
    DWORD   VOLTAGE_C_Add;
    DWORD   VOLTAGE_B_Add;
    DWORD   VOLTAGE_A_Add;
    DWORD   FRECUENCY_Add;
    DWORD   TIME_STAMP_Add1;
    DWORD   TIME_STAMP_Add;
    DWORD   ENERGY_NEG_REACT_TOTAL_Add;
    DWORD   ENERGY_NEG_ACT_TOTAL_Add;
    DWORD   ENERGY_REACT_TOTAL_Add;
    DWORD   ENERGY_ACT_TOTAL_Add;
    DWORD   FLAGS_Add_LWEND;
    WORD    INTERRUPT_COUNT;
    DWORD   OFF_TIME;
    DWORD   RESTART_TIME;
    WORD    SUBINTERVAL_TIME;
    WORD    TEST_TIME;
    WORD    FW_VER;
    WORD    TOU_ID;
    WORD    KH;
}Data_Readings2, * Data_Readings2_ptr;

DWORD ScorpioMeterHandler_DWORD_Parser(DWORD value, float scale);
#endif	/* HANDLERSCORPIO_H */

