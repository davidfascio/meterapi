#include "Utility.h"



WORD wfnCRC_CALC (BYTE *ptFRAME, WORD wSizeFrame, WORD wCRCStart){
    WORD temp;
    BYTE c, flag;
    for (wSizeFrame;wSizeFrame>0;wSizeFrame--){
        temp = (WORD) *ptFRAME;
        temp &= 0x00FF;
        wCRCStart = wCRCStart ^ temp;
        for (c=0;c<8;c++){
            flag = wCRCStart & 0x01;
            wCRCStart = wCRCStart>>1;
            if(flag != 0) wCRCStart = wCRCStart ^ 0xA001;
        }
        ptFRAME++;
    }
    //wCRCStart = (wCRCStart >> 8) | (wCRCStart << 8);
    return (wCRCStart);
}
