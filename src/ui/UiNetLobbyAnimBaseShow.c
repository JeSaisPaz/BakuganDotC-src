// bdc 0x08940d90 UiNetLobbyAnimBaseShow
#include "bdc.h"

/* Default Show slot of `UiNetLobbySlideAnim`: returns 1 (finished) immediately. */

bool UiNetLobbyAnimBaseShow(UiNetLobbySlideAnim *anim, u8 reset)
{
    (void)anim;
    (void)reset;
    return true;
}
