// bdc 0x08988394 UiCollectionTheaterSetResultNone
#include "bdc.h"

/* Sets menu result 0 for `UiCollectionTheater` (`UiSetMenuResult`) when
   it exits. */

void UiCollectionTheaterSetResultNone(UiScreen *screen)

{
  UiSetMenuResult(screen,0);
  return;
}

