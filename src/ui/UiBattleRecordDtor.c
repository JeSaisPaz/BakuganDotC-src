// bdc 0x08948748 UiBattleRecordDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of `UiBattleRecord`: restores vtable
   `g_uiBattleRecordVtbl`, waits for the GE (`GfxWaitGeIdle`), deletes the loaded layout package `+0x6c`
   (virtual dtor, flag 3), stores its id in `g_lastScreenTaskId`, then
   `UiScreenDtor`; frees the object when `flags & 1`. */

/* Header of the layout package node: only the vtable at +0x20 is used here. */
typedef struct UiBattleRecordPackage {
  u8 _unk00[0x20];
  const VtblEntry *vtbl;
} UiBattleRecordPackage;

void UiBattleRecordDtor(UiBattleRecord *self, u32 flags)

{
  if (self != (UiBattleRecord *)0x0) {
    (self->base).base.vtable = g_uiBattleRecordVtbl;
    GfxWaitGeIdle();
    if (self->package != (void *)0x0) {
      const VtblEntry *dtor = &((UiBattleRecordPackage *)self->package)->vtbl[1];

      ((void (*)(void *, int))dtor->fn)((u8 *)self->package + dtor->delta, 3);
      self->package = (void *)0x0;
    }
    g_lastScreenTaskId = (self->base).base.id;
    UiScreenDtor(&self->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}
