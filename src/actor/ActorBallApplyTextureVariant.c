// bdc 0x088b9244 ActorBallApplyTextureVariant
#include "bdc.h"

/* Replaces each material texture of a model with its numbered variant: copies the material
   texture's name, drops an existing 4-character `".NNN"` suffix when the name ends in a digit,
   appends `".%03d"` with `variant` (only when `variant > 0`) and looks the texture up
   (`GfxTryFindTexture`), binding it (`GfxModelSetMaterialTextureByIndex`) until one is
   missing. Returns 1 when every lookup succeeded, 0 when one failed or the model has no
   materials. `variant == 0` returns without touching the model; the original leaves the caller's
   `v0` (always non-zero in `ActorBallMaybeApplyTextureVariant`) and this returns 1. */

u32 ActorBallApplyTextureVariant(void *model, s32 variant)
{
    GfxModel *self = (GfxModel *)model;
    GfxTexture *texture = NULL;
    GfxTexture *material;
    char name[64];
    size_t len;
    s32 digit;
    s32 index;

    if (variant == 0) {
        return 1; /* original: caller's v0, non-zero on its only call path */
    }
    for (index = 0; index < self->materialCount; index++) {
        material = (GfxTexture *)GfxModelGetMaterialTexture(self, index);
        strcpy(name, material->name);
        len = strlen(name);
        digit = name[len - 1] - '0';
        if (digit < 0) {
            digit = -1;
        }
        if (digit >= 10) {
            digit = -1;
        }
        if (digit != -1) {
            name[len - 4] = '\0';
        }
        if (variant > 0) {
            sprintf(name, "%s.%03d", name, variant);
        }
        texture = (GfxTexture *)GfxTryFindTexture(name);
        if (texture == NULL) {
            break;
        }
        GfxModelSetMaterialTextureByIndex(self, index, texture);
    }
    return texture != NULL;
}
