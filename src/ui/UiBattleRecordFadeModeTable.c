// bdc 0x089494f0 UiBattleRecordFadeModeTable
#include "bdc.h"

/* One frame of the per-mode table fade of the battle-record screen
   (`UiBattleRecord`): `dir` 0 raises the alpha (`+0xbc`, colour `+0xb0`) of
   layout sprites 6..0x92 by 1/6, `dir` 1 lowers sprites 6..0xa7 by 1/6; counts frames in `+0x70`
   and returns 1 once it has run 7 frames (0 before). */

int UiBattleRecordFadeModeTable(UiBattleRecord *self, int dir)
{
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  int i;

  if (dir == 0) {
    i = 6;
    do {
      sprites[i]->alpha = sprites[i]->alpha + 0.16666667f;
      i++;
    } while (i < 0x93);
  } else if (dir == 1) {
    i = 6;
    do {
      sprites[i]->alpha = sprites[i]->alpha - 0.16666667f;
      i++;
    } while (i < 0xa8);
  }
  if (self->animFrame >= 7) {
    return 1;
  }
  self->animFrame = self->animFrame + 1;
  return 0;
}
