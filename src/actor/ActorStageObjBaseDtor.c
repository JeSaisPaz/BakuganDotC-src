// bdc 0x088ab488 ActorStageObjBaseDtor
#include "bdc.h"

/* Base destructor of the stage objects (`ActorStageObjBaseCtor`): reinstalls the base vtable,
   releases its layout spawn record `+0x154` (`ActorStageObjReleaseRecord`), frees the buffer
   `+0x318`, destroys the collider `+0x140` and the HP gauge `+0x290` (virtual destructor, vtable
   at their `+0x20`, flags 3), frees `+0x164` and runs `GfxModelDtor`. (GCC 2.x deleting
   destructor: frees the object when bit 0 of `flags` is set). */

/* Virtual delete through slot 1 of a CoreNode-based object's vtable (at its +0x20), flags 3. */
static void StageObjVDelete(void *obj)
{
  const VtblEntry *dtor = &((const VtblEntry *)((CoreNode *)obj)->vtable)[1];
  ((void (*)(void *, u32))dtor->fn)((u8 *)obj + dtor->delta, 3);
}

void ActorStageObjBaseDtor(ActorStageObjBase *self, u32 flags)
{
  if (self == NULL)
    return;
  self->base.base.vtable = g_actorStageObjBaseVtbl;
  if (self->record != NULL) {
    ActorStageObjReleaseRecord(self->record);
    self->record = NULL;
  }
  if (self->buffer != NULL) {
    void *buf = self->buffer;
    MemLock();
    MemFree(buf, NULL, 0);
    MemUnlock();
    self->buffer = NULL;
  }
  if (self->collider != NULL) {
    StageObjVDelete(self->collider);
    self->collider = NULL;
  }
  if (self->hpGauge != NULL) {
    StageObjVDelete(self->hpGauge);
    self->hpGauge = NULL;
  }
  if (self->lights != NULL) {
    struct GfxSprite **lights = self->lights;
    MemLock();
    MemFree(lights, NULL, 0);
    MemUnlock();
    self->lights = NULL;
  }
  GfxModelDtor(&self->base, 0);
  if ((flags & 1) != 0) {
    MemLock();
    MemFree(self, NULL, 0);
    MemUnlock();
  }
}
