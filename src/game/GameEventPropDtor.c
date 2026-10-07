// bdc 0x088ea3ac GameEventPropDtor
#include "bdc.h"

/* Destructor of the event prop: destroys its model object `+0` through its virtual destructor and
   frees the prop when `flags & 1`. The model is a CoreObject (`GameEventPropReleaseModel` unlinks
   and defer-deletes it as one). */

void GameEventPropDtor(void *prop, u32 flags)

{
  GameEventProp *p = (GameEventProp *)prop;
  CoreObject *model;

  if (p != NULL) {
    model = (CoreObject *)p->model;
    if (model != NULL) {
      const VtblEntry *dtor = &((const VtblEntry *)model->vtable)[1];

      ((void (*)(void *, s32))dtor->fn)((u8 *)model + dtor->delta, 3);
      p->model = NULL;
    }
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(prop, NULL, 0);
      MemUnlock();
    }
  }
}
