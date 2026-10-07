// bdc 0x088d9828 GameGimmickEnableMaterialCallback
#include "bdc.h"

/* Sets `+0xbc = 1` and installs the gimmick material callback `0x088d8f84` with
   `GfxModelForEachMaterial`. Called by several gimmick constructors. */

void GameGimmickEnableMaterialCallback(GameGimmick *gimmick)

{
  (gimmick->base).lighting = '\x01';
  GfxModelForEachMaterial(&gimmick->base,GameGimmickMaterialSetBlendMode1,(void *)0x0);
  return;
}

