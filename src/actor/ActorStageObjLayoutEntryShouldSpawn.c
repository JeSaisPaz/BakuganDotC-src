// bdc 0x088b75fc ActorStageObjLayoutEntryShouldSpawn
#include "bdc.h"

/* Decides whether a 12-byte stage layout entry (`BtlStagePlacement`) is spawned in the current
   mode (filter of `BtlStageSpawnPlacedObjects`): returns false for a NULL entry or without a profile
   (`SaveHasProfile`); by its group (`typeGroup` bits 2..7): groups 1, 6, 9, 11 never, group 5
   always, group 12 only when script global 8 (`g_scriptGlobalVars[8]`) is 2; every other group is
   spawned unless that global is 2 and either `variant` bits 6..7 equal 2 or bit 0 of `typeGroup`
   is set. */

bool ActorStageObjLayoutEntryShouldSpawn(BtlStagePlacement *entry)
{
    if (entry == NULL || !SaveHasProfile()) {
        return false;
    }
    switch ((entry->typeGroup & 0xfc) >> 2) {
    case 1:
    case 6:
    case 9:
    case 11:
        return false;
    case 5:
        return true;
    case 12:
        return g_scriptGlobalVars[8] == 2;
    default:
        break;
    }
    if ((entry->variant & 0xc0) == 0x80 && g_scriptGlobalVars[8] == 2) {
        return false;
    }
    if ((entry->typeGroup & 1) != 0 && g_scriptGlobalVars[8] == 2) {
        return false;
    }
    return true;
}
