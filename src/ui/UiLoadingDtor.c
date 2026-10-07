// bdc 0x0890ad34 UiLoadingDtor
#include "bdc.h"

/* Destructor of the now-loading screen (task 10100 / 0x2774, 0x240 bytes, `UiLoadingCtor`):
   reinstalls `g_uiLoadingVtbl`; when the shared objects `g_uiLoadingShared` exist, queues the
   animation player `fab` for `GfxDeferredDelete`, releases the tip-picture request `tipData`
   (`IoDataMngRelease`, after `GfxWaitGeIdle`), deletes the tip texture `tipTexture` (virtual
   deleting destructor, flag 3), clears the message box `box` (`UiTextBoxClear`) and, when
   `iconHit` is set, puts the icon sprites 11..16 back at their `g_loadingIconLayout` positions,
   zeroes their `+0x80` vector and `scaleX..angle` (`sv.q` of the bank zero vector C720) and hides
   them; then `CoreTaskDestroy`, and frees the task when bit 0 of `flags` is set. */

void UiLoadingDtor(UiLoading *self, u32 flags)
{
  CoreObject *tex;
  const VtblEntry *dtor;
  GfxSprite *sprite;
  void *mng;
  int i;

  if (self == NULL) {
    return;
  }
  self->base.vtable = &g_uiLoadingVtbl;
  if (g_uiLoadingShared != NULL) {
    if (g_uiLoadingShared->fab != NULL) {
      GfxDeferredDelete((CoreObject *)g_uiLoadingShared->fab);
      g_uiLoadingShared->fab = NULL;
    }
    if (g_uiLoadingShared->tipData != NULL) {
      GfxWaitGeIdle();
      mng = IoGetDataMng();
      IoDataMngRelease((IoDataMng *)mng, &g_uiLoadingShared->tipData, g_uiLoadingShared->tipData);
      g_uiLoadingShared->tipData = NULL;
    }
    tex = g_uiLoadingShared->tipTexture;
    if (tex != NULL) {
      dtor = &((const VtblEntry *)tex->vtable)[1];
      ((void (*)(void *, s32))dtor->fn)((u8 *)tex + dtor->delta, 3);
      g_uiLoadingShared->tipTexture = NULL;
    }
    if (g_uiLoadingShared->box != NULL) {
      UiTextBoxClear(g_uiLoadingShared->box);
    }
    if (self->iconHit) {
      for (i = 11; i < 17; i++) {
        sprite = g_uiLoadingShared->sprites[i];
        sprite->posX = (float)g_loadingIconLayout[i - 11][0];
        sprite->posY = (float)g_loadingIconLayout[i - 11][1];
        sprite->maybe_billboardParams80[0] = 0.0f;
        sprite->maybe_billboardParams80[1] = 0.0f;
        sprite->maybe_billboardParams80[2] = 0.0f;
        sprite->maybe_billboardParams80[3] = 0.0f;
        sprite->scaleX = 0.0f;
        sprite->scaleY = 0.0f;
        sprite->scaleZ = 0.0f;
        sprite->angle = 0.0f;
        sprite->flags &= 0xfffffffe;
      }
    }
  }
  CoreTaskDestroy(&self->base, 0);
  if ((flags & 1) != 0) {
    MemLock();
    MemFree(self, NULL, 0);
    MemUnlock();
  }
}
