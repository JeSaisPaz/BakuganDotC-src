// bdc 0x0891463c UiUpgradeSetSlotFrameLit
#include "bdc.h"

/* Sets the texture of a slot frame sprite: `up_waku_02` when `lit`, else `up_waku_01`. */

void UiUpgradeSetSlotFrameLit(UiUpgrade *self, GfxSprite *sprite, bool lit)

{
  void *tex;
  char name[64];
  
  if (lit) {
    sprintf(name,"up_waku_02");
  }
  else {
    sprintf(name,"up_waku_01");
  }
  tex = GfxFindTexture(name);
  sprite->texture = tex;
  return;
}

