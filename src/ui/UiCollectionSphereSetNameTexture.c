// bdc 0x0897a4a8 UiCollectionSphereSetNameTexture
#include "bdc.h"

/* Sets a Bakugan name sprite of `UiCollectionSphere` to
   `"cha_spherename_colle_%02d"` for entry id `id` (ids 4/16/19 map to 35/33/34), or to the grey
   placeholder `"collection_special_15"` when `id` = 0. */

void UiCollectionSphereSetNameTexture(UiCollectionSphere *self, GfxSprite *sprite, u8 id)

{
  char name[64];
  
  if (id == 0) {
    sprintf(name,"collection_special_15");
    sprite->alpha = 0.0f;
    sprite->tint[0] = 0.5f;
    sprite->tint[1] = 0.5f;
    sprite->tint[2] = 0.5f;
  }
  else {
    if (id == 0x13) {
      sprintf(name,"cha_spherename_colle_%02d",0x22);
    }
    else if (id == 0x10) {
      sprintf(name,"cha_spherename_colle_%02d",0x21);
    }
    else if (id == 4) {
      sprintf(name,"cha_spherename_colle_%02d",0x23);
    }
    else {
      sprintf(name,"cha_spherename_colle_%02d",id);
    }
    sprite->alpha = 0.0f;
    sprite->tint[0] = 1.0f;
    sprite->tint[1] = 1.0f;
    sprite->tint[2] = 1.0f;
  }
  sprite->texture = GfxFindTexture(name);
}

