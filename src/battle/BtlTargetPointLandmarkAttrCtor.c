// bdc 0x0885bad4 BtlTargetPointLandmarkAttrCtor
#include "bdc.h"

/* Constructor of the TargetPoint companion unit of the hologram landmark stage object
   (`ActorStageObjAttrLandmarkCtor`): same sequence as `BtlTargetPointPropCtor` — TargetPoint
   base constructor `BtlTargetPointCtor` (radius 70.0, height 50.0, a copy of `pos`, no colliders,
   child state 5), installs `g_btlTargetPointLandmarkAttrVtbl`, position raised to 100 units above
   the ground under the unit (`CollisionRaycastPoint`, hit point + 300, `CollisionFindGroundPoint`
   mask `0x3fbf2100`), `targetPointVariant` (`+0x268`) = 1. Returns `unit`. */
BtlBakugan *BtlTargetPointLandmarkAttrCtor(BtlBakugan *unit, const float *pos)
{
    float spawnPos[4] __attribute__((aligned(16)));
    float probe[4] __attribute__((aligned(16)));
    int i;

    for (i = 0; i < 4; i++)
        spawnPos[i] = pos[i];
    BtlTargetPointCtor(70.0f, 50.0f, unit, spawnPos, false, 5);
    unit->base.base.vtable = g_btlTargetPointLandmarkAttrVtbl;
    CollisionRaycastPoint(unit->base.pos, probe);
    probe[1] = probe[1] + 300.0f;
    CollisionFindGroundPoint(spawnPos, probe, 0x3fbf2100);
    for (i = 0; i < 4; i++)
        probe[i] = spawnPos[i];
    unit->base.pos[1] = probe[1] + 100.0f;
    unit->targetPointVariant = 1;
    return unit;
}
