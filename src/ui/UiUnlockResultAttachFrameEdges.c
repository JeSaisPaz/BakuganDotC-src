// bdc 0x089399c0 UiUnlockResultAttachFrameEdges
#include "bdc.h"

/* Keeps the top and bottom edge sprites (2 and 4) of the reward frame of
   `UiUnlockResult` attached to the stretching middle sprite 3: Y = middle Y
   ∓ its Y scale × `+0x5f4` / `+0x5fc`. */

void UiUnlockResultAttachFrameEdges(UiUnlockResult *self)

{
  GfxSprite **sprites;
  GfxSprite *mid;

  sprites = (GfxSprite **)(self->base).data;
  mid = sprites[3];
  sprites[2]->posY = mid->posY - mid->scaleY * self->frameEdges[1];
  sprites = (GfxSprite **)(self->base).data;
  mid = sprites[3];
  sprites[4]->posY = mid->posY + mid->scaleY * self->frameEdges[3];
  return;
}
