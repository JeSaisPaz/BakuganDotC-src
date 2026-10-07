// bdc 0x088ff7cc BtlDemoDrawSortedModels
#include "bdc.h"

/* Draw helper of the battle intro demo task: draws the model chain starting at *chain (linked
   through base.next) camera-relative and back to front. With an empty chain it returns dl
   unchanged. Otherwise it moves the active camera to the origin (BtlDemoCameraToOrigin, which
   returns the original eye; a copy goes to g_btlDemoSavedEye), writes the camera
   (GfxCameraDlWrite) and four GE words 0xc6000707, 0xc7000000, 0xd0000000 | (bits of 0.8f >> 8)
   and 0xc8880002, then collects each model whose ambient alpha ambient[3] is not below 1 (NaN counts) and
   whose visible byte is set, with depth = squared distance from the eye to its root translation
   (data->rootMatrix[12..14]). The pairs are sorted by descending depth (GfxCombSortByDepth); each
   model is drawn through vtable entry 8 with its root translation temporarily made eye-relative
   (xyz minus the eye, then plus the eye again). Finally the camera is restored
   (BtlDemoCameraRestore) and written again; returns the advanced display-list pointer. */

typedef struct BtlDemoDepthPair {
    GfxModel *model;
    float depth;
} BtlDemoDepthPair;

u32 *BtlDemoDrawSortedModels(CoreTask *demo, u32 *dl, void **chain)
{
    BtlDemoDepthPair pairs[64];
    float eye[4];
    union { float f; u32 u; } fogBits;
    GfxModel *model;
    float *rootPos;
    float dx;
    float dy;
    float dz;
    s32 count;
    s32 i;

    (void)demo;
    model = (GfxModel *)*chain;
    count = 0;
    if (model == NULL) {
        return dl;
    }
    BtlDemoCameraToOrigin(eye);
    g_btlDemoSavedEye[0] = eye[0];
    g_btlDemoSavedEye[1] = eye[1];
    g_btlDemoSavedEye[2] = eye[2];
    g_btlDemoSavedEye[3] = eye[3];
    dl = GfxCameraDlWrite(g_gfxActiveCamera, dl, 1);
    dl[0] = 0xc6000707;
    dl[1] = 0xc7000000;
    fogBits.f = 0.800000012f;
    dl[2] = (fogBits.u >> 8) | 0xd0000000;
    dl[3] = 0xc8880002;
    dl = dl + 4;
    do {
        if (!(model->ambient[3] < 1.0f) && model->visible != 0) {
            rootPos = &model->data->rootMatrix[12];
            dx = eye[0] - rootPos[0];
            dy = eye[1] - rootPos[1];
            dz = eye[2] - rootPos[2];
            pairs[count].depth = dx * dx + dy * dy + dz * dz;
            pairs[count].model = model;
            count = count + 1;
        }
        model = (GfxModel *)model->base.next;
    } while (model != NULL);
    if (count != 0) {
        GfxCombSortByDepth(pairs, count);
        for (i = 0; i < count; i++) {
            const VtblEntry *draw;

            model = pairs[i].model;
            rootPos = &model->data->rootMatrix[12];
            rootPos[0] = rootPos[0] - eye[0];
            rootPos[1] = rootPos[1] - eye[1];
            rootPos[2] = rootPos[2] - eye[2];
            draw = &((const VtblEntry *)model->base.vtable)[8];
            ((void (*)(void *, u32 **))draw->fn)((u8 *)model + draw->delta, &dl);
            rootPos = &model->data->rootMatrix[12];
            rootPos[0] = rootPos[0] + eye[0];
            rootPos[1] = rootPos[1] + eye[1];
            rootPos[2] = rootPos[2] + eye[2];
        }
    }
    BtlDemoCameraRestore(eye);
    return GfxCameraDlWrite(g_gfxActiveCamera, dl, 1);
}
