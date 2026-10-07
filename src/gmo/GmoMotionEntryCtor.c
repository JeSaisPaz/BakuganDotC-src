// bdc 0x08a3226c GmoMotionEntryCtor
#include "bdc.h"

/* Constructor of a `GmoMotionEntry` (the element constructor passed to `CxxVecNew` by
   `GmoMotionLoadFromGmo`): runs `CoreNodeCtor` (no anchor), clears `arrayOwner` (`+0x24`), sets
   `pinned = 1` (`+0x28`), installs the vtable `0x08af53c4` and clears the arena pointer (`+0x80`).
   Returns `entry`. */

GmoMotionEntry *GmoMotionEntryCtor(GmoMotionEntry *entry)

{
  CoreNodeCtor((CoreNode *)entry,(CoreNode *)0x0);
  entry->arrayOwner = (void *)0x0;
  entry->pinned = '\x01';
  ((CoreNode *)entry)->vtable = g_gmoMotionEntryVtable;
  entry->arena = (u8 *)0x0;
  return entry;
}

