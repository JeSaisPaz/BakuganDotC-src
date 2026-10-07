// bdc 0x0885b854 BtlTargetPointCtor
#include "bdc.h"

/* Constructor of the model-less "TargetPoint" dummy unit: runs `BtlBakuganCtorTargetPoint`,
   installs `g_btlTargetPointVtbl`, places the unit at `pos` (also the GMO root matrix
   translation row) and sets `playerSlot` 6. When `withColliders` it builds the colliders
   (`BtlBakuganCreateColliders``(unit, offsetY, radius)`), sets their layer to `childState`
   (`BtlBakuganSetColliderLayer`), copies the GMO root matrix to `anchorMatrix` and marks each
   existing collider's `attachDirty`. Then `targetPointVariant` = 2, `stateFlags` and `isPlayer`
   cleared, `base.velocity` zeroed (VFPU bank C720 = 0) and `BtlTargetPoint` `tailFlag`
   cleared. Returns `unit`. */
BtlBakugan *BtlTargetPointCtor(float offsetY, float radius, BtlBakugan *unit, float *pos,
                               bool withColliders, int childState)
{
    int i;
    float tmp[4];

    BtlBakuganCtorTargetPoint(unit);
    unit->base.base.vtable = g_btlTargetPointVtbl;
    for (i = 0; i < 4; i++) {
        tmp[i] = pos[i];
    }
    for (i = 0; i < 4; i++) {
        unit->base.pos[i] = tmp[i];
    }
    for (i = 0; i < 4; i++) {
        tmp[i] = pos[i];
    }
    for (i = 0; i < 4; i++) {
        unit->base.data->rootMatrix[12 + i] = tmp[i];
    }
    unit->playerSlot = 6;
    if (withColliders) {
        BtlBakuganCreateColliders(unit, offsetY, radius);
        BtlBakuganSetColliderLayer(unit, childState);
        {
            float m[16];
            float *src = unit->base.data->rootMatrix;
            for (i = 0; i < 16; i++) {
                m[i] = src[i];
            }
            for (i = 0; i < 16; i++) {
                unit->anchorMatrix[i / 4][i % 4] = m[i];
            }
        }
        if (unit->collider0 != NULL) {
            unit->collider0->attachDirty = 1;
        }
        if (unit->collider1 != NULL) {
            unit->collider1->attachDirty = 1;
        }
    }
    unit->targetPointVariant = 2;
    unit->stateFlags = 0;
    unit->isPlayer = 0;
    for (i = 0; i < 4; i++) {
        unit->base.velocity[i] = 0.0f;
    }
    ((BtlTargetPoint *)unit)->tailFlag = 0;
    return unit;
}
