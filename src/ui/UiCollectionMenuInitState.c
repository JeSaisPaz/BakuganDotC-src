// bdc 0x08973594 UiCollectionMenuInitState
#include "bdc.h"

/* Class init of `UiCollectionMenu` called by `UiCollectionMenuCtor`:
   clears the per-page selections `+0x500/+0x501`, page `+0x503`, move direction and model-motion
   fields, builds the entry masks (`UiCollectionMenuInitEntryMasks`) and clears the item-box model
   pointer `+0x51c` and the light-blink record `+0x754`. */

void UiCollectionMenuInitState(UiCollectionMenu *self)
{
  s32 i;

  for (i = 0; i < 2; i++) {
    (&self->selMain)[i] = 0;
  }
  self->page = 0;
  self->moveDir = 0;
  self->motion = 0;
  self->refresh = 0;
  UiCollectionMenuInitEntryMasks(self);
  self->itemBox = NULL;
  self->itemBoxClosing = 0;
  self->itemBoxOpen = 0;
  memset(&self->lightBlink, 0, 0xc);
}
