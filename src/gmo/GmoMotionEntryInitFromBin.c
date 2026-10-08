// bdc 0x089da5d0 GmoMotionEntryInitFromBin
#include "bdc.h"

/* Virtual method (vtable entry 11): initialises the entry from a pre-baked motion file (`name.bin`,
   found in the pack chain by `GmoMotionLoadFile`): copies the 0x54-byte header (info block, name
   and arena size) to `entry + 0x2c`, reserves the arena if needed (virtual entry 6), copies the
   remaining bytes of the file into it and relocates the track array, per-track table and every
   track's data pointer by the arena address. A NULL `bin` does nothing. */

#define HDR (sizeof(entry->info) + sizeof(entry->name) + sizeof(entry->arenaSize))

void GmoMotionEntryInitFromBin(GmoMotionEntry *entry, const void *bin)

{
  const VtblEntry *reserve;
  int i;

  if (bin != (void *)0x0) {
    memcpy(&entry->info,bin,HDR);
    if (entry->arena == (u8 *)0x0) {
      reserve = &((const VtblEntry *)((CoreNode *)entry)->vtable)[6];
      ((void (*)(void *, int))reserve->fn)((u8 *)entry + reserve->delta,entry->arenaSize);
    }
    memcpy(entry->arena,(const u8 *)bin + (HDR),entry->arenaSize);
    entry->info.tracks = PspAddr(entry->arena) + entry->info.tracks;
    entry->info.table = PspAddr(entry->arena) + entry->info.table;
    for (i = 0; i < (int)entry->info.trackCount; i++) {
      ((GmoMotionTrack *)PspPtr(entry->info.tracks))[i].data = PspAddr(entry->arena) + ((GmoMotionTrack *)PspPtr(entry->info.tracks))[i].data;
    }
  }
}
