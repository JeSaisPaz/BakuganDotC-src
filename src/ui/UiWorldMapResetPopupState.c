// bdc 0x0899c4e8 UiWorldMapResetPopupState
#include "bdc.h"

/* Clears the 0x14-byte state block `+0x10f8` of `UiWorldMap` before one of its
   pop-up sequences (main-phase steps 0xc and 0x18, which then run `UiWorldMapAreaRoulette` / `UiWorldMapStageRoulette`).
    */

void UiWorldMapResetPopupState(UiScreen *screen)
{
  memset(((UiWorldMap *)screen)->popupState, 0, 0x14);
}
