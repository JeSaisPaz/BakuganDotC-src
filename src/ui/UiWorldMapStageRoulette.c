// bdc 0x0899e990 UiWorldMapStageRoulette
#include "bdc.h"

/* Random stage pick of `UiWorldMap`'s stage list (triangle, main-phase steps
   0x18/0x19; state `rouletteStep..`): step 0 collects the area's cleared stages 0..n-1
   (`rouletteItems`, `rouletteCount`) and draws 10 or 11 hops; step 1 hops the cursor to a different
   random stage (`CoreRtcGetMicrosecond` mod count, up to 65 tries, then the first different one),
   playing the cursor sound and redrawing (`UiWorldMapRefreshStageCursor`); with a single stage it
   just selects it as the only hop, silently. Step 2 arms a 4-frame wait; step 3 pulses and zooms the
   cursor (`UiWorldMapZoomSelectedStage`) until the wait runs out, then goes back to step 1 or, after
   the last hop, to step 4. Returns 1 from step 4 on (finished), else 0. */

int UiWorldMapStageRoulette(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  u8 step = map->rouletteStep;
  u8 count;
  u8 tries;
  u8 i;
  int n;
  int k;

  if (step < 2) {
    if (step == 0) {
      n = map->clearedStage[map->areaGroup[map->areaId]];
      map->rouletteCount = 0;
      for (k = 0; k < n; k++) {
        map->rouletteItems[map->rouletteCount] = (u8)k;
        map->rouletteCount++;
      }
      map->stage = 0;
      map->roulettePick = 0xff;
      map->rouletteHops = (u8)((s32)CoreRtcGetMicrosecond() % 2 + 10);
      map->rouletteStep = 1;
      return 0;
    }

    if (map->rouletteCount == 1) {
      map->stage = map->rouletteItems[0];
      map->rouletteHops = 1;
      map->rouletteStep = step + 1;
      return 0;
    }

    tries = 0;
    for (;;) {
      u32 rtc = CoreRtcGetMicrosecond();
      count = map->rouletteCount;
      map->roulettePick = (u8)((s32)rtc % (s32)count);
      if (map->stage != map->rouletteItems[map->roulettePick]) {
        map->stage = map->rouletteItems[map->roulettePick];
        break;
      }
      if (tries == 0x40) {
        /* Random tries exhausted: take the first stage that differs. */
        for (k = 0; k < count; k++) {
          i = (u8)k;
          if (map->stage != map->rouletteItems[i]) {
            map->roulettePick = i;
            map->stage = map->rouletteItems[map->roulettePick];
            goto play;
          }
        }
        if (count > 0) {
          map->roulettePick = i;
        }
        break;
      }
      tries++;
    }
play:
    if (SndHasManager()) {
      SndManagerPlay(SndGetManager(), 1, 0, 0);
    }
    UiWorldMapRefreshStageCursor(screen);
    map->rouletteStep++;
    return 0;
  }

  if (step < 3) {
    map->rouletteWait = 4;
    map->rouletteStep = step + 1;
    return 0;
  }

  if (step >= 4) {
    return 1;
  }

  UiWorldMapPulseStageCursor(screen);
  UiWorldMapZoomSelectedStage(screen);
  UiPulseStep(((GfxSprite **)screen->data)[0x5d], &map->randomPulse);
  if (map->rouletteWait != 0) {
    map->rouletteWait--;
  } else {
    map->rouletteHop++;
    map->rouletteStep = (map->rouletteHop == map->rouletteHops) ? 4 : 1;
  }
  return 0;
}
