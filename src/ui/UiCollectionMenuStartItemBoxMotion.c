// bdc 0x089762e0 UiCollectionMenuStartItemBoxMotion
#include "bdc.h"

/* Activates the item-box model of `UiCollectionMenu` (`+0x520`) and plays
   its open (0) or close (1) motion. */

void UiCollectionMenuStartItemBoxMotion(UiCollectionMenu *self, u8 closing)

{
  if (closing == '\0') {
    self->itemBoxClosing = '\x01';
    UiCollectionMenuPlayItemBoxMotion(self,'\0');
    return;
  }
  self->itemBoxClosing = '\x01';
  UiCollectionMenuPlayItemBoxMotion(self,'\x01');
  return;
}

