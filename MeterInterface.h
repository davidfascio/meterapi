/* 
 * File:   MeterInterface.h
 * Author: fascio
 *
 * Created on 21 de diciembre de 2015, 11:15
 */

#ifndef __METER_INTERFACE_H__
#define	__METER_INTERFACE_H__

//******************************************************************************
// Includes
//******************************************************************************
#include "MeterCommons.h"
#include "G155MeterInterface.h"

//******************************************************************************
// Defines
//******************************************************************************
#define METER_INTERFACE_METER_TYPE_INDEX_OVERFLOW_ERROR_CODE                (-1)
#define METER_INTERFACE_NO_METER_TYPE_ERROR_CODE                            (-2)


//******************************************************************************
// Data types
//******************************************************************************



//******************************************************************************
// Meter Interface Function
//******************************************************************************
BYTE MeterInterface_GetMeterTypeByIndex(BYTE index);
METER_COMMAND_ID_FUNCTION_API_PTR MeterInterface_GetMeterCommandIdFunctionAPI(BYTE meterType);
COMMAND_ID_FUNCTION_PTR MeterInterface_GetCommandIdFunction(METER_COMMAND_ID_FUNCTION_API_PTR meterCommandIdFunctionAPI, BYTE command);
WORD MeterInterface_GetStabilizationTimeoutValue(BYTE meterType);

#endif	/* __METER_INTERFACE_H__ */

