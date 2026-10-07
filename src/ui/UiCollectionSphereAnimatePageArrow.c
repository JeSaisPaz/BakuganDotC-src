// bdc 0x0897c26c UiCollectionSphereAnimatePageArrow
#include "bdc.h"

/* Plays the page-change arrow animation of the sphere (Bakugan figure) collection screen (task 312,
   `maybe_UiScreen312Ctor`; 3x2 grid pages of collected Bakugan shown as 3D models
   (`"00_P_Dragonoid_N_P.gmo"`…), names `"cha_spherename_colle_%02d"`, pop-out motions
   (`"00_dor_dir_popout"`…); cursor `+0xee0`, page `+0xee1`, category `+0xee5`, entry lists
   `+0x1250`). Steps `UiFlashStep(0)`; the three arrow sprites of side `pageDir` are
   `data[18 + pageDir*3 + i]`. While the flash runs: the third sprite is tinted 0.3 (alpha 1),
   each sprite's scaleX grows by 0.1 with scaleY = 2*scaleX, angle 0; returns 0. Once the flash
   completes: tint and alpha reset to 1, scale 1 x 2, angle 0; returns 1. */

#define ARROW_SPRITE(self, i) (((GfxSprite **)(self)->base.data)[(self)->pageDir * 3 + (i) + 18])

s32 UiCollectionSphereAnimatePageArrow(UiCollectionSphere *self)
{
  int done;
  int i;
  GfxSprite *sprite;
  GfxSprite *arrow;

  done = UiFlashStep(0);
  arrow = ARROW_SPRITE(self, 2);
  if (done != 0) {
    arrow->tint[0] = 1.0f;
    arrow->tint[1] = 1.0f;
    arrow->tint[2] = 1.0f;
    arrow->alpha = 1.0f;
    for (i = 0; i < 3; i++) {
      ARROW_SPRITE(self, i)->scaleX = 1.0f;
      ARROW_SPRITE(self, i)->scaleY = 2.0f;
      ARROW_SPRITE(self, i)->angle = 0.0f;
      sprite = ARROW_SPRITE(self, i);
      GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
    }
    return 1;
  }
  arrow->alpha = 1.0f;
  arrow->tint[0] = 0.3f;
  arrow->tint[1] = 0.3f;
  arrow->tint[2] = 0.3f;
  for (i = 0; i < 3; i++) {
    ARROW_SPRITE(self, i)->scaleX = ARROW_SPRITE(self, i)->scaleX + 0.1f;
    sprite = ARROW_SPRITE(self, i);
    sprite->scaleY = sprite->scaleX * 2.0f;
    ARROW_SPRITE(self, i)->angle = 0.0f;
    sprite = ARROW_SPRITE(self, i);
    GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
  }
  return 0;
}
