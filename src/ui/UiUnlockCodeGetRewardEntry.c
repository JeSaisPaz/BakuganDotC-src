// bdc 0x089921a4 UiUnlockCodeGetRewardEntry
#include "bdc.h"

/* Copies reward entry `index` (three halfwords: kind, unlock index, value) of the 8-entry
   special-reward table `g_uiUnlockCodeRewardTable` into `out`. */

void UiUnlockCodeGetRewardEntry(u16 *out, UiUnlockCode *self, u8 index)
{
    u16 table[24];
    u16 entry[3];

    memcpy(&table[3], &g_uiUnlockCodeRewardTable, 0x30);
    entry[1] = table[3 + index * 3 + 1];
    entry[0] = table[3 + index * 3];
    entry[2] = table[3 + index * 3 + 2];
    out[0] = entry[0];
    out[1] = entry[1];
    out[2] = entry[2];
}
