// bdc 0x08a259b4 GmoTextureReleaseWritable
#include "bdc.h"

/* Undoes `GmoTextureMakeWritable` for a texture record: when `flags & 2`, looks up the private
   pixel image and palette (id 1, the copies `GmoTextureBuildWritable` appended:
   `GmoTextureFindImage` / `GmoTextureFindPalette`), unlinks them (`GmoTextureRemoveImage` /
   `GmoTextureRemovePalette`), drops their references (`GmoImageArrayRelease`) and re-selects
   the default GE command list `+8` (`GmoTextureSetImage`). */

void GmoTextureReleaseWritable(void *tex, u32 flags) {
    GmoTexture *t = (GmoTexture *)tex;
    GmoImage *img;
    GmoImage *pal;

    if (t != NULL && (flags & 2) != 0) {
        img = GmoTextureFindImage(t, 1, 0);
        pal = GmoTextureFindPalette(t, 1, 0);
        if (img != NULL) {
            GmoTextureRemoveImage(t, img);
            GmoImageArrayRelease(&img->refCount, 1);
        }
        if (pal != NULL) {
            GmoTextureRemovePalette(t, pal);
            GmoImageArrayReleaseThunk(&pal->refCount, 1);
        }
        GmoTextureSetImage(t, t->palette);
    }
}
