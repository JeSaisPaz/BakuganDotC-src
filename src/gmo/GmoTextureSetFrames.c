// bdc 0x08a10ab4 GmoTextureSetFrames
#include "bdc.h"

/* Stores the three frame selector bytes (`frameA..frameC`, `+0x1d..+0x1f`) of a `GmoTexture`
   and clears its current image and palette. Does nothing for a NULL texture. */
void GmoTextureSetFrames(void *tex_, u8 a, u8 b, u8 c)
{
    GmoTexture *tex = tex_;

    if (tex != NULL) {
        tex->frameA = a;
        tex->frameB = b;
        tex->frameC = c;
        GmoTextureSetImage(tex, NULL);
        GmoTextureSetPalette(tex, NULL);
    }
}
