// bdc 0x08835214 UiTalkWindowStep
#include "bdc.h"

/* State machine of the battle talk window (`talkState`, only while UI window 0xb is active), run
   by `UiTalkWindowUpdate`: 1 lays out the frame (`UiTalkLayoutWindowFrame`), shows sprites
   0x7e..0x83 and goes on as 2; 2 fades them in (sine of the ramp `talkFade`, text alpha
   `talkTextAlpha` = sine * `hudAlpha`); 3–4 hold the page for `talkHold` frames, or with a hold
   of -1 show the "next" cursor (sprite 0xef, state 10) and clear `g_scriptGlobalVars[12]`; 10
   animates the cursor until the confirm button (pad `pressed` bit 0x4000) is pressed; 0x14: when
   the timed hold ran out (`talkHold` 0) and the last printer result `talkPrintBusy` is 0, goes
   straight to 0x1e (next page, no fade-out) and, with `talkForce`, suspends the stage event
   script; otherwise sets the fade to pi/2 and goes on as 0x15; 0x15 waits for the voice stream
   (`SndBgmPlayerIsStopped` on player 1), resumes the stage event script and restores the BGM
   (`BtlSetDuckVolume`); 0x16 fades out and goes to 0x1e; 0x1e goes to 100 (finish) when
   `talkPrintBusy` is set, else 0x1e/0x1f print the next page through the text printer's virtual
   slot 2 (result stored in `talkPrintBusy`), set the hold to 45, 90 or 135 frames by text height
   and go back to 2; 100 hides everything, clears the printer (`GfxSpriteLayerClear`), returns
   to state 0 and sets `g_scriptGlobalVars[12]` = 1 (message finished). */

typedef u8 (*UiTalkPrintFn)(float x, float y, float z, void *self, char *text, s32 flags,
                            u32 resumePos, s32 extra);

