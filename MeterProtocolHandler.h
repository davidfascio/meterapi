/* 
 * File:   MeterProtocolHandler.h
 * Author: fascio
 *
 * Created on 4 de enero de 2016, 16:58
 */

#ifndef __METER_PROTOCOL_HANDLER_H__
#define	__METER_PROTOCOL_HANDLER_H__

#include "ComSerialInterface.h"

void MeterProtocolHandler(void);
void MeterProtocolHandler_PrintData(BYTE* frame, BYTE frameLen);

#endif	/* __METER_PROTOCOL_HANDLER_H__ */

