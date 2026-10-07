// bdc 0x08988d9c UiCollectionTheaterDimPageArrows
#include "bdc.h"

/* Sets the alpha of the left/right page arrows of `UiCollectionTheater`
   to `alpha` and greys out the left one on page 0 and the right one on the last page (3). */

void UiCollectionTheaterDimPageArrows(float alpha, UiScreen *screen)
{
  /* screen->data is an array of sprite pointers: left arrow at [27] (+0x6c), right at [30] (+0x78) */
  GfxSprite **sprites = (GfxSprite **)screen->data;
  UiCollectionTheater *theater = (UiCollectionTheater *)screen;
  GfxSprite *sprite;

  sprite = sprites[27];
  sprite->tint[0] = 1.0f;
  sprite->tint[1] = 1.0f;
  sprite->tint[2] = 1.0f;
  sprite->alpha = alpha;
  sprite = sprites[30];
  sprite->tint[0] = 1.0f;
  sprite->tint[1] = 1.0f;
  sprite->tint[2] = 1.0f;
  sprite->alpha = alpha;
  if (theater->page == 0) {
    sprite = sprites[27];
    sprite->tint[0] = 0.5f;
    sprite->tint[1] = 0.5f;
    sprite->tint[2] = 0.5f;
    sprite->alpha = alpha;
  }
  if (theater->page == 3) {
    sprite = sprites[30];
    sprite->tint[0] = 0.5f;
    sprite->tint[1] = 0.5f;
    sprite->tint[2] = 0.5f;
    sprite->alpha = alpha;
  }
}
