// bdc 0x089152b0 UiUpgradeSetupPhase
#include "bdc.h"

/* Phase 1 of the upgrade screen. Step 0 clears the 103 tweens, creates the layout-0x37 sprites
   (`UiLayoutCreateSprites`) into the sprite array `base.data`, allocates a highlight sprite
   (slot 102) copied from slot 53, derives the scissor rectangle from panel sprite 11, centres
   and resets all 103 sprites (saving their Z/Y in `spriteZ`/`spriteY`), copies the row Y of
   the first column of the grids at 44/54/60/66 and 72..76 down to rows 1..5, starts the
   `"up_grade.fab"` background animation (`UiScreenAnimStart`) at its last frame and falls
   into step 1. Step 1 waits until the loading screen is closed. Any other step starts a
   10-frame fade from black and advances to the next phase. */

void UiUpgradeSetupPhase(UiUpgrade *self)
{
  GfxSprite **sprites;
  GfxSprite *highlight;
  GfxSprite *mem;
  GfxFader *fader;
  GfxFab *fab;
  bool fromLow;
  float size;
  int i;
  int j;

  switch (self->base.phaseStep) {
  case 0:
    memset(self->tweens, 0, sizeof(self->tweens));
    UiLayoutCreateSprites(self->base.spriteLayer, (GfxSprite **)self->base.data, 0x37);
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(sizeof(GfxSprite), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    highlight = NULL;
    if (mem != NULL) {
      GfxSpriteCtor(mem);
      highlight = mem;
    }
    ((GfxSprite **)self->base.data)[102] = highlight;
    GfxSpriteLayerAdd(self->base.spriteLayer,
                      (CoreObject *)((GfxSprite **)self->base.data)[102]);
    sprites = (GfxSprite **)self->base.data;
    GfxSpriteCopy(sprites[53], sprites[102]);
    ((GfxSprite **)self->base.data)[102]->flags &= ~1u;

    sprites = (GfxSprite **)self->base.data;
    self->scissorX = (int)(sprites[11]->posX + 10.0f);
    self->scissorY = (int)(sprites[11]->posY + 12.0f);
    size = GfxSpriteGetWidth(sprites[11]);
    self->scissorW = (int)(size - 20.0f);
    size = GfxSpriteGetHeight(((GfxSprite **)self->base.data)[11]);
    self->scissorH = (int)(size - 24.0f);

    for (i = 0; i < 103; i++) {
      ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
      GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[i]);
      GfxSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f, false);
      GfxSpriteInsetUv(0.5f, ((GfxSprite **)self->base.data)[i]);
      ((GfxSprite **)self->base.data)[i]->alpha = 0.0f;
      UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
      sprites = (GfxSprite **)self->base.data;
      self->spriteZ[i] = sprites[i]->posZ;
      self->spriteY[i] = sprites[i]->posY;
    }

    for (i = 1; i < 6; i++) {
      sprites = (GfxSprite **)self->base.data;
      sprites[44 + i]->posY = sprites[44]->posY;
      sprites = (GfxSprite **)self->base.data;
      sprites[54 + i]->posY = sprites[54]->posY;
      sprites = (GfxSprite **)self->base.data;
      sprites[60 + i]->posY = sprites[60]->posY;
      sprites = (GfxSprite **)self->base.data;
      sprites[66 + i]->posY = sprites[66]->posY;
      for (j = 0; j < 5; j++) {
        sprites = (GfxSprite **)self->base.data;
        sprites[72 + i * 5 + j]->posY = sprites[72 + j]->posY;
      }
    }

    UiScreenAnimStart(&self->base, "up_grade.fab", 0, 0, 1000.0f, -24.0f, 0.0f);
    fab = ((GfxFab **)self->base.bgData)[0];
    GfxFabAdvanceTo(fab, GfxFabGetFrameCount(fab));
    self->base.phaseStep++;
    /* fall through */
  case 1:
    if (!UiLoadingIsOpen()) {
      self->base.phaseStep++;
    }
    return;
  default:
    fader = GfxGetActiveFader();
    fader->start[0] = 0.0f;
    fader->start[1] = 0.0f;
    fader->start[2] = 0.0f;
    fader->start[3] = 1.0f;
    fader = GfxGetActiveFader();
    fader->end[0] = 0.0f;
    fader->end[1] = 0.0f;
    fader->end[2] = 0.0f;
    fader->end[3] = 0.0f;
    fader = GfxGetActiveFader();
    GfxFaderStart(fader, 10);
    self->base.phaseStep = 0;
    self->base.phase++;
    return;
  }
}
