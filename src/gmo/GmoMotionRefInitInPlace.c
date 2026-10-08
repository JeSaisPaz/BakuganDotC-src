// bdc 0x089da6c4 GmoMotionRefInitInPlace
#include "bdc.h"

/* Virtual method (vtable entry 11): points the reference at the pack data `bin` (`info = bin`,
   `name = bin + 0x30`, `arenaSize = bin + 0x50`, `arena = bin + 0x54`) and, the first time (guard:
   the stored track pointer is still smaller than the arena address), relocates the tracks pointer
   (`bin+4`), the table pointer (`bin+8`) and every track's data pointer by the arena address. A
   NULL `bin` does nothing. */

void GmoMotionRefInitInPlace(GmoMotionRef *ref, void *bin)

{
  GmoMotionInfo *info;
  GmoMotionTrack *tracks;
  int i;

  if (bin != (void *)0x0) {
    ref->info = bin;
    ref->name = (const char *)((GmoMotionInfo *)bin + 1);
    ref->arenaSize = (int *)ref->name + 8;
    ref->arena = (u8 *)(ref->arenaSize + 1);
    info = (GmoMotionInfo *)ref->info;
    if (info->tracks < PspAddr(ref->arena)) {
      info->tracks = PspAddr(ref->arena) + info->tracks;
      info->table = PspAddr(ref->arena) + info->table;
      info = (GmoMotionInfo *)ref->info;
      for (i = 0; i < info->trackCount; i++) {
        tracks = (GmoMotionTrack *)PspPtr(info->tracks);
        tracks[i].data = PspAddr(ref->arena) + tracks[i].data;
        info = (GmoMotionInfo *)ref->info;
      }
    }
  }
}
