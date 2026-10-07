// bdc 0x08a25ba0 GmoTextureBuildWritable
#include "bdc.h"

/* Build pass of `GmoTextureMakeWritable`: returns 0 for a NULL texture and 1 otherwise. Only
   when `flags & 2`: copies the shared pixel image / palette list heads into the plan
   (`GmoImageArrayCopy`, `GmoPaletteArrayCopy`) unless an id-1 copy already exists, tags the
   copies with id 1 and appends them (`GmoTextureAppendImage` / `GmoTextureAppendPalette`);
   then, if `image == palette` and flag 0x10 is clear, emits a fresh GE list
   (`GmoTextureWriteDl`, terminated by `0x0b000000` = RET) into pool 2, selects it
   (`GmoTextureSetImage`), drops the plan's pool-2 reference and writes the data cache back. */

s32 GmoTextureBuildWritable(void *texp, u32 flags, void *plan)
{
    GmoTexture *tex = (GmoTexture *)texp;
    GmoImage *imgCopy;
    GmoImage *palCopy;
    GmoImage *palettes;
    u32 *list;
    u32 sizeWord;
    u32 *cursor;

    if (tex == NULL) {
        return 0;
    }
    if ((flags & 2) == 0) {
        return 1;
    }
    imgCopy = GmoTextureFindImage(tex, 1, 0);
    palCopy = GmoTextureFindPalette(tex, 1, 0);
    palettes = tex->palettes; /* read before the image copy is appended */
    if (tex->images != NULL && imgCopy == NULL) {
        GmoImage *copy = (GmoImage *)GmoImageArrayCopy(tex->images, 1, 0x80000001, plan);
        copy->id = 1;
        GmoTextureAppendImage(tex, copy);
    }
    if (palettes != NULL && palCopy == NULL) {
        GmoImage *copy = (GmoImage *)GmoPaletteArrayCopy(palettes, 1, 0x80000001, plan);
        copy->id = 1;
        GmoTextureAppendPalette(tex, copy);
    }
    if (tex->image != tex->palette) {
        return 1;
    }
    if ((tex->flags & 0x10) != 0) {
        return 1;
    }
    /* First pass measures; the size word (+4 for the RET) is what the second pass gets as `end`. */
    sizeWord = (u32)GmoTextureWriteDl(tex, NULL, NULL, 0x80000001) + 4;
    list = (u32 *)GmoImagePlanTake(plan, 2, 4, (int)sizeWord);
    cursor = list;
    GmoTextureWriteDl(tex, &cursor, &sizeWord, 0x80000001);
    *cursor = 0x0b000000;
    cursor++;
    GmoTextureSetImage(tex, list);
    GmoImageHeapReleaseThunk(2, list);
    sceKernelDcacheWritebackAll();
    return 1;
}
