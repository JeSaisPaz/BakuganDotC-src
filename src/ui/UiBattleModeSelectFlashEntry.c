// bdc 0x089b1040 UiBattleModeSelectFlashEntry
#include "bdc.h"

/* Starts the 2-frame-rate confirm flash (`UiFlashStart` speed 2.0) on the selected entry panel of
   `UiBattleModeSelect` (sprite 4 + cursor `+0x74`). */

void UiBattleModeSelectFlashEntry(UiBattleModeSelect *self)

{
  UiFlashStart(2.0f, ((GfxSprite **)self->base.data)[4 + self->cursor], 0, 0);
  return;
}

