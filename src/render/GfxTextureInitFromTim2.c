// bdc 0x089f6e48 GfxTextureInitFromTim2
#include "bdc.h"

/* Initialises a 0x140-byte texture object from an in-memory TIM2 image: zeroes the sub-records at
   `rec38Type` and `gmo`; when `data` starts with the `TIM2` magic (`0x324d4954`) points
   `picture` at `data + 0x10` (or `data + 0x80` when the header's alignment byte is non-zero) and
   takes `name` without extension, truncated to 31 characters, as the texture name (otherwise
   neither is touched). Rounds width/height to powers of two (`GfxTextureRoundPow2`), derives
   the palette count from the CLUT colours (/16 for 4-bit, else /256), gives the texture private GE
   slots (`GfxTextureAllocSlots`, `flag` = from the low heap end) when there is more than one
   palette, builds the GE block (`GfxTextureBuildDl`), records whether the size already was a
   power of two, stores `1/width`/`1/height`, remembers `data` in `tim2` (not owned) and becomes
   `g_textureList` when the list is empty, else is appended to it (`CoreObjectAppend`).
   Returns nothing. */

void GfxTextureInitFromTim2(void *tex, const char *name, void *data, u8 flag)
{
    GfxTexture *t = (GfxTexture *)tex;
    GfxTim2Header *hdr = (GfxTim2Header *)data;
    char buf[0x140];
    char *dot;
    u32 width;
    u32 height;
    s32 palettes;

    memset(&t->rec38Type, 0, 0x20);
    t->rec38Type = 1;
    t->rec38Owner = t;
    t->clutData = NULL;
    t->singleSlot = 1;
    t->blocks = t->inlineBlock;
    memset(&t->gmo, 0, sizeof(GmoTexture));
    t->gmo.refCount = 1;
    t->gmo.palette = t;
    if (hdr->magic == 0x324d4954) {
        t->picture = (GfxTim2Picture *)(hdr + 1);
        if (hdr->format != 0) {
            /* 128-byte aligned layout: picture header at data + 0x80 */
            t->picture = (GfxTim2Picture *)(hdr + 0x80 / sizeof(GfxTim2Header));
        }
        strcpy(buf, name);
        dot = strrchr(buf, '.');
        if (dot != NULL) {
            *dot = '\0';
        }
        if (strlen(buf) >= 0x20) {
            buf[0x1f] = '\0';
        }
        strcpy(t->name, buf);
    }
    t->vramBlock1 = NULL;
    t->vramBlock0 = NULL;
    width = GfxTextureRoundPow2(t, GfxTextureGetWidth(t));
    height = GfxTextureRoundPow2(t, GfxTextureGetHeight(t));
    t->paletteCount = GfxTextureGetClutColors(t);
    if (GfxTextureGetPsm(t) == 4) { /* 4-bit CLUT */
        palettes = t->paletteCount / 16;
    } else {
        palettes = t->paletteCount / 256;
    }
    t->paletteCount = palettes;
    if (palettes > 1) {
        GfxTextureAllocSlots(t, flag);
    }
    GfxTextureBuildDl(t, width, height);
    t->isPow2Size = (width == (u32)GfxTextureGetWidth(t) && height == (u32)GfxTextureGetHeight(t));
    t->tim2 = data;
    t->ownsTim2 = 0;
    t->invWidth = 1.0f / (float)(s32)width;
    t->invHeight = 1.0f / (float)(s32)height;
    if (g_textureList == NULL) {
        g_textureList = t;
    } else {
        CoreObjectAppend((CoreObject *)t, (CoreObject *)g_textureList);
    }
}
