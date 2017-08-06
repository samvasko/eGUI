#include "d4d.h"
#include "d4d_screen_log.h"
#include "fonts.h"
#include "pictures.h"
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "d4d.h"
#include "common_files/d4d_lldapi.h"

D4D_EXTERN_SCREEN(screen_about)

int main(void) {
  printf("Hello from eGUI demo\n\r");

  // Init eGUI
  if (!D4D_Init(&screen_about)) {
    // D4D initialization failed
    printf("eGUI/D4D initialization failed\n\r");
  }

  // Setup right screen orientation
  // D4D_SetOrientation(D4D_ORIENT_LANDSCAPE);

  // Force calibration screen
  // D4D_CalibrateTouchScreen();

  while (true) {
    D4D_Poll();
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
      // User requests quit
      if (e.type == SDL_QUIT) {
        // Quit SDL subsystems
        D4D_LLD_LCD.D4DLCD_DeInit();
        return 0;
      }
    }
  }
}

/* EOF */
