// bdc 0x0885bba0 BtlTargetPointLandmarkCtor
#include "bdc.h"

/* Constructor of the TargetPoint companion unit of the landmark stage object kind 0xb1
   (`ActorStageObjLandmarkCtor`, called with offsetY 700.0, radius 250.0, `withColliders` 1 and
   `childState` 2): copies `pos` (one 16-byte quad) to a stack vector and
   forwards everything to the TargetPoint base constructor
   `BtlTargetPointCtor``(offsetY, radius, unit, &copy, withColliders, childState)`,
   then installs `g_btlTargetPointLandmarkVtbl`, sets `targetPointVariant` (`+0x268`) to 2, the
   kind `+8 = 0x22` and clears `untargetable` (`+0x680`). Returns `unit`. */

BtlTargetPointLandmark *BtlTargetPointLandmarkCtor(float offsetY, float radius, BtlTargetPointLandmark *unit, const float *pos, u8 withColliders, int childState)
{
  float posCopy[4] __attribute__((aligned(16)));

  posCopy[0] = pos[0];
  posCopy[1] = pos[1];
  posCopy[2] = pos[2];
  posCopy[3] = pos[3];
  BtlTargetPointCtor(offsetY, radius, &unit->base, posCopy, (bool)withColliders, childState);
  unit->base.base.base.vtable = g_btlTargetPointLandmarkVtbl;
  unit->base.targetPointVariant = 2;
  unit->base.base.base.unk08 = 0x22;
  unit->untargetable = 0;
  return unit;
}
