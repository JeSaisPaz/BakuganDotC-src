// bdc 0x089daa90 GmoTextureFindByIdNull
#include "bdc.h"

/* Texture-by-id hook of the GMO library installed by `GmoInstallTextureHooks` through
   `GmoSetTextureIdHooks`: ignores `id` and returns NULL, so textures are only resolved by name
   (`GmoTextureByFileName`). */
void *GmoTextureFindByIdNull(u16 id)
{
    (void)id;
    return NULL;
}
