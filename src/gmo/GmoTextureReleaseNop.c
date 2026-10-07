// bdc 0x089daa98 GmoTextureReleaseNop
#include "bdc.h"

/* External-texture release hook installed by GmoInstallTextureHooks through
   GmoSetTextureNameHooks (paired with the lookup GmoTextureByFileName): empty, the game's
   textures are not released by name. */
void GmoTextureReleaseNop(const char *name)
{
    (void)name;
}
