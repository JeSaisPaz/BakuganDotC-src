// bdc 0x089831f0 UiCollectionCardSetCardLabel
#include "bdc.h"

/* Sets a card label sprite of `UiCollectionCard` to
   `"collection_ability_%02d"` (card id + 1), or to the grey placeholder `"collection_special_15"`
   for an empty slot (0xff). */

void UiCollectionCardSetCardLabel(UiCollectionCard *self, GfxSprite *sprite, u8 cardId)

{
  char name [64];
  
  if (cardId == 0xff) {
    sprintf(name,"collection_special_15");
    sprite->alpha = 0.0f;
    sprite->tint[0] = 0.5f;
    sprite->tint[1] = 0.5f;
    sprite->tint[2] = 0.5f;
  }
  else {
    sprintf(name,"collection_ability_%02d",cardId + 1);
    sprite->alpha = 0.0f;
    sprite->tint[0] = 1.0f;
    sprite->tint[1] = 1.0f;
    sprite->tint[2] = 1.0f;
  }
  sprite->texture = GfxFindTexture(name);
}

