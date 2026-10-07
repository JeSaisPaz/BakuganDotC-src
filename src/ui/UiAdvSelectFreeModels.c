// bdc 0x089178e8 UiAdvSelectFreeModels
#include "bdc.h"

/* Deletes the Bakugan model `+0x914` and the pedestal model `+0x918` of the adventure
   partner-select screen (`UiAdvSelectCtor`, task 376) through their virtual destructors
   (vtable at object `+0x14`, slot at vtable `+8`, called with flag 3), clearing both pointers. */

typedef struct AdvSelectVSlot {
  s16 adjust;
  s16 pad;
  void (*fn)(void *, s32);
} AdvSelectVSlot;

typedef struct AdvSelectVTable {
  u8 unk00[8];
  AdvSelectVSlot dtor;
} AdvSelectVTable;

typedef struct AdvSelectVObj {
  u8 unk00[0x14];
  AdvSelectVTable *vtable;
} AdvSelectVObj;

void UiAdvSelectFreeModels(UiAdvSelect *self)
{
  AdvSelectVObj *obj;
  AdvSelectVSlot *slot;

  obj = (AdvSelectVObj *)self->model;
  if (obj != NULL) {
    slot = &obj->vtable->dtor;
    slot->fn((void *)((u8 *)obj + slot->adjust), 3);
    self->model = NULL;
    self->model = NULL;
  }
  obj = (AdvSelectVObj *)self->pedestal;
  if (obj != NULL) {
    slot = &obj->vtable->dtor;
    slot->fn((void *)((u8 *)obj + slot->adjust), 3);
    self->pedestal = NULL;
    self->pedestal = NULL;
  }
}
