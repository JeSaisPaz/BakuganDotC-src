// bdc 0x08940948 UiScreen390MainPhase
#include "bdc.h"

/* Phase 2 of `UiScreen390`: the terminal-use sequence. Steps wait for the active
   fader (`GfxGetActiveFader`, `GfxFaderIsFinished`) and timers run 6x faster once Cross was
   pressed (`UiScreen390CheckSkip`). Normal script (steps 0..6): spawn effect 4 next to the player
   on the terminal side (`UiScreen390SpawnEffect`, 30 units), play sound 0x2c0000b, set the
   terminal object (spawn-record kind `0xcf + +0x80`) to scale 1.0
   (`ActorStageObjRecordSetObjScale`), wait 120 frames, release the effect
   (`UiScreen390ReleaseEffect`), wait 40, restore the terminal's effect
   (`UiScreen390RestoreTerminalEffect`) and advance to `UiScreen390ExitPhase` (step 100). */

/* Counts the step timer down (6 per frame in fast-forward), or advances the step once it ran out. */
static inline void UiScreen390TickTimer(UiScreen390 *s, s32 timer)
{
  if (timer > 0) {
    if (g_uiScreen390FastForward == 0) {
      timer = timer - 1;
    }
    else {
      timer = timer - 6;
    }
    s->frameTimer = timer;
  }
  else {
    s->base.phaseStep = s->base.phaseStep + 1;
  }
}

void UiScreen390MainPhase(UiScreen *screen)

{
  UiScreen390 *s = (UiScreen390 *)screen;
  int step;

  if (GfxFaderIsFinished(GfxGetActiveFader())) {
    UiScreen390CheckSkip(screen);
  }
  step = screen->phaseStep;
  switch ((u32)step) {
  case 0:
    UiScreen390SpawnEffect(30.0f, screen, 4);
    screen->phaseStep = screen->phaseStep + 1;
    break;
  case 1:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      screen->phaseStep = screen->phaseStep + 1;
    }
    break;
  case 2:
    if (SndHasManager()) {
      SndManagerPlay(SndGetManager(), 0x2c0000b, 0, 0);
    }
    ActorStageObjRecordSetObjScale(1.0f, s->terminal + 0xcf);
    s->frameTimer = 120;
    screen->phaseStep = screen->phaseStep + 1;
    UiScreen390TickTimer(s, 120);
    break;
  case 3:
    UiScreen390TickTimer(s, (s32)s->frameTimer);
    break;
  case 4:
    UiScreen390ReleaseEffect(screen);
    s->frameTimer = 40;
    screen->phaseStep = screen->phaseStep + 1;
    break;
  case 5:
    if ((s32)s->frameTimer > 0) {
      s->frameTimer = s->frameTimer - 1;
    }
    else {
      screen->phaseStep = step + 1;
    }
    break;
  case 6:
    UiScreen390RestoreTerminalEffect(screen);
    screen->phaseStep = 100;
    break;
  case 0x14:
    UiScreen390SpawnEffect(30.0f, screen, 5);
    screen->phaseStep = screen->phaseStep + 1;
    break;
  case 0x15:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      screen->phaseStep = screen->phaseStep + 1;
    }
    break;
  case 0x16:
    ActorStageObjRecordForceState0B(s->terminal + 0xcf);
    s->frameTimer = 90;
    screen->phaseStep = screen->phaseStep + 1;
    UiScreen390TickTimer(s, 90);
    break;
  case 0x17:
    UiScreen390TickTimer(s, (s32)s->frameTimer);
    break;
  case 0x18:
    UiScreen390ReleaseEffect(screen);
    s->frameTimer = 50;
    screen->phaseStep = screen->phaseStep + 1;
    break;
  case 0x19:
    UiScreen390TickTimer(s, (s32)s->frameTimer);
    break;
  case 0x1a:
    UiScreen390SpawnEffect(30.0f, screen, 4);
    s->frameTimer = 15;
    screen->phaseStep = screen->phaseStep + 1;
    break;
  case 0x1b:
    if (s->frameTimer != 0) {
      s->frameTimer = s->frameTimer - 1;
    }
    else {
      screen->phaseStep = step + 1;
    }
    break;
  case 0x1c:
    screen->phaseStep = 2;
    break;
  default:
    /* steps 7..0x13 and above 0x1c end the phase */
    screen->phaseStep = 0;
    screen->phase = screen->phase + 1;
    break;
  }
  return;
}
