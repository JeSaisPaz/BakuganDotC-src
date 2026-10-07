// bdc 0x08940d98 UiNetLobbyAnimBaseHide
#include "bdc.h"

/* Default Hide slot of `UiNetLobbySlideAnim`: returns 1 (finished) immediately. */
bool UiNetLobbyAnimBaseHide(UiNetLobbySlideAnim *anim, u8 reset)
{
    (void)anim;
    (void)reset;
    return 1;
}
