/**************************************************************************
*
* Copyright 2014 by Petr Gargulak. eGUI Community.
* Copyright 2009-2013 by Petr Gargulak. Freescale Semiconductor, Inc.
*
***************************************************************************
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License Version 3
* or later (the "LGPL").
*
* As a special exception, the copyright holders of the eGUI project give you
* permission to link the eGUI sources with independent modules to produce an
* executable, regardless of the license terms of these independent modules,
* and to copy and distribute the resulting executable under terms of your
* choice, provided that you also meet, for each linked independent module,
* the terms and conditions of the license of that module.
* An independent module is a module which is not derived from or based
* on this library.
* If you modify the eGUI sources, you may extend this exception
* to your version of the eGUI sources, but you are not obligated
* to do so. If you do not wish to do so, delete this
* exception statement from your version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* You should have received a copy of the GNU General Public License
* and the GNU Lesser General Public License along with this program.
* If not, see <http://www.gnu.org/licenses/>.
*
***************************************************************************/ /*!
*
* @file      d4dlcd_template.c
*
* @author     Petr Gargulak
*
* @version   0.0.10.0
*
* @date      Jan-14-2014
*
* @brief     D4D driver - template lcd driver source c file
*
******************************************************************************/

#include "d4d.h" // include of all public items (types, function etc) of D4D driver
#include "common_files/d4d_lldapi.h" // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "common_files/d4d_private.h" // include the private header file that contains perprocessor macros as D4D_MK_STR
#include <SDL2/SDL.h>

/******************************************************************************
* D4D LCD Driver setting  constants
*
*/ /*! @addtogroup doxd4d_lcd_const
* @{
*******************************************************************************/

/*! @brief  Identification string of driver - must be same as name D4DLCD_FUNCTIONS structure + "_ID"
 it is used for enable the code for compilation   */
#define d4dlcd_sdl_ID 1

/*! @} End of doxd4d_lcd_const                                             */

// copilation enable preprocessor condition
// the string d4dlcd_template_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_LCD) == d4dlcd_sdl_ID)
//  #if (D4D_LLD_LCD == d4dlcd_template_ID)
// include of low level driver heaser file
// it will be included into wole project only in case that this driver is selected in main D4D configuration file
#include "low_level_drivers/LCD/lcd_controllers_drivers/sdl/d4dlcd_sdl.h"
/******************************************************************************
  * Macros
  ******************************************************************************/

/******************************************************************************
  * Internal function prototypes
  ******************************************************************************/

static unsigned char D4DLCD_Init_SDL(void);
static unsigned char D4DLCD_SetWindow_SDL(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
static unsigned char D4DLCD_SetOrientation_SDL(D4DLCD_ORIENTATION new_orientation);
static void D4DLCD_Send_PixelColor_SDL(D4D_COLOR color);
static D4D_COLOR D4DLCD_Read_PixelColor_SDL(void);
static void D4DLCD_Flush_SDL(D4DLCD_FLUSH_MODE mode);
static void D4DLCD_Delay_ms_SDL(unsigned short period);
static unsigned char D4DLCD_DeInit_SDL(void);

/**************************************************************/ /*!
  *
  * Global variables
  *
  ******************************************************************/

/******************************************************************************
  * D4D LCD Driver setting  variables
  *
  */ /*! @addtogroup doxd4d_lcd_variable
  * @{
  *******************************************************************************/
/*! @brief the main structure that contains low level driver api functions
   the name fo this structure is used for recognizing of configured low level driver of whole D4D
   so this name has to be used in main configuration header file of D4D driver to enable this driver
  */
const D4DLCD_FUNCTIONS d4dlcd_sdl = {
    D4DLCD_Init_SDL, ///< The pointer to driver initialization function
    D4DLCD_SetWindow_SDL, ///< The pointer to driver set drawing window function
    D4DLCD_SetOrientation_SDL, ///< The pointer to driver set screen orientation function
    D4DLCD_Send_PixelColor_SDL, ///< The pointer to driver send pixel to LCD function
    D4DLCD_Read_PixelColor_SDL, ///< The pointer to driver get pixel from LCD function
    D4DLCD_Flush_SDL, ///< The pointer to driver flush written pixels to LCD function
    D4DLCD_Delay_ms_SDL, ///< The pointer to driver delay function
    D4DLCD_DeInit_SDL, ///< The pointer to driver deinitialization function
};
/*! @} End of doxd4d_lcd_variable                                           */
/**************************************************************/ /*!
  *
  * Local variables
  *
  ******************************************************************/

static SDL_Window* sdl_Window = NULL;
static SDL_Renderer* sdl_Renderer = NULL;

typedef struct sdl_egui_window_t {
    unsigned short x1;
    unsigned short x2;
    unsigned short y1;
    unsigned short y2;
    unsigned short next_x;
    unsigned short next_y;
} sdl_egui_window_t;

static sdl_egui_window_t sdl_egui_window = { 0 };

/**************************************************************/ /*!
  *
  * Functions bodies
  *
  ******************************************************************/
/******************************************************************************
  * Begin of D4D_LCD public functions
  */ /*! @addtogroup doxd4d_lcd_func
  * @{
  *******************************************************************************/

/**************************************************************************/ /*!
  * @brief   The function is used for initialization of this low level driver
  * @return  result: 1 - Success; 0 - Failed
  * @note    This should initilize all neccessary things to run template lcd driver.
  *******************************************************************************/
static unsigned char D4DLCD_Init_SDL(void)
{
    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return 0;
    }

    // Create window
    sdl_Window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, D4D_SCREEN_SIZE_LONGER_SIDE,
        D4D_SCREEN_SIZE_SHORTER_SIDE, SDL_WINDOW_SHOWN);
    if (sdl_Window == NULL) {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return 0;
    }

    // Create renderer for window
    sdl_Renderer = SDL_CreateRenderer(sdl_Window, -1, SDL_RENDERER_ACCELERATED);
    if (sdl_Renderer == NULL) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return 0;
    }

    return 1;
}

