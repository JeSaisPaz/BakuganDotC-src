// bdc 0x0889f4e8 BtlStageAmbientVecTableStaticInit
#include "bdc.h"

/* Static initialiser of the arena translation unit: fills the 40 rows of
   g_btlArenaEffectExtents with (xz, y, xz, 0) vectors, e.g. row 0 = (15000, 12000,
   15000, 0), rows 1..3 = (24000, 10500, 24000, 0), row 8 = (33000, 20000, 33000, 0);
   rows 20..39 are all (33000, 20000, 33000, 0). */

static void SetRow(s32 row, float xz, float y)
{
    g_btlArenaEffectExtents[row][0] = xz;
    g_btlArenaEffectExtents[row][1] = y;
    g_btlArenaEffectExtents[row][2] = xz;
    g_btlArenaEffectExtents[row][3] = 0.0f;
}

void BtlStageAmbientVecTableStaticInit(void)
{
    s32 row;

    SetRow(0, 15000.0f, 12000.0f);
    SetRow(1, 24000.0f, 10500.0f);
    SetRow(2, 24000.0f, 10500.0f);
    SetRow(3, 24000.0f, 10500.0f);
    SetRow(4, 24000.0f, 12000.0f);
    SetRow(5, 24000.0f, 12000.0f);
    SetRow(6, 24000.0f, 12000.0f);
    SetRow(7, 24000.0f, 12000.0f);
    SetRow(8, 33000.0f, 20000.0f);
    SetRow(9, 24000.0f, 12000.0f);
    SetRow(10, 33000.0f, 20000.0f);
    SetRow(11, 33000.0f, 20000.0f);
    SetRow(12, 33000.0f, 20000.0f);
    SetRow(13, 25500.0f, 10200.0f);
    SetRow(14, 33000.0f, 20000.0f);
    SetRow(15, 33000.0f, 20000.0f);
    SetRow(16, 28000.0f, 9000.0f);
    SetRow(17, 30000.0f, 6000.0f);
    SetRow(18, 30000.0f, 12000.0f);
    SetRow(19, 30000.0f, 12000.0f);
    for (row = 20; row < 40; row++) {
        SetRow(row, 33000.0f, 20000.0f);
    }
}
