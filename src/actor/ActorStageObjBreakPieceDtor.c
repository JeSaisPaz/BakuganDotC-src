// bdc 0x088af960 ActorStageObjBreakPieceDtor
#include "bdc.h"

/* Destructor (vtable `0x08af29a4` slot 1) of the debris piece (`ActorStageObjBreakPieceCtor`):
   chains to `ActorStageObjBaseDtor`. (GCC 2.x deleting destructor: frees the object when bit 0 of
   `flags` is set). */

void ActorStageObjBreakPieceDtor(ActorStageObjBreakPiece *self, u32 flags)

{
  if (self != (ActorStageObjBreakPiece *)0x0) {
    (self->base).base.base.vtable = g_actorStageObjBreakPieceVtbl;
    ActorStageObjBaseDtor(&self->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

