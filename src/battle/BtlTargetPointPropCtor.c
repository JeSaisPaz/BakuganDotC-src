// bdc 0x0885ba08 BtlTargetPointPropCtor
#include "bdc.h"

/* Constructor of the TargetPoint companion unit that `ActorStageObjCtor` attaches to a scenery prop
   with HP (0x680 bytes, stored at the prop's `+0x320`): runs the TargetPoint base constructor
   `BtlTargetPointCtor` (radius 70.0, height 50.0, a copy of `pos`, no colliders, child state 5),
   installs `g_btlTargetPointPropVtbl`, casts a ray from the unit position with
   `CollisionRaycastPoint`, raises that hit point by 300 and finds the ground below it with
   `CollisionFindGroundPoint` (mask `0x3fbf2100`), puts the unit's Y 100 units above that ground
   point and sets `targetPointVariant` (`+0x268`) to 1. Returns `unit`. */

BtlBakugan *BtlTargetPointPropCtor(BtlBakugan *unit, const float *pos)
{
    float spawnPos[4] __attribute__((aligned(16)));
    float probe[4] __attribute__((aligned(16)));
    int i;

    for (i = 0; i < 4; i++)
        spawnPos[i] = pos[i];
    BtlTargetPointCtor(70.0f, 50.0f, unit, spawnPos, false, 5);
    unit->base.base.vtable = g_btlTargetPointPropVtbl;
    CollisionRaycastPoint(unit->base.pos, probe);
    probe[1] = probe[1] + 300.0f;
    CollisionFindGroundPoint(spawnPos, probe, 0x3fbf2100);
    for (i = 0; i < 4; i++)
        probe[i] = spawnPos[i];
    unit->base.pos[1] = probe[1] + 100.0f;
    unit->targetPointVariant = 1;
    return unit;
}
