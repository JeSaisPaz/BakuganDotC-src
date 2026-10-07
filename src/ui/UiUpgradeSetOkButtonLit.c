// bdc 0x089145e0 UiUpgradeSetOkButtonLit
#include "bdc.h"

/* Sets the texture of the OK button sprite `sprite` (`+0xd4`): `c_set_OK_bo_1` when `lit`, else
   `c_set_OK_bo_2`. */

void UiUpgradeSetOkButtonLit(UiUpgrade *self, GfxSprite *sprite, bool lit)

{
  void *tex;
  char name[64];
  
  if (lit) {
    sprintf(name,"c_set_OK_bo_1");
  }
  else {
    sprintf(name,"c_set_OK_bo_2");
  }
  tex = GfxFindTexture(name);
  sprite->texture = tex;
  return;
}

