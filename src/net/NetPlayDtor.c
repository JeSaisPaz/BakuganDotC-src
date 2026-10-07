// bdc 0x0881b0c4 NetPlayDtor
#include "bdc.h"

/* Destructor of the `NetPlay` manager: deletes the remote pad object `+0xe4` through
   its virtual destructor, destroys the `"CONetPlay"` lock (`CoreLockDestroy`) and frees the
   object when `flags & 1`. Called by `NetPlayShutdown`. */

void NetPlayDtor(NetPlay *self, u32 flags)
{
  PadState *pad;

  if (self != (NetPlay *)0x0) {
    pad = self->remotePad;
    if (pad != (PadState *)0x0) {
      const struct { u8 hdr[8]; s16 adj; s16 pad; void (*dtor)(void *, int); } *vt = pad->vtable;
      vt->dtor((u8 *)pad + vt->adj, 3);
      self->remotePad = (PadState *)0x0;
    }
    if (self->lock != (CoreLock *)0x0) {
      CoreLockDestroy(self->lock, 3);
      self->lock = (CoreLock *)0x0;
    }
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self, (char *)0x0, 0);
      MemUnlock();
    }
  }
}
