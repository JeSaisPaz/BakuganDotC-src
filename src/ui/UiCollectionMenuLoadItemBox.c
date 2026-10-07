// bdc 0x08974028 UiCollectionMenuLoadItemBox
#include "bdc.h"

/* Loads the 3D item box of the collection top menu (`UiCollectionMenu`):
   allocates a 0x140-byte model from the low end of the heap (under `MemLock`) and constructs it
   from `"menu_itembox.gmo"` (`GfxModelCtor`) into `itemBox` (a failed allocation leaves
   `itemBox` NULL and the following accesses fault). Sets its root matrix to a Y rotation by
   0.5 rad (VFPU `vrot` of `0.5 * 2/pi` quarter turns) scaled by 1.2, position (240, -35, 0) copied into the matrix
   translation (w = 1), calls virtual method 6 with 0.0f, enables lighting, specular colour
   (0.4, 0.4, 0.4, 1) power 8 (`GfxModelSetSpecular`) and ambient alpha 0. Then fills the eight
   `motionNames` (`UiItemBoxGetMotionName`), loads each `"<name>.gmo"` motion file
   (`GmoMotionLoadFile`), enables motion playback, plays motion 0 at frame 0.2 without loop
   (`GfxModelPlayMotionByName`) and
   calls virtual methods 6 (with -200.0f) and 7. */

void UiCollectionMenuLoadItemBox(UiCollectionMenu *self)

{
  float scale[4];
  float specular[4];
  float *m;
  char path[72];
  GfxModel *mem;
  GfxModel *model;
  const VtblEntry *entry;
  bool fromLow;
  s32 i;

  model = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(sizeof(GfxModel), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (mem != NULL) {
    GfxModelCtor(mem, "menu_itembox.gmo", 0);
    model = mem;
  }
  self->itemBox = model;
  /* vrot of 0.5 * S703 (2/pi): a Y rotation by 0.5 rad, columns 1 and 3 from vidt. */
  m = model->data->rootMatrix;
  m[0] = __builtin_cosf(0.5f);
  m[1] = 0.0f;
  m[2] = -__builtin_sinf(0.5f);
  m[3] = 0.0f;
  m[4] = 0.0f;
  m[5] = 1.0f;
  m[6] = 0.0f;
  m[7] = 0.0f;
  m[8] = __builtin_sinf(0.5f);
  m[9] = 0.0f;
  m[10] = __builtin_cosf(0.5f);
  m[11] = 0.0f;
  m[12] = 0.0f;
  m[13] = 0.0f;
  m[14] = 0.0f;
  m[15] = 1.0f;
  scale[2] = 1.2f;
  scale[1] = 1.2f;
  scale[0] = 1.2f;
  scale[3] = 0.0f;
  /* vscl of the first three columns by scale[0..2]. */
  m = self->itemBox->data->rootMatrix;
  for (i = 0; i < 4; i++) {
    m[i] = m[i] * scale[0];
    m[4 + i] = m[4 + i] * scale[1];
    m[8 + i] = m[8 + i] * scale[2];
  }
  self->itemBox->pos[0] = 240.0f;
  self->itemBox->pos[1] = -35.0f;
  self->itemBox->pos[2] = 0.0f;
  entry = &((const VtblEntry *)self->itemBox->base.vtable)[6];
  ((void (*)(void *, float))entry->fn)((u8 *)self->itemBox + entry->delta, 0.0f);
  self->itemBox->lighting = 1;
  specular[0] = 0.4f;
  specular[1] = 0.4f;
  specular[2] = 0.4f;
  specular[3] = 1.0f;
  GfxModelSetSpecular(8.0f, self->itemBox, specular, NULL);
  model = self->itemBox;
  m = model->data->rootMatrix;
  m[12] = model->pos[0];
  m[13] = model->pos[1];
  m[14] = model->pos[2];
  m[15] = model->pos[3];
  self->itemBox->data->rootMatrix[15] = 1.0f;
  self->itemBox->ambient[3] = 0.0f;
  memset(self->motionNames, 0, 0x200);
  for (i = 0; i < 8; i++) {
    UiItemBoxGetMotionName((u8)i, self->motionNames[i]);
    sprintf(path, "%s.gmo", self->motionNames[i]);
    GmoMotionLoadFile(GmoMotionMgrGet(), path);
  }
  GfxModelEnableMotion(self->itemBox);
  GfxModelPlayMotionByName(0.2f, self->itemBox, self->motionNames[0], false);
  entry = &((const VtblEntry *)self->itemBox->base.vtable)[6];
  ((void (*)(void *, float))entry->fn)((u8 *)self->itemBox + entry->delta, -200.0f);
  entry = &((const VtblEntry *)self->itemBox->base.vtable)[7];
  ((void (*)(void *))entry->fn)((u8 *)self->itemBox + entry->delta);
}
