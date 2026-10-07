// bdc 0x089d985c GmoMotionFreeByName
#include "bdc.h"

/* Frees every registered motion whose name matches `name`: for each entry calls the virtual
   name-compare method (vtable entry 5) and destroys matches with `GmoMotionFree`. */

void GmoMotionFreeByName(void *mgr, const char *name)

{
  CoreNode *entry;
  CoreNode *next;
  const VtblEntry *vt;

  entry = g_gmoMotionRegistry->next;
  while (entry != (CoreNode *)0x0) {
    vt = (const VtblEntry *)entry->vtable + 5;
    next = entry->next;
    if (((int (*)(void *, const char *))vt->fn)((u8 *)entry + vt->delta, name) != 0) {
      GmoMotionFree(mgr, entry);
    }
    entry = next;
  }
}
