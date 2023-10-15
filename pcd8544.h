#ifndef __PCD8544_H
#define __PCD8544_H

#ifdef __cplusplus
extern "C" {
#endif
  
#include "font.h"
#include <stdint.h>



/** @brief DisplayIO, initialize display pins 
  * @return void
**/
void displayInit(void);


/** @brief sendCommand sends command to LCD 
  * @param Data is parameter to be send to LCD
  * @return void
**/
void sendCommand(uint8_t Data);


/** @brief sendData sends data to LCD 
  * @param Data is parameter to be send to LCD
  * @return void
**/
void sendData(uint8_t Data);


/** @brief LCDClear clear the scren
 *  @return void
**/
void LCDClear();


/** @brief Print character 
 *  @param character is char to be printed
 *   @return character
**/
int Putch(char character);


/** @brief Print string using X,Y position
 *  @param str is string to be printed
 *  @param X_Pos is x position ( 0 to 83 )
 *  @param Y_Pos is y position ( 0 to 5 )
    @return *str
**/
int lcdPrint(char *str,int X_Pos, int Y_Pos);


/** @brief lcdInverse inverse colors
  * @return void
**/
void lcdInverse(void);


/** @brief lcdNonInverse inverts the color of the screen
  * @return void
**/
void lcdNonInverse(void);



/** @brief LCDCursor adjust cursor
  * @param positionX, set X poisiton
  * @param positionY, set Y position
  * @return void
**/
void LCDCursor(int positionX, int positionY);



/** @brief printImage prints array named github in font.h
 *  @return void
**/
void printImage(void);
  


#ifdef __cplusplus
}
#endif

#endif 