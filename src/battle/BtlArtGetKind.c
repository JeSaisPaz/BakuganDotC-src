// bdc 0x08888fc0 BtlArtGetKind
#include "bdc.h"

/* Returns the kind word of special art `artId` in `g_btlArtTable` (`combat` unused). Kinds seen:
   1, 2, 3 (status art, e.g. status 7 for 450 frames) and 4 (status art 6/8/9, 450–600 frames).
   The original keeps a null check on the record address that can never fire. */
s32 BtlArtGetKind(BtlCombatState *combat, s32 artId)
{
    const BtlArtRecord *art = &g_btlArtTable[artId];

    (void)combat;
    if (art != NULL) {
        return art->kind;
    }
    return 0;
}
