// bdc 0x089585e8 UiEquipInitModelSlots
#include "bdc.h"

/* Initialises the model-viewer cameras of the Bakugan/gear loadout screen (`UiEquip`,
   `slotCameras`, one per player): each camera is reset (`GfxCameraInit`), aimed from eye
   (0, 0, 150) at the origin, given its screen offset and recomputed (`GfxCameraUpdate` all flags),
   then its vtable entry 2 is called. Two players (`playerCount` < 3): 2 cameras at screen offsets
   (-120, 0) and (120, 0); otherwise 4 cameras at (-120, -111), (120, -111), (-120, 20), (120, 20). */

void UiEquipInitModelSlots(UiEquip *self)

{
  float offsets[6][2];
  const VtblEntry *entry;
  GfxCamera *cam;
  int first;
  int count;
  int i;

  offsets[0][1] = 0.0f;
  offsets[0][0] = -120.0f;
  offsets[1][1] = 0.0f;
  offsets[1][0] = 120.0f;
  offsets[2][0] = -120.0f;
  offsets[3][0] = 120.0f;
  offsets[2][1] = -111.0f;
  offsets[3][1] = -111.0f;
  offsets[4][0] = -120.0f;
  offsets[4][1] = 20.0f;
  offsets[5][0] = 120.0f;
  offsets[5][1] = 20.0f;
  if (self->playerCount < 3) {
    first = 0;
    count = 2;
  } else {
    first = 2;
    count = 4;
  }
  for (i = 0; i < count; i++) {
    GfxCameraInit(&self->slotCameras[i]);
    cam = &self->slotCameras[i];
    cam->target[0] = 0.0f;
    cam->target[1] = 0.0f;
    cam->target[2] = 0.0f;
    cam->target[3] = 0.0f;
    cam = &self->slotCameras[i];
    cam->eye[0] = 0.0f;
    cam->eye[1] = 0.0f;
    cam->eye[2] = 150.0f;
    cam->eye[3] = 0.0f;
    GfxCameraSetScreenOffset(offsets[first + i][0], offsets[first + i][1], &self->slotCameras[i]);
    GfxCameraUpdate(&self->slotCameras[i], 0xffffffff);
    cam = &self->slotCameras[i];
    entry = &((const VtblEntry *)cam->base.vtable)[2];
    ((void (*)(void *))entry->fn)((u8 *)cam + entry->delta);
  }
  return;
}
