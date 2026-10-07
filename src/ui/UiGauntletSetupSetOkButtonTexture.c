// bdc 0x089342d0 UiGauntletSetupSetOkButtonTexture
#include "bdc.h"

/* Sets the OK-button sprite's texture (`+0xd4`) of `UiGauntletSetup`:
   `"c_set_OK_bo_1"` when `lit` is non-zero, else `"c_set_OK_bo_2"` (`GfxFindTexture`). */

void UiGauntletSetupSetOkButtonTexture(UiGauntletSetup *self, GfxSprite *sprite, u8 lit)

{
  void *tex;
  char name [64];
  
  if (lit == '\0') {
    sprintf(name,"c_set_OK_bo_2");
  }
  else {
    sprintf(name,"c_set_OK_bo_1");
  }
  tex = GfxFindTexture(name);
  sprite->texture = tex;
  return;
}

