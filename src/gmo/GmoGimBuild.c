// bdc 0x08a267fc GmoGimBuild
#include "bdc.h"

/* Build pass of `GmoTextureLoadGim`: checks the header, finds the picture, re-initialises the
   image (`GmoTextureReset`), builds it (`GmoGimBuildPicture`) and flushes the data cache. Returns
   1, or 0 on a bad file. */

s32 GmoGimBuild(void *img, void *data, u32 size, s32 index, void *arena)
{
  void *picture;
  void *ctx[4];

  if (img != (void *)0x0 && arena != (void *)0x0 && GmoGimCheckHeader(data, size) != 0) {
    picture = GmoGimFindPicture(data, size, index);
    if (picture != (void *)0x0) {
      GmoTextureReset(img);
      ctx[0] = arena;
      GmoGimBuildPicture(ctx, picture, img);
      sceKernelDcacheWritebackAll();
      return 1;
    }
  }
  return 0;
}
