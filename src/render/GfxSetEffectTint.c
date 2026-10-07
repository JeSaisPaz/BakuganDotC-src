// bdc 0x08804000 GfxSetEffectTint
#include "bdc.h"

/* Tints the colour palettes (CLUTs) of six effect textures with `color` (four byte channels,
   `0xffffffff` = untouched). With `color == 0xffffffff` and no backup yet it only allocates the
   0x18c0-byte backup block g_gfxEffectTintBackup (1584 palette entries of 4 bytes) from the low
   heap; this is the call `GfxInitBootResources` makes. On the first real call it copies the
   original palettes of the textures listed in g_gfxEffectTintTable (`bfx_133p_02_03` 16 entries,
   `ffx_104p_01` 256, `bfx_233p_01` 256, `kemuri1` 768, `dash_smoke` 256, `smoke_g` 32) into the
   backup (flag g_gfxEffectTintSaved); every call then rewrites each palette entry as
   `backup * color / 255` per channel, so repeated tints never compound. */

void GfxSetEffectTint(u32 color)
{
    const GfxEffectTintEntry *entry;
    GfxTexture *tex;
    u32 *src;
    u8 *dst;
    u32 n;
    s32 i;
    u32 r, g, b, a;

    if (g_gfxEffectTintBackup == NULL && color == 0xffffffff) {
        bool fromLow;
        u32 *block;

        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        block = MemAlloc(1584 * sizeof(u32), NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        g_gfxEffectTintBackup = block;
        return;
    }

    if (g_gfxEffectTintSaved == 0) {
        g_gfxEffectTintSaved = 1;
        entry = g_gfxEffectTintTable;
        for (n = 0; n < 6; n++, entry++) {
            tex = GfxFindTexture(entry->textureName);
            if (tex == NULL)
                continue;
            src = GfxTextureGetClutData(tex);
            for (i = 0; i < entry->entryCount; i++, src++)
                g_gfxEffectTintBackup[entry->backupIndex + i] = *src;
        }
    }

    r = color & 0xff;
    g = (color >> 8) & 0xff;
    b = (color >> 16) & 0xff;
    a = (color >> 24) & 0xff;
    entry = g_gfxEffectTintTable;
    for (n = 0; n < 6; n++, entry++) {
        tex = GfxFindTexture(entry->textureName);
        if (tex == NULL)
            continue;
        dst = GfxTextureGetClutData(tex);
        for (i = 0; i < entry->entryCount; i++, dst += 4) {
            u32 c = g_gfxEffectTintBackup[entry->backupIndex + i];

            dst[0] = (u8)(((c & 0xff) * r) / 0xff);
            dst[1] = (u8)((((c >> 8) & 0xff) * g) / 0xff);
            dst[2] = (u8)((((c >> 16) & 0xff) * b) / 0xff);
            dst[3] = (u8)((((c >> 24) & 0xff) * a) / 0xff);
        }
    }
}
