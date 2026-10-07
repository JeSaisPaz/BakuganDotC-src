// bdc 0x0880490c UiNameEntryDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the player name entry screen (task id 3000): reinstalls vtable
   `g_uiNameEntryVtbl`, waits for the GE (`GfxWaitGeIdle`), frees all GMO motions
   (`GmoMotionFreeAll`), and deletes through their virtual destructors (slot 1, flags 3) the avatar
   model, the name and key text printers, the common and language pack nodes, the camera and the base
   model (`+0x98`, `+0x94`, `+0x90`, `+0x70`, `+0x6c`, `+0x9c`, `+0xa4`), clearing each pointer.
   Then `UiScreenDtor``(screen, 0)`; frees the object when `flags & 1`. */

void UiNameEntryDtor(UiNameEntry *self, u32 flags)

{
  const VtblEntry *vt;

  if (self != (UiNameEntry *)0x0) {
    self->base.base.vtable = g_uiNameEntryVtbl;
    GfxWaitGeIdle();
    GmoMotionFreeAll(GmoMotionMgrGet(), 0);
    if (self->avatar != (void *)0x0) {
      vt = (const VtblEntry *)((GfxModel *)self->avatar)->base.vtable + 1;
      ((void (*)(void *, int))vt->fn)((char *)self->avatar + vt->delta, 3);
      self->avatar = (void *)0x0;
    }
    if (self->nameText != (void *)0x0) {
      vt = ((UiTextPrinter *)self->nameText)->layer.vtbl + 1;
      ((void (*)(void *, int))vt->fn)((char *)self->nameText + vt->delta, 3);
      self->nameText = (void *)0x0;
    }
    if (self->keyText != (void *)0x0) {
      vt = ((UiTextPrinter *)self->keyText)->layer.vtbl + 1;
      ((void (*)(void *, int))vt->fn)((char *)self->keyText + vt->delta, 3);
      self->keyText = (void *)0x0;
    }
    if (self->commonPack != (CoreNode *)0x0) {
      vt = (const VtblEntry *)self->commonPack->vtable + 1;
      ((void (*)(void *, int))vt->fn)((char *)self->commonPack + vt->delta, 3);
      self->commonPack = (CoreNode *)0x0;
    }
    if (self->langPack != (CoreNode *)0x0) {
      vt = (const VtblEntry *)self->langPack->vtable + 1;
      ((void (*)(void *, int))vt->fn)((char *)self->langPack + vt->delta, 3);
      self->langPack = (CoreNode *)0x0;
    }
    if (self->camera != (void *)0x0) {
      vt = (const VtblEntry *)((CoreNode *)self->camera)->vtable + 1;
      ((void (*)(void *, int))vt->fn)((char *)self->camera + vt->delta, 3);
      self->camera = (void *)0x0;
    }
    if (self->baseModel != (void *)0x0) {
      vt = (const VtblEntry *)((GfxModel *)self->baseModel)->base.vtable + 1;
      ((void (*)(void *, int))vt->fn)((char *)self->baseModel + vt->delta, 3);
      self->baseModel = (void *)0x0;
    }
    UiScreenDtor(&self->base, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self, (char *)0x0, 0);
      MemUnlock();
    }
  }
}
