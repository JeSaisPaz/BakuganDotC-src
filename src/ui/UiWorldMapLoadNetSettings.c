// bdc 0x0899883c UiWorldMapLoadNetSettings
#include "bdc.h"

/* Clears the four player records of `UiWorldMap`, then fills the local player's
   record (`player[netSession]`) from the shared battle settings in profile words 0x19
   (100/200/300 → 0/1/2, anything else -1), 0x1a (copied), and 0x1b (1/3/5 → 0/1/2, anything
   else kept as is), with `slotTag` = 0xff and `unkE` = 0; used on the network intro path. The
   same words are written back from host packets by `UiWorldMapApplyNetSettings`. */

void UiWorldMapLoadNetSettings(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  s32 i;
  u32 word;
  s8 value;

  for (i = 0; i < 4; i++) {
    memset(&map->player[i], 0, 0x10);
  }
  i = map->netSession;
  map->player[i].slotTag = 0xff;
  map->player[i].unkE = 0;

  word = SaveProfileGetWord(SaveGetProfile(), 0x19);
  if (word == 300) {
    value = 2;
  } else if (word == 200) {
    value = 1;
  } else if (word == 100) {
    value = 0;
  } else {
    value = -1;
  }
  map->player[i].setting[0] = value;

  map->player[i].setting[1] = (s8)SaveProfileGetWord(SaveGetProfile(), 0x1a);

  word = SaveProfileGetWord(SaveGetProfile(), 0x1b);
  if (word == 5) {
    word = 2;
  } else if (word == 3) {
    word = 1;
  } else if (word == 1) {
    word = 0;
  }
  map->player[i].setting[2] = (s8)word;
}
