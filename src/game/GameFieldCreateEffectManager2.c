// bdc 0x088beca0 GameFieldCreateEffectManager2
#include "bdc.h"

/* Like `GameFieldCreateEffectManager` for the second effect manager `+0x608` of the field task
   (id 500, `GameFieldCtor`) (no global, no pool reset); used by
   `GameFieldUpdateBackgroundBattle` for the shot effects. */

/* GCC 2.x virtual destructor call: slot 1 of the vtable, `this` adjusted by the entry delta. */
#define GF_VDELETE(obj, vtbl)                                                        \
  do {                                                                               \
    const VtblEntry *dtor_ = &((const VtblEntry *)(vtbl))[1];                        \
    ((void (*)(void *, u32))dtor_->fn)((u8 *)(obj) + dtor_->delta, 3);               \
  } while (0)

void GameFieldCreateEffectManager2(CoreTask *task, void *eset)
{
  GameFieldTask *field = (GameFieldTask *)task;
  bool fromLow;
  GfxEffectMgr *mgr;
  GfxEffectMgr *result;

  if (field->effectMgr2 != NULL) {
    GF_VDELETE(field->effectMgr2, field->effectMgr2->base.vtbl);
    field->effectMgr2 = NULL;
  }
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mgr = MemAlloc(sizeof(GfxEffectMgr), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  result = NULL;
  if (mgr != NULL) {
    GfxEffectMgrCtor(mgr, eset);
    result = mgr;
  }
  field->effectMgr2 = result;
}
