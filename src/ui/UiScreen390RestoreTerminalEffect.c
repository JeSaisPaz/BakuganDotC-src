// bdc 0x08940924 UiScreen390RestoreTerminalEffect
#include "bdc.h"

/* Re-attaches effect 2 to the terminal object `0xcf + +0x80` of `UiScreen390`
   (`ActorStageObjRecordSpawnEffect(id, 2)`). */

void UiScreen390RestoreTerminalEffect(UiScreen *screen)

{
  ActorStageObjRecordSpawnEffect(((UiScreen390 *)screen)->terminal + 0xcf,2);
  return;
}

