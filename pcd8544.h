#ifndef __PCD8544_H
#define __PCD8544_H

#ifdef __cplusplus
extern "C" {
#endif
  
#include "font.h"
#include <stm32f1xx_hal.h>
#include <stddef.h>


/** @brief DisplayIO, initialize display pins 
  * @return void
**/
void lcd_init(void);


/** @brief LCDClear clear the scren
 *  @return void
**/
void lcd_clear();


/** @brief Print character 
 *  @param character is char to be printed
 *   @return character
**/
void lcd_putch(char character);


/** @brief Print string using X,Y position
 *  @param str is string to be printed
 *  @param X_Pos is x position ( 0 to 83 )
 *  @param Y_Pos is y position ( 0 to 5 )
    @return *str
**/
void lcd_print(const char *str,size_t X_Pos, size_t Y_Pos);


/** @brief lcdInverse inverse colors
  * @return void
**/
void lcd_inverse_colour(void);


/** @brief lcdNonInverse inverts the color of the screen
  * @return void
**/
void lcd_non_inverse_colour(void);


/** @brief LCDCursor adjust cursor
  * @param positionX, set X poisiton
  * @param positionY, set Y position
  * @return void
**/
void lcd_set_cursor(int positionX, int positionY);



/** @brief printImage prints array named github in font.h
 *  @return void
**/
void print_github_image(void);
  


#ifdef __cplusplus
}
#endif

#endif 