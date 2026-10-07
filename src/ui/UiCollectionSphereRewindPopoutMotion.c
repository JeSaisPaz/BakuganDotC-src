// bdc 0x0897dd94 UiCollectionSphereRewindPopoutMotion
#include "bdc.h"

/* Rewinds the motion of model `slot` of `UiCollectionSphere` by 500 frames
   (vtable +0x34 with -500) and updates it, except on special pages. */

typedef struct PopoutVtEntryF {
  short adjust;
  short pad;
  void (*fn)(void *self, float arg);
} PopoutVtEntryF;

typedef struct PopoutVtEntry {
  short adjust;
  short pad;
  void (*fn)(void *self);
} PopoutVtEntry;

typedef struct PopoutModelVt {
  char slots[0x30];
  PopoutVtEntryF rewind;
  PopoutVtEntry update;
} PopoutModelVt;

void UiCollectionSphereRewindPopoutMotion(UiCollectionSphere *self, u8 slot)

{
  if (UiCollectionSphereGetPageKind(self,self->page) != 2) {
    GfxModel *model = self->models[slot];
    if (model != (GfxModel *)0x0) {
      const PopoutModelVt *vt = (const PopoutModelVt *)(model->base).vtable;
      vt->rewind.fn((char *)model + vt->rewind.adjust,-500.0f);
      model = self->models[slot];
      vt = (const PopoutModelVt *)(model->base).vtable;
      vt->update.fn((char *)model + vt->update.adjust);
    }
  }
  return;
}
