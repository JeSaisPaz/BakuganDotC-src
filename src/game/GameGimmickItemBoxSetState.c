// bdc 0x088d76c4 GameGimmickItemBoxSetState
#include "bdc.h"

/* Sets the state `+0x16c` of the item box (breakable obstacle) gimmick (`GameGimmickItemBoxCtor`,
   vtables `0x08af30f4`/`0x08af319c`) and clears its step `+0x180`. */

void GameGimmickItemBoxSetState(GameGimmickItemBox *obj, s32 state)

{
  (obj->base).state = state;
  obj->step = 0;
  return;
}

