/* 
 * File:   main.c
 * Author: Salvador García
 *
 * Created on 18 de noviembre de 2015, 15:36
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "handlerG155.h"
#include "handlerSCORPIO.h"


typedef struct{
    BYTE * buffer;
    WORD buffer_size;
}TESTING_STRING;


    //Leer SN CFE:
    BYTE sn_cfe[] = {0x02, 0x03, 0x06, 0x32, 0x37, 0x39, 0x44, 0x59, 0x4C, 0xB3, 0x7F};
    
    //Leer versión Frimware:
    BYTE firmware_ver[] = {0x02, 0x03, 0x02, 0x04, 0x00, 0xFE, 0x84};
    
    //Relay off
    BYTE relay_off[] = {0x02, 0x06, 0x00, 0x0C, 0x00, 0x01, 0x88, 0x3A};
    
    //Relay on
    BYTE relay_on[] = {0x02, 0x06, 0x00, 0x0C, 0x00, 0x00, 0x49, 0xFA};
    
    //Lectura de medidor
    BYTE read_meter[] = {0x02, 0x03, 0x10, 0x00, 0x00, 0x15, 0x2A, 0x00, 0x4B, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x22, 0x32, 0x18, 0x81, 0xDB, 0xF0};
    
    //Mandar calibración por default
    BYTE default_calibration[] = {0x01, 0x06, 0x00, 0x0D, 0x42, 0x0B, 0x69, 0x6E};
    
    //Escribir No CFE
    BYTE write_CFE[] = {0x04, 0x10, 0x00, 0x0E, 0x00, 0x03, 0x06, 0x31, 0x35, 0x30, 0x35, 0x35, 0x36, 0x8B, 0x1D};
    
    //Reset Acumulados
    BYTE accumulated_reset[] = {0x04, 0x06, 0x00, 0x0C, 0x00, 0x04, 0x48, 0x5F};
    
    //Pulsos para KVARHr(Reactivo)
    BYTE pulse_kvarhr[] = {0x04, 0x06, 0x00, 0x0C, 0x00, 0x02, 0xC8, 0x5D};
    
    
    //Reset
    BYTE reset[] = {0x19, 0x06, 0x00, 0x0C, 0x00, 0x08, 0x4B, 0xD7}; 
    
    //Password Reset
    BYTE password_reset[] = {0x19, 0x06, 0x00, 0x1C, 0xC3, 0x08, 0x1A, 0xE2}; 
       
    //Password Relay
    BYTE password_relay[] = {0x04, 0x06, 0x00, 0x1C, 0xC3, 0x01, 0xD9, 0x69};
    
    //Password Reset Acumulados
    BYTE password_accumulated_reset[] = {0x04, 0x06, 0x00, 0x1C, 0xC3, 0x04, 0x19, 0x6A};
    
    //Password Cambio de Pulsos
    BYTE password_pulse[] = {0x04, 0x06, 0x00, 0x1C, 0xC3, 0x02, 0x99, 0x68};
    
    //Error
    BYTE buffer_error[] = {0x30 ,0x30 ,0x30 ,0x30 ,0x30 ,0x30 ,0x30 ,0x30 ,0x30 ,0x30 ,0x30 ,0x30 ,0x30 ,0x30 ,0x30 ,0x30 ,0x00 ,0x10 ,0x00 ,0x03 ,0x00 ,0x02 ,0xFF ,0xFF ,0xFF ,0xFF ,0xB2 ,0x5A};
    
    //Error Size:
    BYTE error_size[] = {0x02, 0x03, 0x07, 0x32, 0x37, 0x39, 0x44, 0x59, 0x4C, 0xA3, 0xBF};
    
    //Error Fcn
    BYTE error_fcn[] = {0x02, 0x11, 0x00, 0x0C, 0x00, 0x01, 0xFC, 0x39};
    
    //Error address flag
    BYTE error_address_flag[] = {0x01, 0x06, 0x00, 0xEA, 0x42, 0x0B, 0xD9, 0x59};
    
    //Error password
    BYTE error_password[] = {0x04, 0x06, 0x00, 0x1C, 0xC3, 0x05, 0xD8, 0xAA}; 
    
    //Bautizo
   
    BYTE zigbee_baptise[] = {0x00, 0x10, 0x00, 0x12, 0x00, 0x04, 0x08, 0x24, 0xDA, 0xB6, 0x0D, 0x00, 0x00, 0x00, 0xD9, 0xC1, 0x69};
    
    BYTE baptise_frame[] = {0xF7, 0x06, 0x00, 0x11, 0x00, 0x04, 0xCC, 0x9A};
    
    //====================================================================================================================================
    
    //Numero Linker Scorpio
    
    BYTE numero_linker[] = {0x30, 0x30, 0x30, 0x43, 0x4F, 0x50, 0x45, 0x52, 0x4E, 0x49, 0x43, 0x4F, 0x31, 0x30, 0x38, 0x37, 0x00, 0x03, 0x04, 0xFF, 0xFF, 0xFF, 0xFF, 0x27, 0x8F}; //, 0x27, 0x8F ----> A001
    
    //Relay ON Password Scorpio 
    
    BYTE relay_on_password[] = {0x30, 0x30, 0x30, 0x43, 0x4F, 0x50, 0x45, 0x52, 0x4E, 0x49, 0x43, 0x4F, 0x31, 0x30, 0x38, 0x37, 0x00, 0x10, 0x00, 0x17, 0x00, 0x02, 0xC3, 0x00, 0x00, 0x01, 0x9D, 0x2E};
    
    //Relay ON Value Scorpio 
    
    BYTE relay_on_value[] = {0x30, 0x30, 0x30, 0x43, 0x4F, 0x50, 0x45, 0x52, 0x4E, 0x49, 0x43, 0x4F, 0x31, 0x30, 0x38, 0x37, 0x00, 0x10, 0x00, 0x23, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0xDA, 0xA6};
    
    //Relay OFF Value Scorpio 
    
    BYTE relay_off_value[] = {0x30, 0x30, 0x30, 0x43, 0x4F, 0x50, 0x45, 0x52, 0x4E, 0x49, 0x43, 0x4F, 0x31, 0x30, 0x38, 0x37, 0x00, 0x10, 0x00, 0x23, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x1A, 0x67 };
    
   //Meter demand reset Passcword Scorpio
    
    BYTE demand_reset_password[] = {0x30, 0x30, 0x30, 0x43, 0x4F, 0x50, 0x45, 0x52, 0x4E, 0x49, 0x43, 0x4F, 0x31, 0x30, 0x38, 0x37, 0x00, 0x10, 0x00, 0x17, 0x00, 0x02, 0xC3, 0x00, 0x01, 0x00, 0xCD, 0xEE};

   //Meter demand reset Passcword Scorpio
    
    BYTE demand_reset_value[] = {0x30, 0x30, 0x30, 0x43, 0x4F, 0x50, 0x45, 0x52, 0x4E, 0x49, 0x43, 0x4F, 0x31, 0x30, 0x38, 0x37, 0x00, 0x10, 0x00, 0x23, 0x00, 0x02, 0x00, 0x00, 0x01, 0x00, 0x4A, 0xA7};

   //Meter force bootloader Password Scorpio
    
    BYTE force_bootloader_password[] = {0x30, 0x30, 0x30, 0x43, 0x4F, 0x50, 0x45, 0x52, 0x4E, 0x49, 0x43, 0x4F, 0x31, 0x30, 0x38, 0x37, 0x00, 0x10, 0x00, 0x17, 0x00, 0x02, 0x74, 0xCA, 0xF0, 0x2D, 0xCA, 0x6C};
    
   //Cambiar número CFE Scorpio
    
    BYTE change_cfe_number[] = {0x30, 0x30, 0x30, 0x43, 0x4F, 0x50, 0x45, 0x52, 0x4E, 0x49, 0x43, 0x4F, 0x31, 0x30, 0x38, 0x37, 0x00, 0x10, 0x00, 0x04, 0x00, 0x08, 0x30, 0x30, 0x30, 0x43, 0x4F, 0x50, 0x45, 0x52, 0x4E, 0x49, 0x43, 0x4F, 0x31, 0x30, 0x38, 0x37, 0xDD, 0x4C};

   //Push button Scorpio
    
    BYTE pushbutton[] = {0x30, 0x30, 0x30, 0x43, 0x4F, 0x50, 0x45, 0x52, 0x4E, 0x49, 0x43, 0x4F, 0x31, 0x30, 0x38, 0x37, 0x81, 0x2E, 0x2E};
    
  //Uart Parity Even Password Scorpio
    
    BYTE uartparity_even_scorpio[] = {0x30, 0x30, 0x30, 0x30, 0x30, 0x4E, 0x4B, 0x31, 0x31, 0x35, 0x31, 0x31, 0x30, 0x31, 0x34, 0x31, 0x00, 0x10, 0x00, 0x17, 0x00, 0x02, 0xC3, 0x00, 0x40, 0x00, 0x88, 0xF0};
    
    //Error serial number Scoprio
    
    BYTE error_serial_number[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0xFF, 0xFF, 0xFF, 0xFF, 0x2E, 0x5B};
   
   
    
    
#define TESTING_STRING_NULL  {NULL, 0} 


TESTING_STRING testing[] = 
{
    
    //Preubas:
    {firmware_ver, sizeof(firmware_ver)},
    {zigbee_baptise, sizeof(zigbee_baptise)},
    {relay_on, sizeof(relay_on)},
    {sn_cfe, sizeof(sn_cfe)},
    {read_meter, sizeof(read_meter)},
    
    //Medidor Scorpio
    {numero_linker, sizeof(numero_linker)},
    {relay_on_password, sizeof(relay_on_password)},
    {relay_on_value, sizeof(relay_on_value)},
    {demand_reset_password, sizeof(demand_reset_password)},
    {demand_reset_value, sizeof(demand_reset_value)},
    {force_bootloader_password, sizeof(force_bootloader_password)},
    {change_cfe_number, sizeof(change_cfe_number)},
    {pushbutton, sizeof(pushbutton)},
    {uartparity_even_scorpio, sizeof(uartparity_even_scorpio)},
    {relay_off_value, sizeof(relay_off_value)},
    //Medidor G155
    {firmware_ver, sizeof(firmware_ver)},
    {relay_off, sizeof(relay_off)},
    {sn_cfe, sizeof(sn_cfe)},
    {relay_on, sizeof(relay_on)},
    {read_meter, sizeof(read_meter)},
    {default_calibration, sizeof(default_calibration)},
    {write_CFE, sizeof(write_CFE)},
    {accumulated_reset, sizeof(accumulated_reset)},
    {pulse_kvarhr, sizeof(pulse_kvarhr)},
    {reset, sizeof(reset)},
    {password_relay, sizeof(password_relay)},
    {password_accumulated_reset, sizeof(password_accumulated_reset)},
    {password_pulse, sizeof(password_pulse)},
    {password_reset, sizeof(password_reset)},
    {zigbee_baptise, sizeof(zigbee_baptise)},
    {baptise_frame, sizeof(baptise_frame)},
    //Error
    {error_size, sizeof(error_size)},
    {error_fcn, sizeof(error_fcn)},
    {error_address_flag, sizeof(error_address_flag)},
    {error_password, sizeof(error_password)},
    {error_serial_number, sizeof(error_serial_number)},
    {buffer_error, sizeof(buffer_error)}
};


/*
 * 
 */

