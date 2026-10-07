// bdc 0x088daf90 GameGimmickSolidDisable
#include "bdc.h"

/* Vtable `0x08af35d4` slot 16: when active (`+0x15e`), clears the flag and switches the model to
   the gimmick material callback (`GameGimmickEnableMaterialCallback`). */

void GameGimmickSolidDisable(GameGimmick *gimmick)

{
  if (gimmick->active != '\0') {
    gimmick->active = '\0';
    GameGimmickEnableMaterialCallback(gimmick);
  }
  return;
}

