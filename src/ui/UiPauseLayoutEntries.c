// bdc 0x08910df8 UiPauseLayoutEntries
#include "bdc.h"

/* Lays out `count` pause-menu entries vertically: posY = base + step * i, with (base, step) chosen
   per count from a local table copied from `g_pauseEntryLayoutInit` whose bases are then
   overwritten; sets posY of each entry's frame sprite (layout sprite 5 + i) and label sprite
   (13 + i). `count` indexes the 8-row table unchecked. */

void UiPauseLayoutEntries(UiPause *self, int count)
{
    float layout[8][2];
    int i;

    memcpy(layout, g_pauseEntryLayoutInit, sizeof(layout));
    layout[0][0] = 126.0f;
    layout[1][0] = 126.0f;
    layout[2][0] = 102.0f;
    layout[3][0] = 86.0f;
    layout[4][0] = 78.0f;
    layout[5][0] = 70.0f;
    layout[6][0] = 66.0f;
    layout[7][0] = 54.0f;
    for (i = 0; i < count; i++) {
        float fi = (float)i;
        ((GfxSprite **)self->base.data)[5 + i]->posY = layout[count][0] + layout[count][1] * fi;
        ((GfxSprite **)self->base.data)[13 + i]->posY = layout[count][0] + layout[count][1] * fi;
    }
}
