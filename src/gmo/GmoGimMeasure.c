// bdc 0x08a26b74 GmoGimMeasure
#include "bdc.h"

/* Measure pass of `GmoTextureLoadGim`: checks the header (`GmoGimCheckHeader`), finds picture
   `index` (`GmoGimFindPicture`) and measures it (`GmoGimMeasurePicture`). Returns 1, or 0 on a
   bad file. */

s32 GmoGimMeasure(void *img, void *data, u32 size, s32 index, void *arena)
{
  void *picture;
  void *ctx[4];

  if (img != (void *)0x0 && arena != (void *)0x0 && GmoGimCheckHeader(data, size) != 0) {
    picture = GmoGimFindPicture(data, size, index);
    if (picture != (void *)0x0) {
      ctx[0] = arena;
      GmoGimMeasurePicture(ctx, picture);
      return 1;
    }
  }
  return 0;
}
