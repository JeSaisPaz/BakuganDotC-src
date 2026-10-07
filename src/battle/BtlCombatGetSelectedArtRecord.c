// bdc 0x08889068 BtlCombatGetSelectedArtRecord
#include "bdc.h"

/* Returns the `g_btlArtTable` row of the selected special art (`selectedArt`) of a
   `BtlCombatState`, or NULL when none is selected (-1). */
void *BtlCombatGetSelectedArtRecord(BtlCombatState *combat)
{
    if (combat->selectedArt != -1) {
        return &g_btlArtTable[combat->selectedArt];
    }
    return NULL;
}
