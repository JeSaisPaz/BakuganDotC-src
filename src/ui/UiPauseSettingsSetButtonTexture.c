// bdc 0x089ac268 UiPauseSettingsSetButtonTexture
#include "bdc.h"

/* Sets a button sprite's texture to `c_set_OK_bo_1` (selected) or `c_set_OK_bo_2` (unselected). */

void UiPauseSettingsSetButtonTexture(UiPauseSettings *self, GfxSprite *sprite, u8 selected)

{
  void *tex;
  char name [64];
  
  if (selected == '\0') {
    sprintf(name,"c_set_OK_bo_2");
  }
  else {
    sprintf(name,"c_set_OK_bo_1");
  }
  tex = GfxFindTexture(name);
  sprite->texture = tex;
  return;
}

