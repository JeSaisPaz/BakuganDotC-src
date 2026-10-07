// bdc 0x089c75b8 SndManagerSetupGroupSlot
#include "bdc.h"

/* Prepares group slot `slot` (0..31) of the `SndManager` for the group file `name` (a name from
   the seGRP table `g_soundGroupNames`): under the manager lock and only if the slot is empty
   (`state == 0 && bankId == -1`), looks `name` up in the first 0x53 table entries to get
   `groupId` (or leaves it -1), builds its path with `SndBuildGroupPath` into
   `SndGroupSlot.path`, points `name` at that buffer and sets the slot `state` to 1 and `isPackage`
   for `.pac` packages, or `state = 0xb` otherwise (then, if `name` contains a `.`, the extension
   is cut off the path). Finally it drops the slot's pending file-loader requests `fileReq[]`
   (`IoGetDataMng` / `IoDataMngRelease`, owner = the slot). Returns true on success, false if
   the slot index is invalid or the slot is in use. */

bool SndManagerSetupGroupSlot(SndManager *mgr, s32 slot, char *name)

{
  SndGroupSlot *group;
  char *dot;
  u32 i;
  s32 r;
  bool ok;

  ok = false;
  CoreLockAcquire(mgr->lock);
  if (slot >= 0 && slot < 0x20) {
    group = &mgr->groups[slot];
    if (group->state == 0 && group->bankId == -1) {
      group->groupId = -1;
      for (i = 0; i < 0x53; i++) {
        if (strcmp(name, g_soundGroupNames[i]) == 0) {
          group->groupId = (s32)i;
          break;
        }
      }
      SndBuildGroupPath(group->path, group->groupId);
      group->name = group->path;
      if (strstr(name, ".pac") == NULL) {
        group->state = 0xb;
        group->isPackage = 0;
        if (strstr(name, ".") != NULL) {
          dot = group->name;
          if (dot != name) {
            strcat(group->path, name);
            group->name = group->path;
            dot = group->path;
          }
          dot = strstr(dot, ".");
          if (dot != NULL) {
            *dot = '\0';
          }
        }
      }
      else {
        group->state = 1;
        group->isPackage = 1;
      }
      for (r = 0; r < 3; r++) {
        if (group->fileReq[r] != NULL) {
          IoDataMngRelease(IoGetDataMng(), group, group->fileReq[r]);
          group->fileReq[r] = NULL;
        }
      }
      ok = true;
    }
  }
  CoreLockRelease(mgr->lock);
  return ok;
}
