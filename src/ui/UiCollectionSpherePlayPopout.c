// bdc 0x0897e7f4 UiCollectionSpherePlayPopout
#include "bdc.h"

/* In the motion view of `UiCollectionSphere` (mask bit3, not on special
   pages), Square replays the pop-out motion of the detail model (sound 0xb, rewind then half
   speed). */

void UiCollectionSpherePlayPopout(UiCollectionSphere *self)

{
  if ((self->motionButtonMask & 8) != 0 && UiCollectionSphereGetPageKind(self, self->page) != 2 &&
      (self->base.pad->pressed & 0x8000) != 0) {
    const VtblEntry *e;
    GfxModel *model;

    if (SndHasManager()) {
      SndManagerPlay(SndGetManager(), 0xb, 0, 0);
    }
    self->idleTimer = 0.0f;
    UiCollectionSphereRewindPopoutMotion(self, self->detailCell);
    model = self->models[self->detailCell];
    e = &((const VtblEntry *)model->base.vtable)[6];
    ((void (*)(void *, float))e->fn)((u8 *)model + e->delta, 0.5f);
    model = self->models[self->detailCell];
    e = &((const VtblEntry *)model->base.vtable)[7];
    ((void (*)(void *))e->fn)((u8 *)model + e->delta);
  }
  return;
}
