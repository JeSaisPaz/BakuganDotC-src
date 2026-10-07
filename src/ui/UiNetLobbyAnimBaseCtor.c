// bdc 0x08940d08 UiNetLobbyAnimBaseCtor
#include "bdc.h"

/* Base constructor of `UiNetLobbySlideAnim`: installs vtable `0x08af4c04` and runs
   `UiNetLobbyAnimInit` with the forwarded `sprite`/`layoutPos`. */
void UiNetLobbyAnimBaseCtor(UiNetLobbySlideAnim *anim, GfxSprite *sprite, s16 *layoutPos)
{
    anim->vtable = &g_uiNetLobbyAnimBaseVtable;
    UiNetLobbyAnimInit(anim, sprite, layoutPos);
}
