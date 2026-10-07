// bdc 0x089d9b94 GmoMotionContains
#include "bdc.h"

/* Tests whether a motion called `name` is registered (virtual name-compare, vtable entry 5, on each
   entry). Returns 1 when found. When the manager's replace flag (`replaceExisting`) is set the
   match is instead destroyed with `GmoMotionFree` and 0 is returned, so the caller reloads it. */

bool GmoMotionContains(void *mgr, const char *name)

{
  CoreNode *entry;
  const VtblEntry *vt;

  entry = g_gmoMotionRegistry->next;
  while (entry != (CoreNode *)0x0) {
    vt = (const VtblEntry *)entry->vtable + 5;
    if (((int (*)(void *, const char *))vt->fn)((u8 *)entry + vt->delta, name) != 0) {
      if (((GmoMotionMgr *)mgr)->replaceExisting != 0) {
        GmoMotionFree(mgr, entry);
        return false;
      }
      return true;
    }
    entry = entry->next;
  }
  return false;
}
