// bdc 0x08951838 UiTitleChoicePhase
#include "bdc.h"

/* Menu phase (entry 3 of the phase table `0x08a9d3f8` (load `UiTitleLoadAssetsPhase`,
   `UiTitleIntroPhase`, press start, menu, load save, `UiTitleAttractPhase`,
   `UiTitleClosePhase`)) of the title screen (task 200, `UiTitleCtor`; `"title.fab"`,
   `"title_logo.fab"`), stepped by `phaseStep` after updating the background fab (`GfxFabUpdate`):
   1/2: input first: confirm (pad `pressed` 0x4000, sound 0), up/down (`repeat` 0x10/0x40, sound 1)
        moves `choiceIndex` (wrapping over 0..1, highlight angle reset to pi/2 on change); each of
        the choice sprites 2..4 grows its scale factor `unk7c[i]` towards 1 (selected, alpha pulsing
        0.5 + 0.5 * sin) or shrinks it towards 0 (alpha 1), scale 0.7 + 0.3 * (1 - cos(pi * ease)) / 2;
   0:   sprites 2..4 transparent, the default choice is Continue (1) when profile flag 0x40000000 is
        set, then as step 1;
   1:   fades sprites 2 and 3 in with sin(animAngle) (+0.1396 per frame, up to pi/2; confirm jumps
        to full alpha), then step 2;
   2:   on confirm: entry 0 sets menu result 0 and goes on to step 3; entry 1 switches to phase 4
        (load) and stops the BGM; (entry 2 would set menu result 4 and go on);
   3/4: stops the BGM, fades the screen to black over 15 frames, and after 4 frames resets sprite 1
        to cell 0; once the fade is done, step 5;
   5:   releases stream file 0x1a, then any outstanding stream request, then step 6;
   any other step: phase 6 (`UiTitleClosePhase`). Always ends with `UiTitleNop`.
   Bank constant S703 (2/pi) turns the radian angles into quarter turns for `vsin.s`/`vcos.s`. */

void UiTitleChoicePhase(UiScreen *screen)

