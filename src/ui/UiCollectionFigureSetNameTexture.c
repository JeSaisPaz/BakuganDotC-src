// bdc 0x0898c7b0 UiCollectionFigureSetNameTexture
#include "bdc.h"

/* Sets a name-label sprite of `UiCollectionFigure` to the name of figure
   `id`: `"cha_spherename_colle_%02d"` at full tint, or `"collection_special_15"` (placeholder) at
   half tint when `id` is 0; alpha reset to 0 for the following tween. */

void UiCollectionFigureSetNameTexture(UiCollectionFigure *self, GfxSprite *sprite, u8 id)

{
  char name [64];
  
  if (id == 0) {
    sprintf(name,"collection_special_15");
    sprite->alpha = 0.0f;
    sprite->tint[0] = 0.5f;
    sprite->tint[1] = 0.5f;
    sprite->tint[2] = 0.5f;
  }
  else {
    sprintf(name,"cha_spherename_colle_%02d",id);
    sprite->alpha = 0.0f;
    sprite->tint[0] = 1.0f;
    sprite->tint[1] = 1.0f;
    sprite->tint[2] = 1.0f;
  }
  sprite->texture = GfxFindTexture(name);
}

