// bdc 0x0897a598 UiCollectionSphereSetSpecialNameTexture
#include "bdc.h"

/* Like `UiCollectionSphereSetNameTexture` for category 2: on special pages uses
   `"collection_special_%02d"` (id + page/3), else `"cha_spherename_colle_%02d"`; id 0 = grey
   placeholder. */

void UiCollectionSphereSetSpecialNameTexture(UiCollectionSphere *self, GfxSprite *sprite, u8 id)
{
  char name[64];

  if (id == 0) {
    sprintf(name, "collection_special_15");
    sprite->alpha = 0.0f;
    sprite->tint[0] = 0.5f;
    sprite->tint[1] = 0.5f;
    sprite->tint[2] = 0.5f;
  } else {
    if (UiCollectionSphereGetPageKind(self, self->page) == 0) {
      sprintf(name, "collection_special_%02d", id + self->page / 3);
    } else {
      sprintf(name, "cha_spherename_colle_%02d", id);
    }
    sprite->alpha = 0.0f;
    sprite->tint[0] = 1.0f;
    sprite->tint[1] = 1.0f;
    sprite->tint[2] = 1.0f;
  }
  sprite->texture = GfxFindTexture(name);
}
