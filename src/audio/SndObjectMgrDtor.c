// bdc 0x08a2f7d0 SndObjectMgrDtor
#include "bdc.h"

/* Destructor of the sound-object manager's list-owner object (the 0x30-byte `CoreNodeOwner`
   subclass built by `SndObjectMgrCreate`, vtable `0x08af6fd8`): does nothing for NULL; resets the
   vtable word at `+0x20` to `0x08af6fd8`, runs the base destructor `CoreNodeOwnerDtor``(mgr, 0)`
   (which destroys all owned nodes) and, when bit 0 of `flags` is set, frees the object (`MemFree`
   under `MemLock`). */

void SndObjectMgrDtor(CoreNodeOwner *mgr, u32 flags)

{
  if (mgr != (CoreNodeOwner *)0x0) {
    mgr->vtable = g_sndObjectMgrVtbl;
    CoreNodeOwnerDtor(mgr,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(mgr,NULL,0);
      MemUnlock();
    }
  }
  return;
}

