// bdc 0x088d341c UiFieldHudMainPhase
#include "bdc.h"

/* Phase 2 (setup/slide-in) of the field HUD (`UiFieldHudCtor`; sprite array `base.data`), by
   `phaseStep`:
   - 0: clears `unkA0`, sets `g_uiFieldHudEnabled`. On stage 0x20 (script global 1) it switches to
     phase 3, selects cell 2 of sprite 0x2a and puts sprites 0x2d/0x2e at y = -48, and returns.
     Otherwise cancels the player's powers (`ActorPlayerCancelPowers`), lays out the icons
     (`UiFieldHudUpdateIcons`) and advances the step.
   - 1: slides the panels by 15 per frame: sprites 0 (+x, -y), 6..10 (+x, +y), the 0x38d panel
     (2, 4, 0x14, 0x1c, 0x44) and the 0x38e panel (3, 5, 0x13, 0x1b, 0x15) when their story flags are
     set, sprites 0x27/0x1d when the player can throw (all -x, -y), 0x28/0x1e (+x, -y) and 0x36..0x3d
     (+x, +y); after 6 frames (`scrollPos` reaches 90) it resets `scrollPos` and advances the step.
   - 2: sets the alpha of sprites 0..0x47 to 1, sets flag bit 1 on sprites 1, 0xd and 0x43, stores
     the normalised XZ direction of the active camera in `viewDir`, runs the marker/arrow/radar and
     both gauge-panel updates and switches to phase 3, step 0.
   Other steps do nothing.
   A zero-length camera direction scales by 0 (bank constant S713); lanes saturate to [-1,1]. */

void UiFieldHudMainPhase(UiFieldHud *self)
{
  float dx, dz, lenSq, scale;
  const float step = 15.0f;
  int i;

  switch (self->base.phaseStep) {
  case 0:
    self->unkA0 = 0;
    g_uiFieldHudEnabled = 1;
    if (g_scriptGlobalVars[1] == 0x20) {
      self->base.phase = 3;
      GfxSpriteSetVCell(2.0f, ((GfxSprite **)self->base.data)[0x2a]);
      ((GfxSprite **)self->base.data)[0x2d]->posY = -48.0f;
      ((GfxSprite **)self->base.data)[0x2e]->posY = -48.0f;
      return;
    }
    ActorPlayerCancelPowers(self->player);
    UiFieldHudUpdateIcons(self);
    self->base.phaseStep = self->base.phaseStep + 1;
    break;

  case 1:
    ((GfxSprite **)self->base.data)[0]->posX += step;
    ((GfxSprite **)self->base.data)[0]->posY -= step;
    ((GfxSprite **)self->base.data)[6]->posX += step;
    ((GfxSprite **)self->base.data)[6]->posY += step;
    ((GfxSprite **)self->base.data)[7]->posX += step;
    ((GfxSprite **)self->base.data)[7]->posY += step;
    ((GfxSprite **)self->base.data)[8]->posX += step;
    ((GfxSprite **)self->base.data)[8]->posY += step;
    ((GfxSprite **)self->base.data)[9]->posX += step;
    ((GfxSprite **)self->base.data)[9]->posY += step;
    ((GfxSprite **)self->base.data)[10]->posX += step;
    ((GfxSprite **)self->base.data)[10]->posY += step;
    if (GameEventFlagTest(0x38d)) {
      ((GfxSprite **)self->base.data)[2]->posX -= step;
      ((GfxSprite **)self->base.data)[2]->posY -= step;
      ((GfxSprite **)self->base.data)[4]->posX -= step;
      ((GfxSprite **)self->base.data)[4]->posY -= step;
      ((GfxSprite **)self->base.data)[0x14]->posX -= step;
      ((GfxSprite **)self->base.data)[0x14]->posY -= step;
      ((GfxSprite **)self->base.data)[0x1c]->posX -= step;
      ((GfxSprite **)self->base.data)[0x1c]->posY -= step;
      ((GfxSprite **)self->base.data)[0x44]->posX -= step;
      ((GfxSprite **)self->base.data)[0x44]->posY -= step;
    }
    if (GameEventFlagTest(0x38e)) {
      ((GfxSprite **)self->base.data)[3]->posX -= step;
      ((GfxSprite **)self->base.data)[3]->posY -= step;
      ((GfxSprite **)self->base.data)[5]->posX -= step;
      ((GfxSprite **)self->base.data)[5]->posY -= step;
      ((GfxSprite **)self->base.data)[0x13]->posX -= step;
      ((GfxSprite **)self->base.data)[0x13]->posY -= step;
      ((GfxSprite **)self->base.data)[0x1b]->posX -= step;
      ((GfxSprite **)self->base.data)[0x1b]->posY -= step;
      ((GfxSprite **)self->base.data)[0x15]->posX -= step;
      ((GfxSprite **)self->base.data)[0x15]->posY -= step;
    }
    if (ActorPlayerCanThrow(self->player)) {
      ((GfxSprite **)self->base.data)[0x27]->posX -= step;
      ((GfxSprite **)self->base.data)[0x27]->posY -= step;
      ((GfxSprite **)self->base.data)[0x1d]->posX -= step;
      ((GfxSprite **)self->base.data)[0x1d]->posY -= step;
    }
    ((GfxSprite **)self->base.data)[0x28]->posX += step;
    ((GfxSprite **)self->base.data)[0x28]->posY -= step;
    ((GfxSprite **)self->base.data)[0x1e]->posX += step;
    ((GfxSprite **)self->base.data)[0x1e]->posY -= step;
    for (i = 0x36; i < 0x3e; i++) {
      ((GfxSprite **)self->base.data)[i]->posX += step;
      ((GfxSprite **)self->base.data)[i]->posY += step;
    }
    self->scrollPos = self->scrollPos + 15;
    if (self->scrollPos >= 90) {
      self->scrollPos = 0;
      self->base.phaseStep = self->base.phaseStep + 1;
    }
    break;

  case 2:
    for (i = 0; i < 0x48; i++) {
      ((GfxSprite **)self->base.data)[i]->alpha = 1.0f;
    }
    ((GfxSprite **)self->base.data)[1]->flags |= 1;
    ((GfxSprite **)self->base.data)[0xd]->flags |= 1;
    ((GfxSprite **)self->base.data)[0x43]->flags |= 1;

    dx = g_gfxActiveCamera->dir[0];
    dz = g_gfxActiveCamera->dir[2];
    /* normalise (x, 0, z); a zero length scales by 0; each lane saturated to [-1,1] */
    lenSq = dx * dx + 0.0f * 0.0f + dz * dz;
    scale = VfRsq(lenSq);
    if (lenSq == 0.0f)
      scale = 0.0f;
    self->viewDir[0] = VfSat1(dx * scale);
    self->viewDir[1] = VfSat1(dz * scale);

    UiFieldHudUpdatePlayerMarker(self);
    UiFieldHudUpdateHeadingArrow(self);
    UiFieldHudUpdateRadar(self);
    UiFieldHudUpdateGaugePanelA(self);
    UiFieldHudUpdateGaugePanelB(self);
    self->base.phase = 3;
    self->base.phaseStep = 0;
    break;

  default:
    break;
  }
}
