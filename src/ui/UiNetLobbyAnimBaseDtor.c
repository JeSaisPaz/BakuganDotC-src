// bdc 0x08940d34 UiNetLobbyAnimBaseDtor
#include "bdc.h"

/* Base destructor of `UiNetLobbySlideAnim`: restores vtable `0x08af4c04` and frees the object
   when `flags & 1`. */
void UiNetLobbyAnimBaseDtor(UiNetLobbySlideAnim *anim, u32 flags)
{
    if (anim != NULL) {
        anim->vtable = g_uiNetLobbyAnimBaseVtable;
        if ((flags & 1) != 0) {
            MemLock();
            MemFree(anim, NULL, 0);
            MemUnlock();
        }
    }
}
