// bdc 0x088b1060 ActorStageObjPropCheckLanded
#include "bdc.h"

/* Keeps the physics box `+0x334` of a flying prop above the ground (raycast from 9999 above,
   `CollisionRaycastPoint`, when the floor height differs by more than 20) and, on stage type 4
   (`0x08b002b4`), returns 1 once the box's lowest corner (`CollisionPhysBoxLowestCorner`) is at
   or below the water level (`BtlStageGetWaterBedHeight`). */

int ActorStageObjPropCheckLanded(ActorStageObjProp *self)

{
  __attribute__((aligned(16))) float pos[4];
  const float *root;
  CollisionPhysBox *box;
  float water;

  root = &self->base.base.data->rootMatrix[12];
  pos[0] = root[0];
  pos[1] = root[1];
  pos[2] = root[2];
  pos[3] = root[3];
  pos[1] = pos[1] + 9999.0f;
  if (CollisionRaycastPoint(pos, pos) != 0) {
    box = (CollisionPhysBox *)self->physicsBox;
    if (!(fabsf(box->floorY - pos[1]) <= 20.0f)) {
      box = (CollisionPhysBox *)self->physicsBox;
      box->floorY = pos[1];
    }
  }
  if (g_collisionHitInfo.surface == 4) {
    water = BtlStageGetWaterBedHeight();
    if (!(water < CollisionPhysBoxLowestCorner((CollisionPhysBox *)self->physicsBox)->y)) {
      return 1;
    }
  }
  return 0;
}
