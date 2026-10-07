// bdc 0x089406c4 UiScreen390ClearTerminalEffect
#include "bdc.h"

/* Removes the effects attached to the terminal object `0xcf + +0x80` of
   `UiScreen390` (`ActorStageObjRecordStopEffects`). */

void UiScreen390ClearTerminalEffect(UiScreen *screen)

{
  ActorStageObjRecordStopEffects(((UiScreen390 *)screen)->terminal + 0xcf);
  return;
}

