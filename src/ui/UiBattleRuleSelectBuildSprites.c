// bdc 0x0895288c UiBattleRuleSelectBuildSprites
#include "bdc.h"

/* Builds the screen of `UiBattleRuleSelect`: preselects the saved rule
   (`UiBattleRuleSelectGetSavedRule` into `cursor`), sets the enabled flags
   (`UiBattleRuleSelectInitEnabled`), clears the tweens (0x500 bytes), creates the sprites of
   layout 0x15 into `base.data`, hides the first 0x1c with alpha 0 saving their depths in
   `spriteHomeZ`, insets the UVs of sprites 0x15–0x17 by half a texel (`GfxSpriteInsetUv`), and
   creates sprite 0x1c as a hidden copy of the cursor sprite 0x15 (new 0x160-byte
   `GfxSprite` allocated from low memory, added to the layer, `GfxSpriteCopy`);
   sprite 0x1c is the pulse overlay passed to `UiPulseStep`. */

void UiBattleRuleSelectBuildSprites(UiBattleRuleSelect *self)
{
  bool fromLow;
  GfxSprite *sprite;
  GfxSprite *mem;
  s32 i;

  self->cursor = (s8)UiBattleRuleSelectGetSavedRule();
  UiBattleRuleSelectInitEnabled(self);
  memset(self->tweens, 0, 0x500);
  UiLayoutCreateSprites(self->base.spriteLayer, self->base.data, 0x15);
  for (i = 0; i < 0x1c; i++) {
    ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
    ((GfxSprite **)self->base.data)[i]->alpha = 0.0f;
    self->spriteHomeZ[i] = ((GfxSprite **)self->base.data)[i]->posZ;
  }
  for (i = 0x15; i < 0x18; i++) {
    GfxSpriteInsetUv(0.5f, ((GfxSprite **)self->base.data)[i]);
  }
  sprite = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(0x160, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (mem != NULL) {
    GfxSpriteCtor(mem);
    sprite = mem;
  }
  ((GfxSprite **)self->base.data)[0x1c] = sprite;
  GfxSpriteLayerAdd(self->base.spriteLayer, (CoreObject *)((GfxSprite **)self->base.data)[0x1c]);
  GfxSpriteCopy(((GfxSprite **)self->base.data)[0x15], ((GfxSprite **)self->base.data)[0x1c]);
  ((GfxSprite **)self->base.data)[0x1c]->flags &= ~1u;
}
