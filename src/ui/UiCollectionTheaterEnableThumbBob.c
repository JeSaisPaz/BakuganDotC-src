// bdc 0x089894b8 UiCollectionTheaterEnableThumbBob
#include "bdc.h"

/* Resets the thumbnail bob record of `UiCollectionTheater` (`+0x918`,
   0x0c bytes) and enables (1) or disables it. */

void UiCollectionTheaterEnableThumbBob(UiScreen *screen, u8 enable)

{
  UiCollectionTheater *self = (UiCollectionTheater *)screen;

  memset(&self->thumbBobOn,0,0xc);
  self->thumbBobOn = enable;
  return;
}

