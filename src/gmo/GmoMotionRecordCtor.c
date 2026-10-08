// bdc 0x08a14280 GmoMotionRecordCtor
#include "bdc.h"

/* In-place constructor of the 0x30-byte motion records carved by `GmoPlanTakeMotions`: reference
   count `+0x0` = 1, `+0xe` = 1, three float defaults from `0x08aa5294..0x08aa529c` into
   `+0x10/+0x14/+0x18`, the rest cleared. Also used by `GmoCreateMotionArray`. Returns `rec`
   (NULL-safe). */

void *GmoMotionRecordCtor(void *rec)

{
  GmoMotionInfo *info;

  info = (GmoMotionInfo *)rec;
  if (info != (GmoMotionInfo *)0x0) {
    info->value0e = 1;
    info->startFrame = g_gmoMotionDefaultStart;
    info->active = 1;
    info->endFrame = g_gmoMotionDefaultEnd;
    info->unk02 = 0;
    info->frameRate = g_gmoMotionDefaultRate;
    info->tracks = 0;
    info->table = 0;
    info->trackCount = 0;
    info->unk1c = 0;
    info->unk20 = 0;
  }
  return rec;
}
