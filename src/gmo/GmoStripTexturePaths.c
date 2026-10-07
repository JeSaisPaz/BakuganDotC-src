// bdc 0x089daac4 GmoStripTexturePaths
#include "bdc.h"

/* Once per file (guarded by bit 0 of `GmoFile.flags`, which it sets): walks the first model
   chunk (`GmoGetFirstModel`), and for every texture chunk (type `0x0a`) shortens each path string
   sub-chunk (type `0x12`) in place to its file name by copying the text after the last `/` over the
   start (`strrchr` + `strcpy`), so textures are looked up by bare name in the pack chain. A chunk
   with bit 15 of `type` set has no children (its child list is empty) and an 8-byte header. */

void GmoStripTexturePaths(void *gmo)
{
    GmoFile *file = (GmoFile *)gmo;
    const GmoChunk *model;
    const GmoChunk *modelEnd;
    const GmoChunk *tex;
    const GmoChunk *texEnd;
    const GmoChunk *sub;
    char *path;
    char *slash;

    if ((file->flags & 1) != 0) {
        return;
    }
    file->flags |= 1;
    model = (const GmoChunk *)GmoGetFirstModel(file);
    modelEnd = (const GmoChunk *)((const u8 *)model + model->size);
    if ((model->type & 0x8000) != 0) {
        tex = modelEnd;
    } else {
        tex = (const GmoChunk *)((const u8 *)model + model->childOffset);
    }
    while (tex < modelEnd) {
        texEnd = (const GmoChunk *)((const u8 *)tex + tex->size);
        if ((tex->type & 0x7fff) == 0x0a) {
            if ((tex->type & 0x8000) != 0) {
                sub = texEnd;
            } else {
                sub = (const GmoChunk *)((const u8 *)tex + tex->childOffset);
            }
            while (sub < texEnd) {
                if ((sub->type & 0x8000) != 0) {
                    /* short (8-byte) header: data starts where childOffset would be */
                    path = (char *)&sub->childOffset;
                } else {
                    path = (char *)sub + sub->headerSize;
                }
                if ((sub->type & 0x7fff) == 0x12) {
                    slash = strrchr(path, '/');
                    if (slash != NULL) {
                        strcpy(path, slash + 1);
                    }
                }
                sub = (const GmoChunk *)((const u8 *)sub + sub->size);
            }
            texEnd = (const GmoChunk *)((const u8 *)tex + tex->size);
        }
        tex = texEnd;
    }
}
