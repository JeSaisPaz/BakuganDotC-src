// bdc 0x089d9228 GmoMotionInfoInit
#include "bdc.h"

/* Fills a motion info record with defaults: `active = 1`, `unk02 = 0`, no track array, no `u16`
   table, `trackCount = 0`, `value0e = 1`, frame range -1000000.0 .. +1000000.0 (effectively
   unbounded), `frameRate = 60.0`, `unk1c = unk20 = 0`. `GmoMotionParse` overwrites the range and
   rate from the `0xb1`/`0xb2` sub-chunks. */

void GmoMotionInfoInit(GmoMotionInfo *info)
{
  info->active = 1;
  info->unk02 = 0;
  info->tracks = 0;
  info->table = 0;
  info->trackCount = 0;
  info->value0e = 1;
  info->startFrame = -1000000.0f;
  info->endFrame = 1000000.0f;
  info->unk1c = 0;
  info->frameRate = 60.0f;
  info->unk20 = 0;
}
