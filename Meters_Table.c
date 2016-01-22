#include "Meters_Table.h"


Meters_Table            Meters_Table1;

//******************************************************************************
// Meter Table Function
//******************************************************************************

BYTE MeterTable_GetMeterTypeByMeterId(BYTE meterId){
    
    BYTE meterTypeFromTable;
        
    if (meterId > NUM_MAX_METERS)
        return METER_TABLE_METER_ID_ERROR_CODE;  
    
    meterTypeFromTable = Meters_Table1.Meter_DEV[meterId].Type;        
    
    return meterTypeFromTable;     
}

BYTE MeterTable_GetModbusIdByMeterId(BYTE meterId){
    
    if (meterId > NUM_MAX_METERS)
        return METER_TABLE_METER_ID_ERROR_CODE;
    
    return (Meters_Table1.Meter_DEV[meterId].Signature) - SIGNATURE_MTR + METER_TABLE_OFFSET_SIGNATURE_MTR;
}

BYTE MeterTable_GetSerialNumberByMeterId(BYTE meterId, BYTE * serialNumber, BYTE serialNumberLen){
    
    if (meterId > NUM_MAX_METERS)
        return 0;
    
    if(serialNumberLen < Lenght_Meter_ID)
        return 0;
    
    memset(serialNumber, 0, serialNumberLen);
    memcpy(serialNumber, Meters_Table1.Meter_DEV[meterId].Serial_Num, Lenght_Meter_ID);        
    return Lenght_Meter_ID;    
}

BYTE MeterTable_FindAvailableMeterId(void){
    
    BYTE index = 0;
    
    /*while ( ( Meters_Table1.Meter_DEV[index++].Signature != METER_TABLE_EMPTY_LOCATION) &&
            ( index < NUM_MAX_METERS));*/
    
    for(index = 0; Meters_Table1.Meter_DEV[index].Signature != METER_TABLE_EMPTY_LOCATION; index++){
        
        if (index >= NUM_MAX_METERS)
            break;
    }       
    
    if(index == NUM_MAX_METERS)
        return METER_TABLE_NO_METER_ID_FOUND;
    
    return index;
}

BYTE MeterTable_FindAvailableModbusId(void){
    
    BYTE modbusId = MeterTable_FindAvailableMeterId();
    return ++modbusId;
}

BOOL MeterTable_IsValidSerialNumber(BYTE * serialNumber, WORD serialNumberLen){
    
    BYTE index;
    
    if(serialNumberLen != Lenght_Meter_ID)
        return FALSE;
    
    for (index = 0; 
        (isalnum(* serialNumber) && (index < serialNumberLen));
        index++, serialNumber++);
    
    if (index < serialNumberLen)
        return FALSE;
            
     
    return TRUE;
}

BYTE MeterTable_FindMeterBySerialNumber(BYTE * serialNumber, WORD serialNumberLen){
    
    BYTE index;
    
    for (index = 0;
         (memcmp(Meters_Table1.Meter_DEV[index].Serial_Num, serialNumber, serialNumberLen) && (index < NUM_MAX_METERS));
         index++);
    
    if(index == NUM_MAX_METERS)
        return METER_TABLE_SERIAL_NUMBER_NOT_FOUND;
    
    return index;    
}

BYTE MeterTable_AddNewMeter(BYTE meterId, BYTE meterType,BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen){
    
    Meter_Eneri * meter_eneri_ptr = &Meters_Table1.Meter_DEV[meterId];
    
    /*if(bfnIIC_MEM24_1025_Write(serialNumber,
            Meter_Table_ADD+((meterId)*serialNumberLen),
            serialNumberLen) == FALSE)
        
        return METER_TABLE_COULD_NOT_SAVE_SERIAL_NUMBER_IN_NVM;*/
    
    memcpy(meter_eneri_ptr->Serial_Num, serialNumber, serialNumberLen);
    meter_eneri_ptr->Type = meterType;
    memset(meter_eneri_ptr->MACAdd_Display.v, METER_TABLE_EMPTY_VALUE, MAC_SIZE);
    meter_eneri_ptr->Signature = modbusId + SIGNATURE_MTR - METER_TABLE_OFFSET_SIGNATURE_MTR;
    
    printf("Adding new meter\n");
    return METER_TABLE_METER_NO_ERROR_CODE;
}

