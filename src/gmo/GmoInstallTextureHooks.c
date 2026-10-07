// bdc 0x089e10a8 GmoInstallTextureHooks
#include "bdc.h"

/* Registers the GMO texture hooks: the name lookup `GmoTextureByFileName` plus `0x089daa98` with
   `GmoSetTextureNameHooks`, and `0x089daa90` with `GmoSetTextureIdHooks`. Called by
   `GmoSystemInit`. */

void GmoInstallTextureHooks(void)

{
  GmoSetTextureNameHooks(GmoTextureByFileName,GmoTextureReleaseNop);
  GmoSetTextureIdHooks(GmoTextureFindByIdNull,(void *)0x0);
  return;
}

