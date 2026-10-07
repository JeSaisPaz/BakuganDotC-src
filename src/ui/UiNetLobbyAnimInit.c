// bdc 0x08940cf8 UiNetLobbyAnimInit
#include "bdc.h"

/* Initialises a `UiNetLobbySlideAnim`: sprite, layout position and frame counter 0 (vtable slot
   +0x10). */

void UiNetLobbyAnimInit(UiNetLobbySlideAnim *anim, GfxSprite *sprite, s16 *layoutPos)

{
  anim->sprite = sprite;
  anim->layoutPos = layoutPos;
  anim->frame = 0;
  return;
}

