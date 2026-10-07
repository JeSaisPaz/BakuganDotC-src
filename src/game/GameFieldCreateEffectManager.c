// bdc 0x088bebc4 GameFieldCreateEffectManager
#include "bdc.h"

/* Creates the main effect manager of the field task (id 500, `GameFieldCtor`) for the effect set
   `eset`: resets the effect pool (`GfxEffectModelsLoad`), deletes the previous manager `+0x604`, allocates
   a new one (0xa0 bytes, `GfxEffectMgrCtor`) and also publishes it in the global `g_worldEffectMgr`. */

/* GCC 2.x virtual destructor call: slot 1 of the vtable, `this` adjusted by the entry delta. */
#define GF_VDELETE(obj, vtbl)                                                        \
  do {                                                                               \
    const VtblEntry *dtor_ = &((const VtblEntry *)(vtbl))[1];                        \
    ((void (*)(void *, u32))dtor_->fn)((u8 *)(obj) + dtor_->delta, 3);               \
  } while (0)

void GameFieldCreateEffectManager(CoreTask *task, void *eset)
{
  GameFieldTask *field = (GameFieldTask *)task;
  bool fromLow;
  GfxEffectMgr *mgr;
  GfxEffectMgr *result;

  GfxEffectModelsLoad();
  if (field->effectMgr != NULL) {
    GF_VDELETE(field->effectMgr, field->effectMgr->base.vtbl);
    field->effectMgr = NULL;
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
  field->effectMgr = result;
  g_worldEffectMgr = result;
}
