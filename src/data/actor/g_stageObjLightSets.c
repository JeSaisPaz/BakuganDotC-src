// bdc 0x08abd3bc g_stageObjLightSets
#include "bdc.h"

__typeof__(const StageObjLightSet *[5]) g_stageObjLightSets = {
    (const struct StageObjLightSet *)&g_stageObjLightSetPlantTower01,
    (const struct StageObjLightSet *)&g_stageObjLightSetPipeUnit01,
    (const struct StageObjLightSet *)&g_stageObjLightSetLandmark01,
    (const struct StageObjLightSet *)&g_stageObjLightSetChimney02,
    (const struct StageObjLightSet *)&g_stageObjLightSetBuilding02,
};
