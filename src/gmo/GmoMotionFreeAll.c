// bdc 0x089d9710 GmoMotionFreeAll
#include "bdc.h"

/* Frees registered motions: every entry when `all != 0`, otherwise only the entries whose
   `pinned` byte (`+0x28`) is set (set to 1 on construction, so effectively all entries that were
   not pinned). Walks the chain behind `g_gmoMotionRegistry`, using `GmoMotionFree`. */

void GmoMotionFreeAll(void *mgr, u8 all)

{
  CoreNode *entry;
  CoreNode *next;

  entry = g_gmoMotionRegistry->next;
  while (entry != (CoreNode *)0x0) {
    next = entry->next;
    if (all != 0 || ((GmoMotionEntry *)entry)->pinned != 0) {
      GmoMotionFree(mgr, entry);
    }
    entry = next;
  }
}
