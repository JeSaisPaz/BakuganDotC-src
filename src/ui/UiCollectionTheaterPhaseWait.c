// bdc 0x08987f80 UiCollectionTheaterPhaseWait
#include "bdc.h"

/* First phase (phase list at slot `0x08a9e92c` of table `0x08a9e8d0`) of the UiCollectionTheater
   collection screen: waits one frame (`+0x2c`), then advances the phase `+0x28` to
   `UiCollectionTheaterPhaseLoad`. */

void UiCollectionTheaterPhaseWait(UiScreen *screen)

{
  if (screen->phaseStep == 0) {
    screen->phaseStep = 1;
    return;
  }
  screen->phaseStep = 0;
  screen->phase = screen->phase + 1;
  return;
}

