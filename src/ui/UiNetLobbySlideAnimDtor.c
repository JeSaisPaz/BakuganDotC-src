// bdc 0x08940e50 UiNetLobbySlideAnimDtor
#include "bdc.h"

/* Destructor of `UiNetLobbySlideAnim` (vtable `0x08af4c34`): runs the base dtor and frees the
   object when `flags & 1`. */
void UiNetLobbySlideAnimDtor(UiNetLobbySlideAnim *anim, u32 flags)
{
    if (anim == NULL) {
        return;
    }
    anim->vtable = g_uiNetLobbySlideAnimVtbl;
    UiNetLobbyAnimBaseDtor(anim, 0);
    if ((flags & 1) != 0) {
        MemLock();
        MemFree(anim, NULL, 0);
        MemUnlock();
    }
}
