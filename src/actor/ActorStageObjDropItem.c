// bdc 0x088a99b0 ActorStageObjDropItem
#include "bdc.h"

/* Drops a battle item where a non-pushable stage object stood, once (`itemDropped`): casts a ray
   from 3000 above its position (`CollisionRaycastPoint`); with no hit it only sets the flag.
   Otherwise finds the ground below (`CollisionFindGroundPoint`), rolls `CoreRandNext``(100)`
   against the stage's row of `g_actorStageObjDropTable` (stage >= 40 uses row 0) and, past the
   no-drop percentage, `BtlItemRollKind`. Kind 6 (none) returns without setting the flag; any other
   kind creates the item at the ray hit point raised to 100 above ground with a 600-frame lifetime
   (`BtlItemCreate`) and sets the flag. */

void ActorStageObjDropItem(ActorStageObjBase *self)
{
  float pos[4] __attribute__((aligned(16)));
  float ground[4] __attribute__((aligned(16)));
  s32 stage;
  s32 kind;
  s32 roll;

  if (self->itemDropped != 0) {
    return;
  }
  if (ActorStageObjIsPushable(self) != 0) {
    return;
  }
  pos[0] = self->base.pos[0];
  pos[1] = self->base.pos[1];
  pos[2] = self->base.pos[2];
  pos[3] = self->base.pos[3];
  pos[1] = pos[1] + 3000.0f;
  if (CollisionRaycastPoint(pos, pos) != 0) {
    CollisionFindGroundPoint(ground, self->base.pos, 0x3fbf2100);
    pos[1] = ground[1] + 100.0f;
    kind = 6;
    stage = g_scriptGlobalVars[1];
    if (!(stage < 0x28)) {
      stage = 0;
    }
    roll = (s32)CoreRandNext(100);
    if (!(roll < g_actorStageObjDropTable[stage][0])) {
      kind = BtlItemRollKind(g_actorStageObjDropTable[stage][1], g_actorStageObjDropTable[stage][2],
                             g_actorStageObjDropTable[stage][3], 0);
    }
    if (kind == 6) {
      return;
    }
    BtlItemCreate(kind, (u32 *)pos, 600, 0, NULL);
  }
  self->itemDropped = 1;
}
