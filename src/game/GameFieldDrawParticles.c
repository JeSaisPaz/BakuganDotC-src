// bdc 0x088bee54 GameFieldDrawParticles
#include "bdc.h"

/* Particle-set draw helper of the field task (called from `GameFieldDrawScene`); byte-identical
   to `BtlDemoDrawSortedModels`. Draws the model chain starting at *set (linked through
   base.next) camera-relative and back to front. With an empty chain it returns dl unchanged.
   Otherwise it moves the active camera to the origin (`GameFieldCameraRecenterBegin`, which
   returns the original eye; a copy goes to g_gameFieldParticleSavedEye), writes the camera
   (GfxCameraDlWrite) and four GE words 0xc6000707, 0xc7000000, 0xd0000000 | (bits of 0.8f >> 8)
   and 0xc8880002, then collects each model whose ambient alpha ambient[3] (`+0x6c`) is not below
   1 (NaN counts) and whose visible byte is set, with depth = squared distance from the eye to its
   root translation (data->rootMatrix[12..14]). The pairs are sorted by descending depth
   (GfxCombSortByDepth); each model is drawn through vtable entry 8 with its root translation
   temporarily made eye-relative (xyz minus the eye, then plus the eye again). Finally the camera
   is restored (`GameFieldCameraRecenterEnd`) and written again; returns the advanced
   display-list pointer. */

typedef struct GameFieldDepthPair {
    GfxModel *model;
    float depth;
} GameFieldDepthPair;

u32 *GameFieldDrawParticles(void *owner, u32 *dl, void **set)
{
    GameFieldDepthPair pairs[64];
    float eye[4];
    union { float f; u32 u; } fogBits;
    GfxModel *model;
    float *rootPos;
    float dx;
    float dy;
    float dz;
    s32 count;
    s32 i;

    (void)owner;
    model = (GfxModel *)*set;
    count = 0;
    if (model == NULL) {
        return dl;
    }
    GameFieldCameraRecenterBegin(eye);
    g_gameFieldParticleSavedEye[0] = eye[0];
    g_gameFieldParticleSavedEye[1] = eye[1];
    g_gameFieldParticleSavedEye[2] = eye[2];
    g_gameFieldParticleSavedEye[3] = eye[3];
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
            /* depth = |eye - rootPos|^2 over xyz */
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
    GameFieldCameraRecenterEnd(eye);
    return GfxCameraDlWrite(g_gfxActiveCamera, dl, 1);
}
