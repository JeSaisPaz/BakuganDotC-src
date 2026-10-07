// bdc 0x0889c998 BtlStageCreateMapModel
#include "bdc.h"

/* Creates an arena model object: allocates 0x140 bytes from the low heap (under `MemLock`), runs
   `GfxModelCtor` with `name`, writes the model's root matrix as a rotation about Y by `pose[3]`
   (cos/sin of `pose[3]` in radians) with translation `pose[0..2]` (w 1.0), then, when
   `dimmed` is set, enables `lighting` with ambient 0.3/0.3/0.3/1.0, else clears `lighting`;
   appends it to `list` (`CoreObjectListAppend`) and returns it. A failed allocation is not
   checked: the model is then NULL. Used by `BtlStageLoadMap` for `battle_map.gmo` and the subset
   models. */
CoreObject *BtlStageCreateMapModel(const char *name, float *pose, void *list, u8 dimmed)
{
    bool fromLow;
    GfxModel *mem;
    GfxModel *model;
    float *matrix;
    float c;
    float s;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(sizeof(GfxModel), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    model = NULL;
    if (mem != NULL) {
        GfxModelCtor(mem, name, 0);
        model = mem;
    }
    /* rootMatrix = rotation about Y by pose[3] (vmul.s by S703 = 2/pi, then vrot.q) */
    matrix = model->data->rootMatrix;
    c = __builtin_cosf(pose[3]);
    s = __builtin_sinf(pose[3]);
    matrix[0] = c;
    matrix[1] = 0.0f;
    matrix[2] = -s;
    matrix[3] = 0.0f;
    matrix[4] = 0.0f;
    matrix[5] = 1.0f;
    matrix[6] = 0.0f;
    matrix[7] = 0.0f;
    matrix[8] = s;
    matrix[9] = 0.0f;
    matrix[10] = c;
    matrix[11] = 0.0f;
    matrix[12] = 0.0f;
    matrix[13] = 0.0f;
    matrix[14] = 0.0f;
    matrix[15] = 1.0f;
    matrix = model->data->rootMatrix;
    matrix[12] = pose[0];
    matrix[13] = pose[1];
    matrix[14] = pose[2];
    matrix[15] = 1.0f;
    if (dimmed != 0) {
        model->lighting = 1;
        model->ambient[0] = 0.300000012f;
        model->ambient[1] = 0.300000012f;
        model->ambient[2] = 0.300000012f;
        model->ambient[3] = 1.0f;
    } else {
        model->lighting = 0;
    }
    CoreObjectListAppend(&model->base, list);
    return &model->base;
}