void UiTalkWindowStep(void *win)
{
  BtlHud *hud = (BtlHud *)win;
  UiTextPrinter *printer;
  const VtblEntry *entry;
  GfxSprite *cursor;
  float fade;
  float ramp;
  float s;
  float cell;
  float height;
  s32 hold;
  s32 voice;
  s32 i;
  u8 busy;

  if (UiGetWindowActive(0xb) == 0) {
    return;
  }

  switch (hud->talkState) {
  case 1:
    hud->talkTextAlpha = 0.0f;
    hud->talkOverlayAlpha = 1.0f;
    hud->talkFade = 0.0f;
    hud->talkRamp = 0.0f;
    UiTalkLayoutWindowFrame(hud);
    for (i = 0x7e; i < 0x84; i++) {
      hud->sprites[i]->flags |= 1;
      hud->sprites[i]->alpha = 0.0f;
    }
    fade = hud->talkFade + 0.14f;
    ramp = hud->talkRamp + 0.07f;
    hud->talkState = hud->talkState + 1;
    hold = hud->talkHold;
    goto fade_in;

  case 2:
    fade = hud->talkFade + 0.14f;
    ramp = hud->talkRamp + 0.07f;
    hold = hud->talkHold;
  fade_in:
    hud->talkFade = fade;
    if (hold == -1) {
      fade = fade + 0.14f;
      hud->talkFade = fade;
    }
    if (fade < 0.0f) {
      fade = 0.0f;
    }
    else if (!(fade <= 1.5707964f)) {
      fade = 1.5707964f;
    }
    hud->talkFade = fade;
    hud->talkRamp = ramp;
    if (!(ramp < 1.0f)) {
      hud->talkRamp = 1.0f;
    }
    s = __builtin_sinf(fade);
    for (i = 0x7e; i < 0x81; i++) {
      hud->sprites[i]->alpha = s;
    }
    for (i = 0x81; i < 0x84; i++) {
      hud->sprites[i]->alpha = s;
    }
    hud->talkTextAlpha = s * hud->hudAlpha;
    if (s < 1.0f) {
      return;
    }
    hold = hud->talkHold;
    hud->talkState = hud->talkState + 1;
    goto hold_start;

  case 3:
    hold = hud->talkHold;
  hold_start:
    if (hold == -1) {
      /* wait for confirm: show the "next" cursor under the right cap */
      hud->sprites[0xef]->flags |= 1;
      hud->sprites[0xef]->alpha = 1.0f;
      hud->sprites[0xef]->posX = hud->sprites[0x80]->posX + 32.0f;
      hud->sprites[0xef]->posY = hud->sprites[0x80]->posY + 68.0f;
      hud->sprites[0xef]->posW = 0.0f;
      g_scriptGlobalVars[12] = 0;
      hud->talkState = 10;
      return;
    }
    hold = hold - 1;
    hud->talkState = hud->talkState + 1;
    goto hold_tick;

  case 4:
    hold = hud->talkHold - 1;
  hold_tick:
    hud->talkHold = hold;
    if (hold > 0) {
      hud->talkTextAlpha = hud->hudAlpha;
      if (g_btlBattleOver != 0) {
        hud->talkTextAlpha = 0.0f;
      }
      return;
    }
    hud->talkHold = 0;
    hud->talkState = 0x14;
    cursor = hud->sprites[0xef];
    goto cursor_anim;

  case 10:
    cursor = hud->sprites[0xef];
  cursor_anim:
    /* posW counts animation frames: 16 frames, 8 per cell */
    cell = cursor->posW + 1.0f;
    cursor->posW = cell;
    cursor = hud->sprites[0xef];
    if (!(cell < 16.0f)) {
      cursor->posW = 0.0f;
      cursor = hud->sprites[0xef];
    }
    GfxSpriteSetUCell((float)(s32)(hud->sprites[0xef]->posW * 0.125f), cursor);
    /* the asm tests bit 0x40 of the high byte of `pressed` */
    if ((hud->pad->pressed & 0x4000) == 0) {
      hud->talkTextAlpha = hud->hudAlpha;
      if (g_btlBattleOver != 0) {
        hud->talkTextAlpha = 0.0f;
      }
      return;
    }
    hud->sprites[0xef]->flags &= ~1u;
    hud->talkState = 0x14;
    hold = hud->talkHold;
    goto next_page;

  case 0x14:
    hold = hud->talkHold;
  next_page:
    if (hold == 0 && hud->talkPrintBusy == 0) {
      hud->talkState = 0x1e;
      if (hud->talkForce == 0) {
        return;
      }
      if (BtlStageReturnFalse() != 0) {
        return;
      }
      hud->talkScriptSuspended = 1;
      BtlStageSuspendEventScript();
      return;
    }
    hud->talkFade = 1.5707964f;
    hud->talkState = hud->talkState + 1;
    voice = hud->talkVoiceId;
    goto voice_wait;

  case 0x15:
    voice = hud->talkVoiceId;
  voice_wait:
    if (voice != -1) {
      if (SndBgmPlayerExists(1) && SndBgmPlayerIsStopped(SndBgmPlayerGet(1)) == 0) {
        return;
      }
      if (BtlCameraTaskExists() != 0 && ((BtlMain *)BtlGetCameraTask())->phase != 2 &&
          hud->talkScriptSuspended != 0) {
        BtlStageResumeEventScript();
      }
      if (BtlCameraTaskExists() != 0) {
        BtlGetCameraTask();
        BtlSetDuckVolume(1.0f, 1.0f, 2.0f);
      }
    }
    hud->talkState = hud->talkState + 1;
    fade = hud->talkFade - 0.2f;
    ramp = hud->talkRamp - 0.05f;
    hold = hud->talkHold;
    goto fade_out;

  case 0x16:
    fade = hud->talkFade - 0.2f;
    ramp = hud->talkRamp - 0.05f;
    hold = hud->talkHold;
  fade_out:
    hud->talkFade = fade;
    if (hold == -1) {
      fade = fade - 0.2f;
      hud->talkFade = fade;
    }
    if (fade < 0.0f) {
      fade = 0.0f;
    }
    else if (!(fade <= 1.5707964f)) {
      fade = 1.5707964f;
    }
    hud->talkFade = fade;
    s = __builtin_sinf(fade);
    hud->talkRamp = ramp;
    if (!(ramp < 1.0f)) {
      hud->talkRamp = 1.0f;
    }
    for (i = 0x7e; i < 0x81; i++) {
      hud->sprites[i]->alpha = s;
    }
    for (i = 0x81; i < 0x84; i++) {
      hud->sprites[i]->alpha = s;
    }
    hud->talkTextAlpha = s * hud->hudAlpha;
    if (!(s <= 0.0f)) {
      return;
    }
    hud->talkState = 0x1e;
    busy = hud->talkPrintBusy;
    goto print_check;

  case 0x1e:
    busy = hud->talkPrintBusy;
  print_check:
    if (busy != 0) {
      hud->talkState = 100;
      return;
    }
    hud->talkState = hud->talkState + 1;
    printer = (UiTextPrinter *)hud->overlayObj[0];
    goto print_page;

  case 0x1f:
    printer = (UiTextPrinter *)hud->overlayObj[0];
  print_page:
    GfxSpriteLayerClear(&printer->layer);
    printer->glyphs = NULL;
    printer = (UiTextPrinter *)hud->overlayObj[0];
    entry = &printer->layer.vtbl[2];
    hud->talkPrintBusy = ((UiTalkPrintFn)entry->fn)(hud->talkTextPos[0], hud->talkTextPos[1], 48.0f,
                                                    (u8 *)printer + entry->delta, hud->talkText,
                                                    0, printer->resumePos, 0);
    height = hud->talkTextHeight;
    printer = (UiTextPrinter *)hud->overlayObj[0];
    if (height <= printer->lineHeight * 4.0f) {
      hold = 45;
    }
    else if (height <= printer->lineHeight * 5.0f) {
      hold = 90;
    }
    else {
      hold = 135;
    }
    hud->talkHold = hold;
    hud->talkState = 2;
    return;

  case 100:
    for (i = 0x7e; i < 0x84; i++) {
      hud->sprites[i]->flags &= ~1u;
    }
    printer = (UiTextPrinter *)hud->overlayObj[0];
    GfxSpriteLayerClear(&printer->layer);
    printer->glyphs = NULL;
    hud->talkState = 0;
    if (BtlCameraTaskExists() != 0 && ((BtlMain *)BtlGetCameraTask())->phase != 2 &&
        hud->talkScriptSuspended != 0) {
      BtlStageResumeEventScript();
    }
    hud->talkScriptSuspended = 0;
    hud->talkVoiceId = -1;
    g_scriptGlobalVars[12] = 1;
    return;

  default:
    return;
  }
}
