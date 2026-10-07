// bdc 0x08940dd8 UiNetLobbySlideAnimCtor
#include "bdc.h"

/* Constructor of the slide animator `UiNetLobbySlideAnim`: base ctor (no sprite), vtable `0x08af4c34`, Init
   with `sprite`/`layoutPos` through the vtable, offset and mode 0. */
UiNetLobbySlideAnim *UiNetLobbySlideAnimCtor(UiNetLobbySlideAnim *anim, GfxSprite *sprite, s16 *layoutPos)
{
    typedef void (*InitFn)(void *, GfxSprite *, s16 *);

    UiNetLobbyAnimBaseCtor(anim, NULL, NULL);
    anim->vtable = g_uiNetLobbySlideAnimVtbl;
    ((InitFn)g_uiNetLobbySlideAnimVtbl[2].fn)((char *)anim + g_uiNetLobbySlideAnimVtbl[2].delta, sprite, layoutPos);
    anim->offset = 0;
    anim->mode = 0;
    return anim;
}
