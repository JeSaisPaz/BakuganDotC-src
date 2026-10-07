// bdc 0x088b28ec ActorStageObjRecordSet
#include "bdc.h"

/* Fills a spawn record: position `+0x20`, kind `+0x32` (and the original kind `+0x38`), `+0x30 =
   arg`, type `+0x34`, variant `+0x36`; also sets the profile byte `+0x80` to 0 on stages 0..3 and
   to 1 on stages 8..0xb. */

void ActorStageObjRecordSet(void *rec, float *pos, s16 kind, s16 arg, s16 type, s16 variant)
{
  ActorStageObjRecord *r = rec;

  r->pos[0] = pos[0];
  r->pos[1] = pos[1];
  r->pos[2] = pos[2];
  r->pos[3] = pos[3];
  r->field32[0] = kind;
  r->field32[3] = kind;
  r->field30 = arg;
  r->field32[1] = type;
  r->field32[2] = variant;
  switch ((u32)g_scriptGlobalVars[1]) {
  case 0:
  case 1:
  case 2:
  case 3:
    SaveGetProfile()->data->stageProfileByte = 0;
    return;
  case 8:
  case 9:
  case 10:
  case 0xb:
    SaveGetProfile()->data->stageProfileByte = 1;
  }
}