{
  UiTitle *title = (UiTitle *)screen;
  GfxSprite *sprite;
  GfxFader *fader;
  PadState *pad;
  bool confirm = false;
  bool advance;
  s32 delta = 0;
  s32 oldIndex;
  s32 index;
  s32 i;
  float angle;
  float alpha;
  float ease;
  float scale;

  GfxFabUpdate(((GfxFab **)screen->bgData)[2]);
  if (screen->phaseStep > 0 && screen->phaseStep < 3) {
    pad = screen->pad;
    if ((pad->pressed & 0x4000) != 0) {
      confirm = true;
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 0, 0, 0);
      }
    } else if ((pad->repeat & 0x10) != 0) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 1, 0, 0);
      }
      delta = -1;
    } else if ((pad->repeat & 0x40) != 0) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 1, 0, 0);
      }
      delta = 1;
    }
    oldIndex = (s32)title->choiceIndex;
    index = oldIndex + delta;
    title->choiceIndex = (u32)index;
    if (index < 0) {
      index = 1;
      title->choiceIndex = 1;
    }
    if (index >= 2) {
      title->choiceIndex = 0;
      index = 0;
    }
    if (index < 0) {
      index = 0;
    } else if (index > 1) {
      index = 1;
    }
    title->choiceIndex = (u32)index;
    if (oldIndex != index) {
      title->choiceAngle = 1.5707964f;
    }
    for (i = 0; i < 3; i++) {
      sprite = ((GfxSprite **)screen->data)[i + 2];
      if ((u32)i == title->choiceIndex) {
        title->unk7c[i] = title->unk7c[i] + 0.17453292f;
        angle = title->choiceAngle + 0.05235988f;
        title->choiceAngle = angle;
        if (!(angle <= 3.1415927f)) {
          angle = angle - 3.1415927f;
          title->choiceAngle = angle;
        }
        sprite->alpha = __builtin_sinf(angle) * 0.5f + 0.5f;
        scale = title->unk7c[i];
      } else {
        title->unk7c[i] = title->unk7c[i] - 0.17453292f;
        sprite->alpha = 1.0f;
        scale = title->unk7c[i];
      }
      if (scale < 0.0f) {
        scale = 0.0f;
      } else if (!(scale <= 1.0f)) {
        scale = 1.0f;
      }
      title->unk7c[i] = scale;
      ease = 1.0f - (scale - 1.0f) * (scale - 1.0f);
      scale = (1.0f - __builtin_cosf(ease * 3.1415927f)) * 0.5f * 0.3f + 0.7f;
      GfxSpriteSetScaleRotation(sprite, scale, scale, 0.0f, false);
    }
  }

  switch (screen->phaseStep) {
  case 0:
    fader = GfxGetActiveFader();
    fader->sortKey = 1500.0f;
    for (i = 2; i < 5; i++) {
      ((GfxSprite **)screen->data)[i]->alpha = 0.0f;
    }
    for (i = 0; i < 3; i++) {
      title->unk7c[i] = 0.0f;
    }
    title->choiceIndex = SaveProfileHasFlags(SaveGetProfile(), 0x40000000) != 0;
    title->choiceAngle = 1.5707964f;
    title->animAngle = 0.0f;
    screen->phaseStep = screen->phaseStep + 1;
    /* fallthrough */
  case 1:
    angle = title->animAngle + 0.13962634f;
    title->animAngle = angle;
    if (angle < 0.0f) {
      angle = 0.0f;
    } else if (!(angle <= 1.5707964f)) {
      angle = 1.5707964f;
    }
    title->animAngle = angle;
    alpha = __builtin_sinf(angle);
    for (i = 2; i < 4; i++) {
      ((GfxSprite **)screen->data)[i]->alpha = alpha;
    }
    if (confirm) {
      alpha = 1.0f;
    }
    if (alpha < 1.0f) {
      break;
    }
    for (i = 2; i < 4; i++) {
      ((GfxSprite **)screen->data)[i]->alpha = 1.0f;
    }
    screen->phaseStep = screen->phaseStep + 1;
    /* fallthrough */
  case 2:
    if (!confirm) {
      break;
    }
    index = (s32)title->choiceIndex;
    advance = true;
    if (index == 0) {
      UiSetMenuResult(screen, 0);
    } else if (index == 1) {
      screen->phase = 4;
      screen->phaseStep = 0;
      SndBgmCancelChannel(0);
      SndBgmQueueStop(0.5f, 0);
      advance = false;
    } else if (index == 2) {
      UiSetMenuResult(screen, 4);
    }
    if (advance) {
      screen->phaseStep = screen->phaseStep + 1;
    }
    break;
  case 3:
    SndBgmCancelChannel(0);
    SndBgmQueueStop(0.5f, 0);
    fader = GfxGetActiveFader();
    fader->start[0] = 0.0f;
    fader->start[1] = 0.0f;
    fader->start[2] = 0.0f;
    fader->start[3] = 0.0f;
    fader = GfxGetActiveFader();
    fader->end[0] = 0.0f;
    fader->end[1] = 0.0f;
    fader->end[2] = 0.0f;
    fader->end[3] = 1.0f;
    GfxFaderStart(GfxGetActiveFader(), 15);
    title->timer = 4;
    screen->phaseStep = screen->phaseStep + 1;
    /* fallthrough */
  case 4:
    title->timer = title->timer - 1;
    if (title->timer < 0) {
      GfxSpriteSetUCell(0.0f, ((GfxSprite **)screen->data)[1]);
    }
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      screen->phaseStep = screen->phaseStep + 1;
    }
    break;
  case 5:
    SndStreamFileRelease(0x1a);
    if (!SndStreamFileIsLoaded(0x1a) && SndStreamFileRelease(-1)) {
      screen->phaseStep = screen->phaseStep + 1;
    }
    break;
  default:
    screen->phase = 6;
    screen->phaseStep = 0;
    break;
  }
  UiTitleNop();
}
