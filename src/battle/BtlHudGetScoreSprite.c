// bdc 0x0883357c BtlHudGetScoreSprite
#include "bdc.h"

/* Returns HUD sprite `0x3d + player * 3 + part` of the score board: part 0 is the player's score
   icon, parts 1 and 2 the two halves of the "+N" gain popup. */
GfxSprite *BtlHudGetScoreSprite(BtlHud *self, s32 part, s32 player)
{
    return self->sprites[player * 3 + part + 0x3d];
}
