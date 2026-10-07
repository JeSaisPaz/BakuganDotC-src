// bdc 0x089176a4 UiAdvSelectInitState
#include "bdc.h"

/* Initialises the selection state of the adventure partner-select screen (`UiAdvSelectCtor`, task
   376; cursor `+0x74`, candidates `+0x8a0` as 4-byte `{id, partner, locked, ?}` slots): builds the
   candidates (`UiAdvSelectBuildCandidates`), puts the cursor `+0x74` on the first unlocked one
   (`UiAdvSelectFirstUnlocked`), clears the camera/model pointers `+0x910..+0x918` and the
   animation records `+0x8b8..+0x8db`. */

void UiAdvSelectInitState(UiAdvSelect *self)

{
  UiAdvSelectBuildCandidates(self);
  self->cursor = (s8)UiAdvSelectFirstUnlocked(self);
  self->camera = (void *)0x0;
  self->model = (void *)0x0;
  self->pedestal = (void *)0x0;
  memset(&self->blinkAOn,0,0x24);
  return;
}

