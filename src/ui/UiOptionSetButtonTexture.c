// bdc 0x08971468 UiOptionSetButtonTexture
#include "bdc.h"

/* Sets an OK/Defaults button texture of `UiOption`: `"c_set_OK_bo_1"` when selected,
   `"c_set_OK_bo_2"` otherwise. */

void UiOptionSetButtonTexture(UiOption *self, GfxSprite *sprite, u8 selected)

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

