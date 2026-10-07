// bdc 0x089c5dcc SndManagerLoadGroup
#include "bdc.h"

/* Requests that sound group `groupId` be loaded: builds its path with `SndBuildGroupPath`, and
   under the manager lock checks whether a slot with that name already exists; if not takes the
   first empty slot (`name == NULL`), prepares it with `SndManagerSetupGroupSlot` using the
   group's seGRP table name and records `groupId` in the slot. Returns 1 if the group is already
   present or was queued, 0 if there was no free slot. */

bool SndManagerLoadGroup(SndManager *mgr, s32 groupId)
{
  char *name = g_soundGroupNames[groupId];
  bool ok = false;
  char path[256];
  s32 i;

  SndBuildGroupPath(path, groupId);
  CoreLockAcquire(mgr->lock);
  for (i = 0; i < 32; i++) {
    if (mgr->groups[i].name != NULL && strcmp(mgr->groups[i].name, path) == 0) {
      ok = true;
      break;
    }
  }
  if (!ok) {
    for (i = 0; i < 32; i++) {
      if (mgr->groups[i].name == NULL) {
        SndManagerSetupGroupSlot(mgr, i, name);
        mgr->groups[i].groupId = groupId;
        ok = true;
        break;
      }
    }
  }
  CoreLockRelease(mgr->lock);
  return ok;
}
