// bdc 0x08a01f58 GfxFabObjectInit
#include "bdc.h"

/* Initialises one placed object of a `.fab` clip: stores the fab, clip, placement record `def`
   and key data, the first key (`keys + (def[3] & 0xfff)`, also the key cursor), sets the colour to
   white (`g_colorWhite`), the additive colour to zero (VFPU bank C720) and the
   transform to identity, start frame `def[0]`, depth
   `-def[2]`, and resolves the referenced definition (`GfxFabFindDef`). A nested clip (kind 2,
   `GfxFabGetClip`) gets `obj` as its owner. A bitmap (kind 0/1, `GfxFabGetBitmap`) is bound;
   when it is missing the object is marked done and the function returns at once. If the texture is
   not a power-of-two size, or the used size (picture `gsTexClut` bit 2: signed 12-bit width/height,
   else the picture size) differs from it, the object gets its own quad (`g_gfxFabBitmapQuad` with
   the UVs scaled to the used area, written back from the cache) instead of `g_gfxFabDefaultQuad`. The blend mode (-1 none) comes from
   `def[3] >> 12` (2/3: 1, or the owner object's mode when this bitmap is inside a nested clip and is
   not named "fab_start00"; 5/8: 2; 9: 3), then from a `__B<c>` suffix in `keys` (A: 2, M: 1, R: 0,
   S: 3). A kind-1 bitmap adds the definition's s16 offset (`ref[4]`, `ref[5]`) to the
   translation. */

void GfxFabObjectInit(GfxFabObject *obj, GfxFab *fab, GfxFabClip *clip, const u16 *def, char *keys)
{
    const u16 *ref;
    GfxFabClip *sub;
    GfxTexture *bmp;
    const char *suffix;
    s32 width;
    s32 height;
    u32 size;
    u32 i;
    float inv;
    float scaled;

    obj->fab = fab;
    obj->clip = clip;
    obj->link = def;
    obj->keys = (const u8 *)keys;
    obj->done = 0;
    obj->keyStart = (const u8 *)keys + (def[3] & 0xfff);
    obj->color[0] = g_colorWhite.x;
    obj->color[1] = g_colorWhite.y;
    obj->color[2] = g_colorWhite.z;
    obj->color[3] = g_colorWhite.w;
    obj->colorAdd[0] = 0.0f;
    obj->colorAdd[1] = 0.0f;
    obj->colorAdd[2] = 0.0f;
    obj->colorAdd[3] = 0.0f;
    for (i = 0; i < 4; i++) {
        obj->transform[i][0] = (i == 0) ? 1.0f : 0.0f;
        obj->transform[i][1] = (i == 1) ? 1.0f : 0.0f;
        obj->transform[i][2] = (i == 2) ? 1.0f : 0.0f;
        obj->transform[i][3] = (i == 3) ? 1.0f : 0.0f;
    }
    obj->keyCursor = (const GfxFabKey *)obj->keyStart;
    obj->frame = def[0];
    obj->depth = -(float)def[2];
    ref = GfxFabFindDef(fab, def[1]);
    obj->ref = ref;
    obj->uvs = (void *)g_gfxFabDefaultQuad;
    obj->bitmap = NULL;
    obj->subClip = NULL;

    if (ref[6] == 2) {
        sub = GfxFabGetClip(fab, obj->ref[1]);
        obj->subClip = sub;
        sub->owner = obj;
    } else if (obj->ref[6] == 0 || obj->ref[6] == 1) {
        bmp = (GfxTexture *)GfxFabGetBitmap(fab, obj->ref[1]);
        obj->bitmap = bmp;
        if (bmp == NULL) {
            obj->done = 1;
            return;
        }
        width = GfxTextureGetWidth(obj->bitmap);
        height = GfxTextureGetHeight(obj->bitmap);
        if (obj->bitmap->picture->gsTexClut & 4) {
            size = obj->bitmap->picture->gsTexClut;
            width = (s32)((((size & 0x7ff8) >> 3) ^ 0x800) - 0x800);
            height = (s32)((((size & 0x7ff8000) >> 15) ^ 0x800) - 0x800);
        }
        if (obj->bitmap->isPow2Size == 0 || width != GfxTextureGetWidth(obj->bitmap) ||
            height != GfxTextureGetHeight(obj->bitmap)) {
            for (i = 0; i < 4; i++) {
                obj->quad[i] = g_gfxFabBitmapQuad[i];
            }
            scaled = (float)width * obj->bitmap->invWidth;
            obj->quad[3].u = scaled;
            obj->quad[1].u = scaled;
            inv = obj->bitmap->invHeight;
            obj->uvs = obj->quad;
            scaled = (float)height * inv;
            obj->quad[3].v = scaled;
            obj->quad[2].v = scaled;
            sceKernelDcacheWritebackRange(obj->quad, 0x30);
        }
    }

    obj->blendMode = -1;
    switch ((def[3] & 0xf000) >> 12) {
    case 2:
    case 3:
        obj->blendMode = 1;
        if (obj->subClip == NULL && obj->clip->owner != NULL &&
            obj->clip->owner->subClip != NULL &&
            strcasecmp(obj->bitmap->name, "fab_start00") != 0) {
            obj->blendMode = obj->clip->owner->blendMode;
        }
        break;
    case 5:
    case 8:
        obj->blendMode = 2;
        break;
    case 9:
        obj->blendMode = 3;
        break;
    default:
        break;
    }

    suffix = strstr(keys, "__");
    if (suffix != NULL && suffix[2] == 'B') {
        switch (suffix[3]) {
        case 'A':
            obj->blendMode = 2;
            break;
        case 'M':
            obj->blendMode = 1;
            break;
        case 'R':
            obj->blendMode = 0;
            break;
        case 'S':
            obj->blendMode = 3;
            break;
        default:
            break;
        }
    }

    if (obj->ref[6] == 1) {
        obj->transform[3][0] += (float)(s16)obj->ref[4];
        obj->transform[3][1] += (float)(s16)obj->ref[5];
    }
}