BYTE MeterTable_UpdateMeter(BYTE meterId, BYTE meterType, BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen){
    
    Meter_Eneri * meter_eneri_ptr = &Meters_Table1.Meter_DEV[meterId];
    
    if (memcmp(meter_eneri_ptr->Serial_Num, serialNumber, serialNumberLen))
        return METER_TABLE_SERIAL_NUMBER_NOT_MATCHED_ERROR_CODE;
    
    meter_eneri_ptr->Type = meterType;
    meter_eneri_ptr->Signature = modbusId + SIGNATURE_MTR - METER_TABLE_OFFSET_SIGNATURE_MTR;
    
    printf("Update meter\n");
    return METER_TABLE_METER_NO_ERROR_CODE;    
}

BYTE MeterTable_DeleteMeter(BYTE meterId, BYTE meterType,BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen){
    
    Meter_Eneri * meter_eneri_ptr = &Meters_Table1.Meter_DEV[meterId];
    
    /*if(bfnIIC_MEM24_1025_Write(serialNumber,
            Meter_Table_ADD+((meterId)*serialNumberLen),
            serialNumberLen) == FALSE)
        
        return METER_TABLE_COULD_NOT_SAVE_SERIAL_NUMBER_IN_NVM;*/
    
    memset(meter_eneri_ptr, METER_TABLE_EMPTY_VALUE, sizeof(Meter_Eneri));        
    printf("Deleting Meter\n");
    return METER_TABLE_METER_NO_ERROR_CODE;
}

BYTE MeterTable_AddNewMeterBySerialNumber(BYTE meterType, BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen){

    BYTE meterId;
    
    if(!MeterTable_IsValidSerialNumber(serialNumber, serialNumberLen))
        return METER_TABLE_SERIAL_NUMBER_ERROR_CODE;
    
    meterId = MeterTable_FindMeterBySerialNumber(serialNumber, serialNumberLen);
    
    if( meterId == METER_TABLE_SERIAL_NUMBER_NOT_FOUND)
    {        
        meterId = MeterTable_FindAvailableMeterId();
        
        if(meterId == METER_TABLE_NO_METER_ID_FOUND)
            return METER_TABLE_NO_METER_ID_FOUND;
        
        return MeterTable_AddNewMeter(meterId, meterType, modbusId, serialNumber, serialNumberLen);
    }
    
    return MeterTable_UpdateMeter(meterId, meterType, modbusId, serialNumber, serialNumberLen);    
    //return METER_TABLE_METER_ALREADY_EXISTS;
}

BYTE MeterTable_DeleteMeterBySerialNumber(BYTE meterType, BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen){
    
    BYTE meterId;
    
    if(!MeterTable_IsValidSerialNumber(serialNumber, serialNumberLen))
        return METER_TABLE_SERIAL_NUMBER_ERROR_CODE;
    
    meterId = MeterTable_FindMeterBySerialNumber(serialNumber, serialNumberLen);
    
    if( meterId == METER_TABLE_SERIAL_NUMBER_NOT_FOUND)
        return METER_TABLE_SERIAL_NUMBER_NOT_FOUND;
        
    return MeterTable_DeleteMeter(meterId, meterType, modbusId, serialNumber, serialNumberLen);
}

BYTE MeterTable_SaveMeasurementBySerialNumber(BYTE meterType, BYTE modbusId, BYTE * serialNumber, WORD serialNumberLen, Data_Readings_Ptr measurement){
 
    BYTE meterId;
    WORD crc_measurement;
    Readings_Eneri * readings_eneri_ptr;
    
    if(!MeterTable_IsValidSerialNumber(serialNumber, serialNumberLen))
        return METER_TABLE_SERIAL_NUMBER_ERROR_CODE;
    
    meterId = MeterTable_FindMeterBySerialNumber(serialNumber, serialNumberLen);
    
    if( meterId == METER_TABLE_SERIAL_NUMBER_NOT_FOUND)
        return METER_TABLE_SERIAL_NUMBER_NOT_FOUND;
    
    if(measurement == NULL)
        return METER_TABLE_MEASUREMENT_ERROR_CODE;
    
    readings_eneri_ptr = &Meters_Table1.Readings[meterId];    
    memcpy(&readings_eneri_ptr->Reading, measurement, sizeof(Data_Readings));
    
    //readings_eneri_ptr->Reading.TIME_STAMP_Add = ZCL_Callback_GetCurrentTimeInSeconds();
    
    crc_measurement = wfnCRC_CALC((BYTE *) &readings_eneri_ptr->Reading, sizeof(Data_Readings), METER_TABLE_MEASUREMENT_CRC_BASE);
    
    inverted_memcpy((BYTE *) readings_eneri_ptr->CRC, (BYTE *) &crc_measurement, sizeof(crc_measurement));
    
    return METER_TABLE_METER_NO_ERROR_CODE;
}
