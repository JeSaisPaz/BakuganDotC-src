// bdc 0x0889d344 BtlStageGetPlacementList
#include "bdc.h"

/* Returns the placement list of kind `kind` for the current arena: row `g_btlArenaIndex` of
   `g_btlStagePlacementTables``[kind]`. Used by `BtlStageSpawnPlacedObjects`. */
BtlStagePlacementList *BtlStageGetPlacementList(s32 kind)
{
    return &g_btlStagePlacementTables[kind][g_btlArenaIndex];
}
