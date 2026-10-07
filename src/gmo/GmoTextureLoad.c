// bdc 0x08a25f8c GmoTextureLoad
#include "bdc.h"

/* Loads an embedded image file into a texture object by trying each decoder in turn: GIM
   (`GmoTextureLoadGim`), TIM2 (`GmoTextureLoadTim2`), TGA (`GmoTextureLoadTga`) and BMP
   (`GmoTextureLoadBmp`). Returns 1 when one of them succeeded, else 0. */

s32 GmoTextureLoad(void *img, const void *data, u32 size, s32 index) {
    void *d = (void *)data;

    if (GmoTextureLoadGim(img, d, size, index) == 0 &&
        GmoTextureLoadTim2(img, d, size, index) == 0 &&
        GmoTextureLoadTga(img, d, size, index) == 0 &&
        GmoTextureLoadBmp(img, d, size, index) == 0) {
        return 0;
    }
    return 1;
}
