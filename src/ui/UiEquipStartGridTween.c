// bdc 0x0895d918 UiEquipStartGridTween
#include "bdc.h"

/* Starts the zoom tween (in: `out` = 0, out: 1, start scale 1.5) of the four sprite groups of the
   Bakugan grid of the Bakugan/gear loadout screen before a battle (task 302, `UiEquipCtor`),
   via `UiTweenBegin`:
   - the 20 face cells (`spriteIdx[1]`): shown (`flags |= 1`) with their face icon
     (`UiEquipSetFaceTexture`, `"baku_face_%02d"`) only for Bakugan owned in profile
     `bakuganBitsA`;
   - the 20 cells of `spriteIdx[2]`: always shown;
   - the 20 cells of `spriteIdx[3]`: shown only for Bakugan NOT owned (placeholder);
   - the `spriteIdx[0x4c]` cells from `spriteIdx[0x4b]`: shown only for owned Bakugan whose
     "used" bit (`newItemGroups[0xb..]`, see `UiEquipMarkSelectionsUsed`) is still clear.
   Every cell gets its tween started whether or not it was shown. */

void UiEquipStartGridTween(UiEquip *self, u8 out)
{
  SaveProfile *profile;
  GfxSprite *sprite;
  int id;
  int i;

  for (i = self->spriteIdx[1]; i < self->spriteIdx[1] + 20; i++) {
    profile = SaveGetProfile();
    id = UiEquipMapBakuganIndex(self, 0, (u8)(i - self->spriteIdx[1]));
    sprite = ((GfxSprite **)self->base.data)[i];
    if ((u8)(profile->data->bakuganBitsA[id / 8] & (1 << (id % 8))) != 0) {
      sprite->flags |= 1;
      sprite = ((GfxSprite **)self->base.data)[i];
      UiEquipSetFaceTexture(self, sprite, UiEquipMapBakuganIndex(self, 0, (u8)(i - self->spriteIdx[1])));
      sprite = ((GfxSprite **)self->base.data)[i];
    }
    UiTweenBegin(1.5f, out, sprite, &self->tweens[i], 3);
  }

  for (i = self->spriteIdx[2]; i < self->spriteIdx[2] + 20; i++) {
    ((GfxSprite **)self->base.data)[i]->flags |= 1;
    UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
  }

  for (i = self->spriteIdx[3]; i < self->spriteIdx[3] + 20; i++) {
    profile = SaveGetProfile();
    id = UiEquipMapBakuganIndex(self, 0, (u8)(i - self->spriteIdx[3]));
    sprite = ((GfxSprite **)self->base.data)[i];
    if ((u8)(profile->data->bakuganBitsA[id / 8] & (1 << (id % 8))) == 0) {
      sprite->flags |= 1;
      sprite = ((GfxSprite **)self->base.data)[i];
    }
    UiTweenBegin(1.5f, out, sprite, &self->tweens[i], 3);
  }

  for (i = self->spriteIdx[0x4b]; i < self->spriteIdx[0x4b] + self->spriteIdx[0x4c]; i++) {
    profile = SaveGetProfile();
    id = UiEquipMapBakuganIndex(self, 0, (u8)(i - self->spriteIdx[0x4b]));
    if ((u8)(profile->data->bakuganBitsA[id / 8] & (1 << (id % 8))) != 0) {
      profile = SaveGetProfile();
      id = UiEquipMapBakuganIndex(self, 0, (u8)(i - self->spriteIdx[0x4b]));
      if ((u8)(profile->data->newItemGroups[0xb + id / 8] & (1 << (id % 8))) == 0) {
        ((GfxSprite **)self->base.data)[i]->flags |= 1;
      }
    }
    UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
  }
}
