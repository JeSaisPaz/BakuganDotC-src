// bdc 0x089d9780 GmoMotionFree
#include "bdc.h"

/* Destroys one registered motion entry. Entries created one by one (`entry+0x24 == 0`) are
   destroyed through the virtual destructor with `flags = 3` (free the object). Entries that are
   elements of the array built by `GmoMotionLoadFromGmo` (`+0x24 == 1`, or a pointer to the array
   start + 0x10 for the last element) are destroyed with `flags = 2` (no free), and the owning array
   block is freed for the element that carries its pointer. */

void GmoMotionFree(void *mgr, CoreNode *entry)

{
  GmoMotionRef *ref = (GmoMotionRef *)entry;
  const VtblEntry *vt;
  u32 owner = ref->arrayOwner;
  void *block;

  if (owner == 0) {
    if (entry != (CoreNode *)0x0) {
      vt = (const VtblEntry *)entry->vtable + 1;
      ((void (*)(void *, u32))vt->fn)((u8 *)entry + vt->delta, 3);
    }
  } else if (owner >= 2) {
    vt = (const VtblEntry *)entry->vtable + 1;
    ((void (*)(void *, u32))vt->fn)((u8 *)entry + vt->delta, 2);
    block = (u32 *)(uintptr_t)owner - 4; /* array block header precedes element 0 */
    if (block != (void *)0x0) {
      MemLock();
      MemFree(block, (char *)0x0, 0);
      MemUnlock();
    }
  } else {
    vt = (const VtblEntry *)entry->vtable + 1;
    ((void (*)(void *, u32))vt->fn)((u8 *)entry + vt->delta, 2);
  }
}
