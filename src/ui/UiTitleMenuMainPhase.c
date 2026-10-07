// bdc 0x0895067c UiTitleMenuMainPhase
#include "bdc.h"

/* Main phase (entry 2 of the phase table `0x08a9d3b8`) of screen task 1000 (`UiTitleMenuCtor`),
   stepped by `phaseStep`: step 0 waits until the now-loading task is gone (`UiLoadingIsOpen`) and
   starts looping BGM 0x1a on channel 0 (`SndBgmQueuePlay`); step 1 edits the first sprite of
   `data` (one input per frame: held d-pad moves it by 1 px, repeat bits 0x1000/0x8000 scale
   it by ±0.01, held 0x4000 rotates it by 1 degree, pressed 0x2000 does nothing), reapplies its
   scale/rotation, and on pressed 0x8 (start) sets menu result 0, plays sound 0 and advances the
   step; step 2 cancels and fades out BGM channel 0 over 1 s, resets `phaseStep` and advances
   `phase`. Other steps do nothing. */

void UiTitleMenuMainPhase(UiScreen *screen)
{
  PadState *pad;
  GfxSprite *sprite;
  int step;
  bool confirm;

  step = screen->phaseStep;
  if (step < 0 || step > 2) {
    return;
  }
  if (step == 2) {
    SndBgmCancelChannel(0);
    SndBgmQueueStop(1.0f, 0);
    screen->phaseStep = 0;
    screen->phase = screen->phase + 1;
    return;
  }
  if (step == 0) {
    if (UiLoadingIsOpen()) {
      return;
    }
    SndBgmQueuePlay(0, 0x1a, 1, 0);
    screen->phaseStep = screen->phaseStep + 1;
  }

  pad = screen->pad;
  sprite = *(GfxSprite **)screen->data;
  confirm = false;
  if ((pad->buttons & 0x10) != 0) {
    sprite->posY = sprite->posY + -1.0f;
  } else if ((pad->buttons & 0x40) != 0) {
    sprite->posY = sprite->posY + 1.0f;
  } else if ((pad->buttons & 0x80) != 0) {
    sprite->posX = sprite->posX + -1.0f;
  } else if ((pad->buttons & 0x20) != 0) {
    sprite->posX = sprite->posX + 1.0f;
  } else if ((pad->pressed & 0x2000) != 0) {
    /* no change */
  } else if ((pad->repeat & 0x1000) != 0) {
    sprite->scaleX = sprite->scaleX + 0.01f;
    sprite->scaleY = sprite->scaleY + 0.01f;
  } else if ((pad->repeat & 0x8000) != 0) {
    sprite->scaleX = sprite->scaleX - 0.01f;
    sprite->scaleY = sprite->scaleY - 0.01f;
  } else if ((pad->buttons & 0x4000) != 0) {
    sprite->angle = sprite->angle + 0.017453292f;
  } else if ((pad->pressed & 0x8) != 0) {
    confirm = true;
  }
  GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
  if (confirm) {
    UiSetMenuResult(screen, 0);
    if (SndHasManager()) {
      SndManagerPlay(SndGetManager(), 0, 0, 0);
    }
    screen->phaseStep = screen->phaseStep + 1;
  }
}
