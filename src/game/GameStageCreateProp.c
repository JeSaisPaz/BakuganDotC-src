// bdc 0x088d3b5c GameStageCreateProp
#include "bdc.h"

/* Creates a static stage model (0x140 bytes allocated from the low end of the heap, `GfxModelCtor`
   with `gmoName`) and returns it. Its root matrix becomes a rotation about Y by `pos[3]` (radians)
   with translation `pos[0..2]`, w 1; the translation
   row is copied to the model position `+0x20`. A nonzero `flag` is stored in `lighting` with ambient
   (0.3, 0.3, 0.3, 1), otherwise `lighting` is cleared. Then its materials are fixed
   (`GameStagePropFixMaterials`) and it is appended to `list` (`CoreObjectListAppend`). A failed
   allocation is not checked: the NULL object is still dereferenced. */

CoreObject *GameStageCreateProp(char *gmoName, float *pos, void *list, bool flag)
{
  bool fromLow;
  GfxModel *self;
  GfxModel *obj;
  float *m;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  self = MemAlloc(0x140, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  obj = NULL;
  if (self != NULL) {
    GfxModelCtor(self, gmoName, 0);
    obj = self;
  }
  m = obj->data->rootMatrix;
  m[0] = __builtin_cosf(pos[3]);
  m[1] = 0.0f;
  m[2] = -__builtin_sinf(pos[3]);
  m[3] = 0.0f;
  m[4] = 0.0f;
  m[5] = 1.0f;
  m[6] = 0.0f;
  m[7] = 0.0f;
  m[8] = __builtin_sinf(pos[3]);
  m[9] = 0.0f;
  m[10] = __builtin_cosf(pos[3]);
  m[11] = 0.0f;
  m[12] = 0.0f;
  m[13] = 0.0f;
  m[14] = 0.0f;
  m[15] = 1.0f;
  m = &obj->data->rootMatrix[12];
  m[0] = pos[0];
  m[1] = pos[1];
  m[2] = pos[2];
  m[3] = 1.0f;
  obj->pos[0] = m[0];
  obj->pos[1] = m[1];
  obj->pos[2] = m[2];
  obj->pos[3] = m[3];
  if (flag) {
    obj->lighting = flag;
    obj->ambient[0] = 0.3f;
    obj->ambient[1] = 0.3f;
    obj->ambient[2] = 0.3f;
    obj->ambient[3] = 1.0f;
  } else {
    obj->lighting = 0;
  }
  GameStagePropFixMaterials(obj);
  CoreObjectListAppend(&obj->base, list);
  return &obj->base;
}
