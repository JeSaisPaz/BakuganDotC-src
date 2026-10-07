// bdc 0x08941170 UiNetLobbySlideAnimSetOffset
#include "bdc.h"

/* Sets the slide offset (`+0x10`) of a `UiNetLobbySlideAnim`. */
void UiNetLobbySlideAnimSetOffset(UiNetLobbySlideAnim *anim, s32 offset)
{
    anim->offset = offset;
}
