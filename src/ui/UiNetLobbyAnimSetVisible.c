// bdc 0x08940da0 UiNetLobbyAnimSetVisible
#include "bdc.h"

/* Shows or hides the sprite of a `UiNetLobbySlideAnim` (visibility flag 1 at `+0xd0`), if it has
   one. */

void UiNetLobbyAnimSetVisible(UiNetLobbySlideAnim *anim, u8 visible)

{
  GfxSprite *sprite;

  sprite = anim->sprite;
  if (sprite != NULL) {
    if (visible != 0) {
      sprite->flags = sprite->flags | 1;
      return;
    }
    sprite->flags = sprite->flags & ~1u;
  }
}
