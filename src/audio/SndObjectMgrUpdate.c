// bdc 0x089c2628 SndObjectMgrUpdate
#include "bdc.h"

/* Per-frame update of all sound objects, called by `BootEndOfFrame` before the emitter update:
   walks the linked list headed at `list+0x24` (next pointer at `+4`) and runs `SndObjectUpdate`
   on each; an object that reports 0 is deleted: if it belongs to the pool `g_soundObjectMgr[1]`
   (`MemPoolFree` succeeds) its virtual destructor is called with flag 2, otherwise with flag 3
   (delete). */


static void CallDtor(CoreNode *object, int flags)
{
  const VtblEntry *e = (const VtblEntry *)((const VtblEntry *)object->vtable + 1);

  ((void (*)(void *, int))e->fn)((char *)object + e->delta,flags);
}

void SndObjectMgrUpdate(void *list)

{
  CoreNode *object = ((CoreNodeOwner *)list)->head;

  while (object != (CoreNode *)0x0) {
    CoreNode *next;
    s32 alive = SndObjectUpdate(object);

    next = object->next;
    if (alive == 0) {
      if ((g_soundObjectMgr[1] != (void *)0x0) && MemPoolFree((MemPool *)g_soundObjectMgr[1],object)) {
        CallDtor(object,2);
        object = (CoreNode *)0x0;
      }
      if (object != (CoreNode *)0x0) {
        CallDtor(object,3);
      }
    }
    object = next;
  }
  return;
}
