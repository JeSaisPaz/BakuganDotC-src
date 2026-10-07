// bdc 0x088c1e10 GameFieldIsMapMovieSet1Watched
#include "bdc.h"

/* Returns 1 when the profile byte `+0x412 + 4*map` is set, `map` being script variable 15
   (`g_scriptGlobalVars->+0x3c`, the current map id from `GameStageToMapId`). The profile keeps
   four bytes per map at `+0x411 + 4*map`, one per story movie set (`GameStoryMovieCtor` marks set
   `s` watched at `+0x411 + 4*map + s`; `GameStoryMovieSelectQuestMode` reads byte 0 as 'entered',
   2 as 'cleared', 3 as 'rewarded'), so this tests byte 1: movie set 1 of the current map. */

s32 GameFieldIsMapMovieSet1Watched(void)
{
  u32 map = (u32)g_scriptGlobalVars[15];

  if (SaveGetProfile()->data->mapMovieWatched[map & 0xff][1] != 0) {
    return 1;
  }
  return 0;
}
