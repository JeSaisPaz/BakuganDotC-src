// bdc 0x089b1f60 GameStageGetData3c
#include "bdc.h"

/* Copies the 0x3c-byte record of a story stage from `g_gameStageRecords` (index
   `g_gameStageRecAreaBase[area] + slot`) into `out`, through a stack copy. */

void GameStageGetData3c(void *out, u8 area, u8 slot)
{
    s16 tmp[30];
    const s16 *src;
    s16 *dst = (s16 *)out;
    int i;

    src = (const s16 *)(g_gameStageRecords + (g_gameStageRecAreaBase[area] + slot) * 0x3c);
    for (i = 0; i < 30; i++) {
        tmp[i] = src[i];
    }
    for (i = 0; i < 30; i++) {
        dst[i] = tmp[i];
    }
}
