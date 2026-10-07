// bdc 0x0899cf50 UiWorldMapSetRankLetter
#include "bdc.h"

/* Sets an evaluation sprite of `UiWorldMap` to the rank letter for `rank`: 0 S, 1
   A, 2 B, 3+ C (`"hyouka_moji_02_s"`…`"_c"`). */

void UiWorldMapSetRankLetter(UiScreen *screen, GfxSprite *sprite, u8 rank)

{
  char name[64];

  if (rank == 0) {
    sprintf(name,"hyouka_moji_02_s");
  }
  else if (rank < 2) {
    sprintf(name,"hyouka_moji_02_a");
  }
  else if (rank < 3) {
    sprintf(name,"hyouka_moji_02_b");
  }
  else {
    sprintf(name,"hyouka_moji_02_c");
  }
  sprite->texture = GfxFindTexture(name);
  return;
}
