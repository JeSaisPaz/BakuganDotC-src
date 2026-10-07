// bdc 0x0883a768 BtlHudSetAttributeIcon
#include "bdc.h"

/* Sets a sprite to an attribute icon: copies the 6-row colour table
   `g_btlHudAttributeColours` to the stack, optionally remaps `index` through
   {0, 4, 1, 2, 3, 5} when `remap` is set, then sets the sprite's tint to the row's RGB / 256
   and alpha 1 (one 16-byte quad store through a stack temp) and
   selects cell row `index` (`GfxSpriteSetCell`). `self` is unused. */

void BtlHudSetAttributeIcon(BtlHud *self, GfxSprite *sprite, int index, bool remap)
{
    s32 colours[6][4];
    s32 remapTable[7];

    (void)self;
    memcpy(colours, g_btlHudAttributeColours, sizeof(colours));
    remapTable[0] = 0;
    remapTable[1] = 4;
    remapTable[2] = 1;
    remapTable[3] = 2;
    remapTable[4] = 3;
    remapTable[5] = 5;
    remapTable[6] = 0;
    if (remap) {
        index = remapTable[index];
    }
    sprite->tint[0] = (float)colours[index][0] * 0.00390625f;
    sprite->tint[1] = (float)colours[index][1] * 0.00390625f;
    sprite->tint[2] = (float)colours[index][2] * 0.00390625f;
    sprite->alpha = 1.0f;
    GfxSpriteSetCell(sprite, 0.0f, (float)index);
}
