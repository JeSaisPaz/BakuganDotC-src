// bdc 0x088d23b4 UiFieldHudStage32Phase
#include "bdc.h"

/* Phase 3 (per-frame running phase) of the field HUD (`UiFieldHudCtor`; sprite array `base.data`):
   stores the normalised XZ direction of the active camera in `viewDir`; shows sprite 0x2a while the
   player has a talk target or trigger in reach, 0x45 while a trigger is in reach and 0x26 while a talk
   target is in reach. On stage 0x20 (script global 1) it hides every other sprite 0..0x47 and runs
   `UiFieldHudShowPromptA` only. Otherwise, when the player can throw, it swaps sprites
   0x46/0x47/0x2a vs 0x27/0x28/0x1e on `throwLocked`, runs the marker/arrow/radar updates, shows the
   profile's `fieldCounter` as one or two digit cells (sprites 9 and 10), then prompt B, the point
   counter, both gauge panels and the fader.
   A zero-length camera direction gives a zero `viewDir`. */

void UiFieldHudStage32Phase(UiFieldHud *self)
{
  float dirX;
  float dirZ;
  float lenSq;
  float scale;
  GfxSprite **sprites;
  GfxSprite *sprite;
  int count;
  int digit;
  int i;

  /* viewDir = normalize(camera dir with y = 0), each lane saturated to [-1,1] */
  dirX = g_gfxActiveCamera->dir[0];
  dirZ = g_gfxActiveCamera->dir[2];
  lenSq = dirX * dirX + 0.0f * 0.0f + dirZ * dirZ;
  scale = VfRsq(lenSq);
  if (lenSq == 0.0f) {
    scale = 0.0f;
  }
  self->viewDir[0] = VfSat1(dirX * scale);
  self->viewDir[1] = VfSat1(dirZ * scale);

  if (ActorPlayerGetByte3ad(self->player) != 0 || ActorPlayerGetByte3ae(self->player) != 0) {
    ((GfxSprite **)self->base.data)[0x2a]->flags |= 1;
  } else {
    ((GfxSprite **)self->base.data)[0x2a]->flags &= ~1u;
  }
  if (ActorPlayerGetByte3ae(self->player) != 0) {
    ((GfxSprite **)self->base.data)[0x45]->flags |= 1;
  } else {
    ((GfxSprite **)self->base.data)[0x45]->flags &= ~1u;
  }
  if (ActorPlayerGetByte3ad(self->player) != 0) {
    ((GfxSprite **)self->base.data)[0x26]->flags |= 1;
  } else {
    ((GfxSprite **)self->base.data)[0x26]->flags &= ~1u;
  }

  if (g_scriptGlobalVars[1] == 0x20) {
    for (i = 0; i < 0x48; i++) {
      if (i != 0x2a && i != 0x26 && i != 0x45) {
        ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
      }
    }
    UiFieldHudShowPromptA(self);
    return;
  }

  if (ActorPlayerCanThrow(self->player)) {
    if (self->player->throwLocked != 0) {
      ((GfxSprite **)self->base.data)[0x46]->flags |= 1;
      ((GfxSprite **)self->base.data)[0x27]->flags &= ~1u;
      ((GfxSprite **)self->base.data)[0x47]->flags |= 1;
      ((GfxSprite **)self->base.data)[0x2a]->flags |= 1;
      ((GfxSprite **)self->base.data)[0x28]->flags &= ~1u;
      ((GfxSprite **)self->base.data)[0x1e]->flags &= ~1u;
    } else {
      ((GfxSprite **)self->base.data)[0x46]->flags &= ~1u;
      ((GfxSprite **)self->base.data)[0x27]->flags |= 1;
      ((GfxSprite **)self->base.data)[0x47]->flags &= ~1u;
      ((GfxSprite **)self->base.data)[0x28]->flags |= 1;
      ((GfxSprite **)self->base.data)[0x1e]->flags |= 1;
    }
  }
  UiFieldHudUpdatePlayerMarker(self);
  UiFieldHudUpdateHeadingArrow(self);
  UiFieldHudUpdateRadar(self);

  count = SaveGetProfile()->data->fieldCounter;
  if (count < 10) {
    GfxSpriteSetCell(((GfxSprite **)self->base.data)[10], (float)(count / 5), (float)(count % 5));
    sprites = (GfxSprite **)self->base.data;
    sprites[10]->posX = (sprites[9]->posX + 6.0f) - 2.0f;
    ((GfxSprite **)self->base.data)[9]->flags &= ~1u;
  } else {
    digit = count / 10;
    GfxSpriteSetCell(((GfxSprite **)self->base.data)[9], (float)(digit / 5), (float)(digit % 5));
    digit = count % 10;
    GfxSpriteSetCell(((GfxSprite **)self->base.data)[10], (float)(digit / 5), (float)(digit % 5));
    sprites = (GfxSprite **)self->base.data;
    sprites[10]->posX = sprites[9]->posX + 12.0f;
    sprite = ((GfxSprite **)self->base.data)[9];
    sprite->flags |= 1;
  }
  UiFieldHudShowPromptB(self);
  UiFieldHudUpdateCounters(self);
  UiFieldHudUpdateGaugePanelA(self);
  UiFieldHudUpdateGaugePanelB(self);
  UiFieldHudFaderUpdate(&self->fader);
}
