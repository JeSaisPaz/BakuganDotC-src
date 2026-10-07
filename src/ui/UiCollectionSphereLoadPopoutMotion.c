// bdc 0x0897db68 UiCollectionSphereLoadPopoutMotion
#include "bdc.h"

/* Loads the pop-out motion of the selected entry for model `slot` of
   `UiCollectionSphere` (`UiCollectionSphereBuildMotionName`,
   `"%s.gmo"`, `GmoMotionLoadFile`), enables motion on the model (`GfxModelEnableMotion`),
   selects the motion named by `UiCollectionSphereBuildMotionModelName` (kept in
   `motionModelName`, frame 0.2, no loop, `GfxModelPlayMotionByName`), then advances it by 0.5 (vtable +0x34)
   and updates it (vtable +0x3c). Does nothing on special pages (kind 2) or for an empty slot. */

void UiCollectionSphereLoadPopoutMotion(UiCollectionSphere *self, u8 slot)

{
  s8 category;
  u8 kind;
  u8 id;
  GfxModel *model;
  const VtblEntry *vt;
  char fileName[64];
  char motionName[64];

  id = 0;
  kind = UiCollectionSphereGetPageKind(self,(u8)self->page);
  if ((kind != 2) && (self->models[slot] != (GfxModel *)0x0)) {
    category = self->category;
    if (category < 2) {
      if (category >= 0) {
        id = self->entryIds[self->page * 6 + self->cursor];
      }
    }
    else if (category < 3) {
      if (kind == 0) {
        id = self->entryIds[(self->page / 3) * 6 + self->cursor];
      }
      else {
        id = self->kind1Ids[self->page / 3 + self->cursor];
      }
    }
    UiCollectionSphereBuildMotionName(self,(u8)category,id,motionName);
    sprintf(fileName,"%s.gmo",motionName);
    GmoMotionLoadFile(GmoMotionMgrGet(),fileName);
    GfxModelEnableMotion(self->models[slot]);
    UiCollectionSphereBuildMotionModelName(self,(u8)self->category,id,self->motionModelName);
    GfxModelPlayMotionByName(0.2f,self->models[slot],self->motionModelName,false);
    model = self->models[slot];
    vt = &((const VtblEntry *)(model->base).vtable)[6];
    ((void (*)(void *, float))vt->fn)((u8 *)model + vt->delta,0.5f);
    model = self->models[slot];
    vt = &((const VtblEntry *)(model->base).vtable)[7];
    ((void (*)(void *))vt->fn)((u8 *)model + vt->delta);
  }
  return;
}
