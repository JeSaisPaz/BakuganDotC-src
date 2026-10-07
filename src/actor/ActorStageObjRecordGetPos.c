// bdc 0x088b2a80 ActorStageObjRecordGetPos
#include "bdc.h"

/* Copies the position (`+0x20`) of the first stage spawn record whose kind (`+0x32`) is `kind` to `out`, or zeroes `out` when there is none. Called by `GameStageGetTerminalPosition`. */

void ActorStageObjRecordGetPos(float *out, int kind)
{
  ActorStageObjRecord *rec;

  if (g_stageObjRecordList != (void **)0x0) {
    rec = (ActorStageObjRecord *)*g_stageObjRecordList;
    while (rec != (ActorStageObjRecord *)0x0) {
      if (rec->field32[0] == kind) {
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