/**************************************************************************/ /*!
  * @brief   The function is used for deinitialization of this low level driver
  * @return  result: 1 - Success; 0 - Failed
  * @note    This should deinitilize all neccessary things to run template lcd driver.
  *******************************************************************************/
static unsigned char D4DLCD_DeInit_SDL(void)
{
    //Destroy window
    SDL_DestroyRenderer(sdl_Renderer);
    SDL_DestroyWindow(sdl_Window);
    sdl_Window = NULL;
    sdl_Renderer = NULL;

    //Quit SDL subsystems
    SDL_Quit();
    return 1;
}

/**************************************************************************/ /*!
  * @brief   The function sets the logic drawing window in memory of LCD driver
  * @param   x1 - left cordination of logic window
  * @param   y1 - top cordination of logic window
  * @param   x2 - right cordination of logic window
  * @param   y2 - bottom cordination of logic window
  * @return  result: 1 - Success; 0 - Failed
  * @note    After this function could be write/read pixels to/from LCD panel
  *******************************************************************************/

static unsigned char D4DLCD_SetWindow_SDL(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2)
{
    sdl_egui_window.x1 = x1;
    sdl_egui_window.x2 = x2;
    sdl_egui_window.y1 = y1;
    sdl_egui_window.y2 = y2;

    sdl_egui_window.next_x = x1;
    sdl_egui_window.next_y = y1;
    return 1;
}

/**************************************************************************/ /*!
  * @brief   The function set the new orientation of LCD
  * @param   new_orientation - new orientation of the LCD
  * @return  result: 1 - Success; 0 - Failed
  * @note    This function changes the orientation of LCD
  *******************************************************************************/
static unsigned char D4DLCD_SetOrientation_SDL(D4DLCD_ORIENTATION new_orientation)
{
    return 1;
}

/**************************************************************************/ /*!
  * @brief   The function send the one pixel into LCD drawing window
  * @param   color - value of pixel color
  * @return  none
  * @note    This function writes the one pixel to the opened drawing window by \ref  D4DLCD_SetWindow_SDL function.
  *           If the window is lager then 1 pixel the driver MUST autmatically increment the next pixel address.
  *******************************************************************************/
static void D4DLCD_Send_PixelColor_SDL(D4D_COLOR color)
{
    SDL_SetRenderDrawColor(sdl_Renderer, D4D_COLOR_GET_R(color), D4D_COLOR_GET_G(color), D4D_COLOR_GET_B(color), 0xFF);
    SDL_RenderDrawPoint(sdl_Renderer, sdl_egui_window.next_x, sdl_egui_window.next_y);
    if (++sdl_egui_window.next_x > sdl_egui_window.x2) {
        sdl_egui_window.next_y++;
        sdl_egui_window.next_x = sdl_egui_window.x1;
    }
}

/**************************************************************************/ /*!
  * @brief   The function reads the one Pixel from LCD (if this function is supported)
  * @return  color - value of pixel color
  * @note    This function reads the one pixel from the opened drawing window by \ref  D4DLCD_SetWindow_SDL function.
  *           If the window is lager then 1 pixel the driver MUST autmatically increment the next pixel address.
  *******************************************************************************/
static D4D_COLOR D4DLCD_Read_PixelColor_SDL(void)
{
    return 0;
}

/**************************************************************************/ /*!
  * @brief   For buffered low level interfaces is used to inform
  *            driver the complete object is drawed and pending pixels should be flushed
  * @param   mode - mode of Flush
  * @return  none
  * @note    This function just notify the driver that eGUI finish drawing element/object/screen to allow driver handle any kind of caching memmory.
  *******************************************************************************/
static void D4DLCD_Flush_SDL(D4DLCD_FLUSH_MODE mode)
{
    SDL_RenderPresent(sdl_Renderer);
}

/**************************************************************************/ /*!
  * @brief   For do some small delays in ms
  * @param   period - 1ms periods time
  * @return  none
  * @note    This function is just used to do some delays of eGUI (just for initialization purposes, not for run)
  *******************************************************************************/
static void D4DLCD_Delay_ms_SDL(unsigned short period)
{
    SDL_Delay(100);
}

/*! @} End of doxd4d_lcd_func                                           */

#endif //(D4D_MK_STR(D4D_LLD_LCD) == d4dlcd_template_ID)
