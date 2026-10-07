// bdc 0x08917b20 UiAdvSelectUpdateModel
#include "bdc.h"

/* Calls the update slot `+0x3c` of the Bakugan model `+0x914` of the adventure partner-select
   screen (`UiAdvSelectCtor`, task 376; cursor `+0x74`, candidates `+0x8a0` as 4-byte `{id,
   partner, locked, ?}` slots) when it exists. */

typedef struct ModelVtbl {
  u8 _unk00[0x38];
  s16 thisAdjust; /* +0x38 */
  s16 pad3a;
  void (*update)(void *); /* +0x3c */
} ModelVtbl;

typedef struct ModelObj {
  u8 _unk00[0x14];
  ModelVtbl *vtable;
} ModelObj;

void UiAdvSelectUpdateModel(UiAdvSelect *self)

{
  ModelObj *model = (ModelObj *)self->model;
  if (model != NULL) {
    model->vtable->update((u8 *)model + model->vtable->thisAdjust);
  }
  return;
}
