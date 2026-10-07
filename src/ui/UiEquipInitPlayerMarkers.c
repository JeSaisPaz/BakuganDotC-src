// bdc 0x08958d98 UiEquipInitPlayerMarkers
#include "bdc.h"

/* Sets up the three per-player marker sprites of the Bakugan/gear loadout screen (task 302,
   `UiEquipCtor`): stores their sprite indices in markerBase/markerSpriteA/markerSpriteB
   (0x6f+p, 0x82+p, 0x80+p for the 2-player layout, filling 2 slots; 0x80+p, 0xa5+p, 0xa1+p for
   4 players, filling 4) and insets each one's UVs by half a texel. Then, for every player, it
   centres each marker's pivot, resets its scale to 1 and rotation to 0, and records the base
   marker position in markerBasePos[p] and the A/B markers' offsets from it in
   markerOfsA[p]/markerOfsB[p]. */

#define UIEQ_SPRITE(self, n) (((GfxSprite **)(self)->base.data)[(n)])

void UiEquipInitPlayerMarkers(UiEquip *self)
{
  GfxSprite **slot;
  GfxSprite **baseSlot;
  int i;

  if (self->playerCount < 3) {
    for (i = 0; i < 2; i++) {
      self->markerBase[i] = (u8)(i + 0x6f);
      self->markerSpriteA[i] = (u8)(i + 0x82);
      self->markerSpriteB[i] = (u8)(i + 0x80);
      GfxSpriteInsetUv(0.5f, UIEQ_SPRITE(self, self->markerBase[i]));
      GfxSpriteInsetUv(0.5f, UIEQ_SPRITE(self, self->markerSpriteA[i]));
      GfxSpriteInsetUv(0.5f, UIEQ_SPRITE(self, self->markerSpriteB[i]));
    }
  } else {
    for (i = 0; i < 4; i++) {
      self->markerBase[i] = (u8)(i + 0x80);
      self->markerSpriteA[i] = (u8)(i + 0xa5);
      self->markerSpriteB[i] = (u8)(i + 0xa1);
      GfxSpriteInsetUv(0.5f, UIEQ_SPRITE(self, self->markerBase[i]));
      GfxSpriteInsetUv(0.5f, UIEQ_SPRITE(self, self->markerSpriteA[i]));
      GfxSpriteInsetUv(0.5f, UIEQ_SPRITE(self, self->markerSpriteB[i]));
    }
  }

  for (i = 0; i < self->playerCount; i++) {
    GfxSpriteCenterPivot(UIEQ_SPRITE(self, self->markerBase[i]));
    UiSpriteSetScaleRotation(UIEQ_SPRITE(self, self->markerBase[i]), 1.0f, 1.0f, 0.0f);
    slot = &UIEQ_SPRITE(self, self->markerBase[i]);
    self->markerBasePos[i][0] = (*slot)->posX;
    self->markerBasePos[i][1] = (*slot)->posY;
  }

  for (i = 0; i < self->playerCount; i++) {
    GfxSpriteCenterPivot(UIEQ_SPRITE(self, self->markerSpriteA[i]));
    UiSpriteSetScaleRotation(UIEQ_SPRITE(self, self->markerSpriteA[i]), 1.0f, 1.0f, 0.0f);
    slot = &UIEQ_SPRITE(self, self->markerSpriteA[i]);
    baseSlot = &UIEQ_SPRITE(self, self->markerBase[i]);
    self->markerOfsA[i][0] = (*slot)->posX - (*baseSlot)->posX;
    self->markerOfsA[i][1] = (*slot)->posY - (*baseSlot)->posY;
  }

  for (i = 0; i < self->playerCount; i++) {
    GfxSpriteCenterPivot(UIEQ_SPRITE(self, self->markerSpriteB[i]));
    UiSpriteSetScaleRotation(UIEQ_SPRITE(self, self->markerSpriteB[i]), 1.0f, 1.0f, 0.0f);
    slot = &UIEQ_SPRITE(self, self->markerSpriteB[i]);
    baseSlot = &UIEQ_SPRITE(self, self->markerBase[i]);
    self->markerOfsB[i][0] = (*slot)->posX - (*baseSlot)->posX;
    self->markerOfsB[i][1] = (*slot)->posY - (*baseSlot)->posY;
  }
}
