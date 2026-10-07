// bdc 0x0890d30c UiLoadingHasBallTexture
#include "bdc.h"

/* Returns true when theme `+0x18` is 0 and the texture `lo_ball_00` is loaded (its name is not
   `NonTexture`, `GfxFindTexture`). */

bool UiLoadingHasBallTexture(UiLoading *self)

{
  bool result = self->theme == 0;
  GfxTexture *texture = (GfxTexture *)GfxFindTexture("lo_ball_00");

  if (strcasecmp(texture->name, "NonTexture") == 0) {
    result = false;
  }
  return result;
}
