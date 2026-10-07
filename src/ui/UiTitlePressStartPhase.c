// bdc 0x089513fc UiTitlePressStartPhase
#include "bdc.h"

/* "Press start" phase (entry 2 of the phase table `0x08a9d3f8` (load `UiTitleLoadAssetsPhase`,
   `UiTitleIntroPhase`, press start, menu, load save, `UiTitleAttractPhase`, `UiTitleClosePhase`))
   of the title screen (task 200, `UiTitleCtor`; `"title.fab"`, `"title_logo.fab"`), stepped by
   `phaseStep`:
   0/1: once the background intro is over (`UiTitleUpdateBackground`) or the 250-frame timer ran out,
        fades sprites 0, 1 and 5 (logo, prompt, frame) in with sin(animAngle), +pi/90 per frame up to
        pi/2 (45 frames); confirm (pad `pressed` 0x4000) jumps to full alpha;
   2:   pulses the prompt alpha (0.1 + 0.9 * sin, angle +pi/60 wrapping at pi); 0x960 frames (40 s)
        with no button held on `g_padState` switch to phase 5 (`UiTitleAttractPhase`);
   on confirm (in step 1 or 2): prompt cell 1 (flash), sound 0, full alpha, timer 5, next step;
   3:   after 5 frames back to cell 0, fades the three sprites out (-pi/18 per frame) and, once the
        alpha is below 0.05, clears them and switches to phase 3 (the choice menu).
   A negative or >= 4 `phaseStep` also goes straight to phase 3. Always ends with `UiTitleNop`.
   The alphas are sin(animAngle) (`vsin.s` of angle * 2/pi). */

void UiTitlePressStartPhase(UiScreen *screen)

{
  UiTitle *title = (UiTitle *)screen;
  bool confirm = false;
  s32 step = screen->phaseStep;
  float angle;
  float alpha;

  if (step < 2) {
    if (step < 0) {
      goto toChoice;
    }
    if (step <= 0) {
      title->animAngle = 0.0f;
      ((GfxSprite **)screen->data)[0]->alpha = 0.0f;
      ((GfxSprite **)screen->data)[1]->alpha = 0.0f;
      ((GfxSprite **)screen->data)[5]->alpha = 0.0f;
      title->timer = 250;
      screen->phaseStep = screen->phaseStep + 1;
    }
    if (UiTitleUpdateBackground(screen) == 0) {
      title->timer = title->timer - 1;
      if (title->timer >= 0) {
        goto done;
      }
    }
    angle = title->animAngle + 0.034906585f;
    title->animAngle = angle;
    if (!(angle <= 1.5707964f)) {
      title->animAngle = 1.5707964f;
      angle = 1.5707964f;
    }
    alpha = __builtin_sinf(angle);
    if ((screen->pad->pressed & 0x4000) != 0) {
      alpha = 1.0f;
      confirm = true;
    }
    ((GfxSprite **)screen->data)[0]->alpha = alpha;
    ((GfxSprite **)screen->data)[1]->alpha = alpha;
    ((GfxSprite **)screen->data)[5]->alpha = alpha;
    if (alpha < 1.0f) {
      goto done;
    }
    title->animAngle = 1.5707964f;
    screen->phaseStep = screen->phaseStep + 1;
  } else if (step >= 3) {
    if (step >= 4) {
      goto toChoice;
    }
    /* step 3: fade out */
    title->timer = title->timer - 1;
    if (title->timer < 0) {
      GfxSpriteSetUCell(0.0f, ((GfxSprite **)screen->data)[1]);
    }
    angle = title->animAngle - 0.17453292f;
    title->animAngle = angle;
    if (angle < 0.0f) {
      angle = 0.0f;
    } else if (!(angle <= 1.5707964f)) {
      angle = 1.5707964f;
    }
    title->animAngle = angle;
    alpha = __builtin_sinf(angle);
    ((GfxSprite **)screen->data)[0]->alpha = alpha;
    ((GfxSprite **)screen->data)[1]->alpha = alpha;
    ((GfxSprite **)screen->data)[5]->alpha = alpha;
    if (!(alpha < 0.05f)) {
      goto done;
    }
    ((GfxSprite **)screen->data)[0]->alpha = 0.0f;
    ((GfxSprite **)screen->data)[1]->alpha = 0.0f;
    ((GfxSprite **)screen->data)[5]->alpha = 0.0f;
    screen->phaseStep = screen->phaseStep + 1;
    goto toChoice;
  }

  /* step 2 (also entered from step 1 once the fade-in is complete) */
  if (UiTitleUpdateBackground(screen) != 0) {
    angle = title->animAngle;
    if ((screen->pad->pressed & 0x4000) != 0) {
      confirm = true;
    } else if (g_padState != (PadState *)0x0) {
      if (g_padState->buttons != 0) {
        title->idleFrames = 0;
      } else {
        title->idleFrames = title->idleFrames + 1;
        if (title->idleFrames >= 0x960) {
          screen->phase = 5;
          screen->phaseStep = 0;
        }
      }
    }
    angle = angle + 0.05235988f;
    title->animAngle = angle;
    if (!(angle <= 3.1415927f)) {
      angle = angle - 3.1415927f;
      title->animAngle = angle;
    }
    alpha = __builtin_sinf(angle);
    ((GfxSprite **)screen->data)[0]->alpha = alpha * 0.9f + 0.1f;
  }
  if (confirm) {
    GfxSpriteSetUCell(1.0f, ((GfxSprite **)screen->data)[1]);
    if (SndHasManager()) {
      SndManagerPlay(SndGetManager(), 0, 0, 0);
    }
    ((GfxSprite **)screen->data)[0]->alpha = 1.0f;
    title->animAngle = 1.5707964f;
    title->timer = 5;
    screen->phaseStep = screen->phaseStep + 1;
  }
  goto done;

toChoice:
  screen->phase = 3;
  screen->phaseStep = 0;
done:
  UiTitleNop();
}
