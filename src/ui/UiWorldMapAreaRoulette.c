// bdc 0x089a0d80 UiWorldMapAreaRoulette
#include "bdc.h"

/* Random area pick of `UiWorldMap` (triangle on the area list, main-phase steps
   0xc/0xd; state `rouletteStep..`): step 0 collects the unlocked areas (`unlockMask` bits →
   `rouletteItems`, `rouletteCount`), clears `areaId` and draws 10 or 11 hops; step 1 hops the area
   cursor to a different random area (`CoreRtcGetMicrosecond` mod count, up to 65 tries, then the
   first different one), playing the cursor sound, redrawing (`UiWorldMapRefreshAreaCursor`) and,
   when `SaveGetProfileFlag0` is set, storing the pick through `UiWorldMapSetPlayerSelection`;
   with a single area it just selects it as the only hop, silently. Step 2 arms a 4-frame wait;
   step 3 pulses and zooms the cursor until the wait runs out, then goes back to step 1 or, after
   the last hop, to step 4. Returns 1 from step 4 on (finished), else 0. Same scheme as
   `UiWorldMapStageRoulette`. */

int UiWorldMapAreaRoulette(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  u8 step = map->rouletteStep;
  u8 mask;
  u8 count;
  u8 tries;
  u8 i;
  int k;

  if (step < 2) {
    if (step == 0) {
      map->rouletteCount = 0;
      mask = map->unlockMask;
      count = map->rouletteCount;
      for (k = 0; k < 8; k++) {
        if ((mask & (1 << k)) != 0) {
          map->rouletteItems[count] = (u8)k;
          count++;
        }
      }
      map->rouletteCount = count;
      map->areaId = 0;
      map->roulettePick = 0xff;
      map->rouletteHops = (u8)((s32)CoreRtcGetMicrosecond() % 2 + 10);
      map->rouletteStep = 1;
      return 0;
    }

    if (map->rouletteCount == 1) {
      map->areaId = map->rouletteItems[0];
      map->rouletteHops = 1;
      map->rouletteStep = step + 1;
      return 0;
    }

    tries = 0;
    for (;;) {
      u32 rtc = CoreRtcGetMicrosecond();
      count = map->rouletteCount;
      map->roulettePick = (u8)((s32)rtc % (s32)count);
      if (map->areaId != map->rouletteItems[map->roulettePick]) {
        map->areaId = map->rouletteItems[map->roulettePick];
        break;
      }
      if (tries == 0x40) {
        /* Random tries exhausted: take the first area that differs. */
        for (k = 0; k < count; k++) {
          i = (u8)k;
          if (map->areaId != map->rouletteItems[i]) {
            map->roulettePick = i;
            map->areaId = map->rouletteItems[map->roulettePick];
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
    UiWorldMapRefreshAreaCursor(screen);
    if (SaveGetProfileFlag0()) {
      UiWorldMapSetPlayerSelection(screen, map->netSession, map->areaId);
    }
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

  UiWorldMapPulseCursor(screen);
  UiWorldMapPulseSelectedArea(screen);
  UiWorldMapZoomSelectedArea(screen);
  UiPulseStep(((GfxSprite **)screen->data)[0x5d], &map->randomPulse);
  if (map->rouletteWait != 0) {
    map->rouletteWait--;
  } else {
    map->rouletteHop++;
    map->rouletteStep = (map->rouletteHop == map->rouletteHops) ? 4 : 1;
  }
  return 0;
}
