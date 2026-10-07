// bdc 0x08abd48c g_btlStagePlacementTables
#include "bdc.h"

__typeof__(BtlStagePlacementList *[5]) g_btlStagePlacementTables = {
    (struct BtlStagePlacementList *)&g_btlStagePlacementTables00,
    (struct BtlStagePlacementList *)&g_gameStageLayoutTables00,
    (struct BtlStagePlacementList *)&g_gameStageLayoutTables00,
    (struct BtlStagePlacementList *)&g_gameStageLayoutTables00,
    (struct BtlStagePlacementList *)&g_gameStageLayoutTables00,
};
