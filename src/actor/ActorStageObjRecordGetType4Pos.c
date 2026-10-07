// bdc 0x088b3220 ActorStageObjRecordGetType4Pos
#include "bdc.h"

/* Copies the position of the type-4 spawn record (`+0x34 == 4`) with id `id` (`+0x30`) to `out`, or zeroes it. Type 4 records are crystal points: the callers are the crystal code (`ActorCrystalSpawnAtPoint`, `ActorCrystalStandSpawnAtPoint`, `ActorCrystalRegenerate`). Sibling of `ActorStageObjRecordGetType8Pos` (type 8). */

void ActorStageObjRecordGetType4Pos(float *out, int id)
{
  ActorStageObjRecord *rec;

  if (g_stageObjRecordList != (void **)0x0) {
    rec = (ActorStageObjRecord *)*g_stageObjRecordList;
    while (rec != (ActorStageObjRecord *)0x0) {
      if (rec->field32[1] == 4 && rec->field30 == id) {
        out[0] = rec->pos[0];
        out[1] = rec->pos[1];
        out[2] = rec->pos[2];
        out[3] = rec->pos[3];
        return;
      }
      rec = (ActorStageObjRecord *)rec->base.next;
    }
  }
  out[2] = 0.0f;
  out[1] = 0.0f;
  out[0] = 0.0f;
  out[3] = 0.0f;
}
