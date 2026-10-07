// bdc 0x0897dd34 UiCollectionSphereUpdatePopoutMotion
#include "bdc.h"

/* Updates the model in slot `slot` of `UiCollectionSphere` (vtable +0x3c)
   except on special pages. */

typedef struct ModelVtblEntry {
  s16 thisAdjust;
  s16 _pad;
  void (*fn)(void *self);
} ModelVtblEntry;

typedef struct ModelVtbl {
  u8 _unk00[0x38];
  ModelVtblEntry update; /* +0x38 this-adjust, +0x3c function */
} ModelVtbl;

void UiCollectionSphereUpdatePopoutMotion(UiCollectionSphere *self, u8 slot)

{
  GfxModel *model;
  const ModelVtbl *vtbl;

  if (UiCollectionSphereGetPageKind(self, self->page) != 2) {
    model = self->models[slot];
    if (model != (GfxModel *)0) {
      vtbl = (const ModelVtbl *)model->base.vtable;
      vtbl->update.fn((u8 *)model + vtbl->update.thisAdjust);
    }
  }
}
