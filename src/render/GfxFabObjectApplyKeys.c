// bdc 0x08a02380 GfxFabObjectApplyKeys
#include "bdc.h"

/* Per-frame update of a placed `.fab` object (`GfxFabObject`): an object marked `done` deletes
   itself (virtual deleting dtor, flags 3) and returns. Otherwise, while the current `frame` is not
   past the placement's last frame (`link[5]`), applies every key (`GfxFabKey`) from `keyCursor`
   whose frame equals `frame` (stopping at the first later key, at most `link[4]` keys), stores the
   advanced cursor and finally increments `frame`. Key types: 0 resets the transform to identity
   (VFPU `vmidt`), adds the bitmap offset for a kind-1 definition, scales
   the diagonal and sets the translation; 1 sets the 2x3 part of the transform (x/y rows scaled,
   translation plain); 2 sets `color`; 3 sets `colorAdd`; 4 marks the object `done` and returns at
   once (cursor and frame are left as they are). Bitmap keys are scaled by twice the bitmap size
   (`ref[2]`/`ref[3]`), nested clips (`subClip` set) by 1. Types >= 5 are skipped. Called by
   `GfxFabClipUpdate`. */
void GfxFabObjectApplyKeys(GfxFabObject *obj)
{
    const u16 *def;
    const u16 *ref;
    const GfxFabKey *key;
    const float *data;
    float sx;
    float sy;
    u16 i;

    if (obj->done != 0) {
        if (obj != NULL) {
            /* virtual deleting destructor: vtable entry 1, flags 3 */
            const VtblEntry *dtor = &((const VtblEntry *)obj->base.vtable)[1];
            ((void (*)(void *, s32))dtor->fn)((u8 *)obj + dtor->delta, 3);
        }
        return;
    }
    def = obj->link;
    if (def == NULL)
        return;
    if (obj->frame <= def[5]) {
        key = obj->keyCursor;
        for (i = 0; i < def[4]; i++) {
            if (obj->frame < key->frame)
                break;
            if (obj->frame == key->frame) {
                data = (const float *)(key + 1);
                if (obj->subClip != NULL) {
                    sx = 1.0f;
                    sy = 1.0f;
                } else {
                    sx = (float)obj->ref[2] * 2.0f;
                    sy = (float)obj->ref[3] * 2.0f;
                }
                switch (key->type) {
                case 0:
                    obj->transform[0][0] = 1.0f; obj->transform[0][1] = 0.0f;
                    obj->transform[0][2] = 0.0f; obj->transform[0][3] = 0.0f;
                    obj->transform[1][0] = 0.0f; obj->transform[1][1] = 1.0f;
                    obj->transform[1][2] = 0.0f; obj->transform[1][3] = 0.0f;
                    obj->transform[2][0] = 0.0f; obj->transform[2][1] = 0.0f;
                    obj->transform[2][2] = 1.0f; obj->transform[2][3] = 0.0f;
                    obj->transform[3][0] = 0.0f; obj->transform[3][1] = 0.0f;
                    obj->transform[3][2] = 0.0f; obj->transform[3][3] = 1.0f;
                    ref = obj->ref;
                    if (ref[6] == 1) {
                        obj->transform[3][0] = obj->transform[3][0] + (float)(s16)ref[4];
                        obj->transform[3][1] = obj->transform[3][1] + (float)(s16)ref[5];
                    }
                    obj->transform[0][0] = obj->transform[0][0] * sx;
                    obj->transform[1][1] = obj->transform[1][1] * sy;
                    obj->transform[3][0] = data[0];
                    obj->transform[3][1] = data[1];
                    break;
                case 1:
                    ref = obj->ref;
                    if (ref[6] == 1) {
                        obj->transform[3][0] = obj->transform[3][0] + (float)(s16)ref[4];
                        obj->transform[3][1] = obj->transform[3][1] + (float)(s16)ref[5];
                    }
                    obj->transform[0][0] = data[0] * sx;
                    obj->transform[1][0] = data[1] * sy;
                    obj->transform[3][0] = data[2];
                    obj->transform[0][1] = data[3] * sx;
                    obj->transform[1][1] = data[4] * sy;
                    obj->transform[3][1] = data[5];
                    break;
                case 2:
                    obj->color[0] = data[0];
                    obj->color[1] = data[1];
                    obj->color[2] = data[2];
                    obj->color[3] = data[3];
                    break;
                case 3:
                    obj->colorAdd[0] = data[0];
                    obj->colorAdd[1] = data[1];
                    obj->colorAdd[2] = data[2];
                    obj->colorAdd[3] = data[3];
                    break;
                case 4:
                    obj->done = 1;
                    return;
                default:
                    break;
                }
            }
            key = (const GfxFabKey *)((const u8 *)key + sizeof(GfxFabKey) + key->size);
        }
        obj->keyCursor = key;
    }
    obj->frame = obj->frame + 1;
}
