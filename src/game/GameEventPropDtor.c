// bdc 0x088ea3ac GameEventPropDtor
#include "bdc.h"

/* Destructor of the event prop: destroys its model object `+0` through its virtual destructor and
   frees the prop when `flags & 1`. */


typedef struct {
  s16 adj;
  s16 pad;
  void (*dtor)(void *obj, s32 flags);
} GameEventPropVEntry;

typedef struct {
  u8 pad[8];
  GameEventPropVEntry dtorEntry;
} GameEventPropVTable;

typedef struct {
  u8 pad[0x14];
  GameEventPropVTable *vtable;
} GameEventPropModel;

void GameEventPropDtor(void *prop, u32 flags)

{
  GameEventPropModel *model;

  if (prop != NULL) {
    model = *(GameEventPropModel **)prop;
    if (model != NULL) {
      GameEventPropVEntry *e = &model->vtable->dtorEntry;
      e->dtor((u8 *)model + e->adj, 3);
      *(void **)prop = NULL;
    }
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(prop, NULL, 0);
      MemUnlock();
    }
  }
}
