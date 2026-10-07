// bdc 0x08a2bfa8 GameGimmickCorePointIsHiddenOnRadar
#include "bdc.h"

/* Vtable slot 17 of the core-point gimmick (`GameGimmickCorePointCtor`, vtable `0x08af234c`):
   returns the byte `+0x1a8`, which hides the core point from the field radar
   (`UiFieldHudUpdateRadar`) when set. */

u8 GameGimmickCorePointIsHiddenOnRadar(GameGimmickCorePoint *gimmick)

{
  return gimmick->invisible;
}

