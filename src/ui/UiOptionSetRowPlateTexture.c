// bdc 0x0897140c UiOptionSetRowPlateTexture
#include "bdc.h"

/* Sets a row plate texture of `UiOption`: `"option_sol00"` when selected,
   `"option_sol01"` otherwise. */

void UiOptionSetRowPlateTexture(UiOption *self, GfxSprite *sprite, u8 selected)

{
  void *tex;
  char name [64];
  
  if (selected == '\0') {
    sprintf(name,"option_sol01");
  }
  else {
    sprintf(name,"option_sol00");
  }
  tex = GfxFindTexture(name);
  sprite->texture = tex;
  return;
}

