// bdc 0x08988190 UiCollectionTheaterPhaseLoad
#include "bdc.h"

/* Phase 1 of `UiCollectionTheater`: creates the sprites
   (`UiCollectionTheaterCreateSprites`) in step 0 and advances to the main phase on the next
   frame. */

void UiCollectionTheaterPhaseLoad(UiScreen *screen)

{
  if (screen->phaseStep == 0) {
    UiCollectionTheaterCreateSprites(screen);
    screen->phaseStep = screen->phaseStep + 1;
  }
  else {
    screen->phaseStep = 0;
    screen->phase = screen->phase + 1;
  }
  return;
}