void printTramas(BYTE * buffer, WORD buffer_size);

int main(int argc, char** argv) {
    
    WORD ISG155;
    WORD ISSCORPIO;
    int index;
    
    for (index = 0; index < (sizeof(testing)/sizeof(TESTING_STRING)); index++){
    
        ISG155    = API_G155_Recieve_handler(testing[index].buffer,testing[index].buffer_size);
        ISSCORPIO = API_SCORPIO_Recieve_handler(testing[index].buffer,testing[index].buffer_size);
        printf("Buffer: ");
        printTramas(testing[index].buffer,testing[index].buffer_size);
        printf("\n");
        
        
        if (ISG155 == HANDLER_G155_NO_ERROR){
            printf("Status: Medidor G155\n\n");
        }
        if (ISSCORPIO == HANDLER_SCORPIO_NO_ERROR){
            printf("Status: Medidor Scorpio\n\n");
        }
        if ( ISG155 != HANDLER_G155_NO_ERROR && ISSCORPIO != HANDLER_SCORPIO_NO_ERROR) {
            
            printf("Status: Medidor no identificado\n\n");
        }
    }
    //return (EXIT_SUCCESS);
}


void printTramas(BYTE * buffer, WORD buffer_size){
    int index;
    
    for (index = 0; index < buffer_size; index++, buffer++){
        
        printf("0x%02X ", *buffer);        
    }
}
