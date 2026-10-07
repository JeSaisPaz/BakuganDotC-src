// bdc 0x088d3e98 GameStageGetPackagePath
#include "bdc.h"

/* Builds the package name of stage `stage` in the static buffer `g_gameStagePackPath`: the entry's pack name
   (`GameStageFindEntry`) plus `"b"`, `"c"` or `"d"` for profile word 8 = 1/2/3 (stage variant),
   plus `".lzs"`. */

char *GameStageGetPackagePath(s32 stage)
{
  s32 *entry;
  SaveProfile *self;
  u32 variant;

  entry = GameStageFindEntry(stage);
  strcpy(g_gameStagePackPath, ((char **)entry)[2]);
  self = SaveGetProfile();
  variant = SaveProfileGetWord(self, 8);
  if ((s32)variant < 2) {
    if (0 < (s32)variant) {
      strcat(g_gameStagePackPath, "b");
    }
  } else if ((s32)variant < 3) {
    strcat(g_gameStagePackPath, "c");
  } else if ((s32)variant < 4) {
    strcat(g_gameStagePackPath, "d");
  }
  strcat(g_gameStagePackPath, ".lzs");
  return g_gameStagePackPath;
}
