// bdc 0x089f899c GfxFabGetClip
#include "bdc.h"

/* Returns the `index`-th clip of the fab's clip list (`+0x88`), or NULL. */

GfxFabClip *GfxFabGetClip(GfxFab *fab, int index)

{
  GfxFabClip *clip;
  int i;

  clip = fab->clips;
  i = 0;
  while (clip != NULL) {
    if (i == index) {
      return clip;
    }
    clip = clip->next;
    i++;
  }
  return NULL;
}
