// bdc 0x08abf0b0 g_gameStageSpawnTables
#include "bdc.h"

__typeof__(GameStageSpawnPointList *[4]) g_gameStageSpawnTables = {
    (struct GameStageSpawnPointList *)&g_gameStageSpawnTables00,
    (struct GameStageSpawnPointList *)&g_gameStageSpawnTables01,
    (struct GameStageSpawnPointList *)&g_gameStageSpawnTables02,
    (struct GameStageSpawnPointList *)&g_gameStageSpawnTables03,
};
