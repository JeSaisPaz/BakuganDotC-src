// bdc 0x08988388 UiCollectionTheaterPhaseClose
#include "bdc.h"

/* Last phase (slot `0x08a9e94c` of table `0x08a9e8d0`) of the UiCollectionTheater collection
   screen: requests the screen's close (`closeRequested`, `+0x4c` = 1). */

void UiCollectionTheaterPhaseClose(UiScreen *screen)

{
  screen->closeRequested = '\x01';
  return;
}

