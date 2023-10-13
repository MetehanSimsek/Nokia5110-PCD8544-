#ifndef __PCD8544_H
#define __PCD8544_H

#ifdef __cplusplus
extern "C" {
#endif
  
#include "font.h"
#include <stdint.h>




void displayInit(void);
int sendCommand(uint8_t Data);
void sendData(uint8_t Data);
void LCDClear();
int Putch(char character);
int lcdPrint(char *str,int X_Pos, int Y_Pos);
void lcdInverse(void);
void lcdNonInverse(void);


void printImage(void);
  
#ifdef __cplusplus
}
#endif

#endif 