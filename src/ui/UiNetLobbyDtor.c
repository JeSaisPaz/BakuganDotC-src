// bdc 0x089415d0 UiNetLobbyDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the ad-hoc multiplayer lobby screen (task id 2000): deletes the
   highlight rect, the text box, the two side-panel slide animations and their pointer array, and
   the sub-task (each through its virtual destructor with flag 3, clearing the field), then stores
   its id in `g_lastScreenTaskId`, clears `data` and calls `UiScreenDtor``(screen, 0)`; frees
   the object when `flags & 1`. */

typedef void (*UiNetLobbyDeleteFn)(void *obj, s32 flags);

void UiNetLobbyDtor(UiNetLobby *self, u32 flags)
{
  GfxRect *rect;
  UiNetLobbySlideAnim *anim;
  CoreTask *task;
  UiNetLobbySlideAnim **panels;
  const VtblEntry *vt;
  s32 i;

  if (self != (UiNetLobby *)0x0) {
    self->base.base.vtable = &g_uiNetLobbyVtable;
    rect = (GfxRect *)self->highlightRect;
    if (rect != (GfxRect *)0x0) {
      vt = rect->vtbl;
      ((UiNetLobbyDeleteFn)vt[1].fn)((u8 *)rect + vt[1].delta, 3);
      self->highlightRect = (void *)0x0;
    }
    if (self->textBox != (void *)0x0) {
      UiTextBoxDelete((UiTextBox *)self->textBox, 3);
      self->textBox = (void *)0x0;
    }
    for (i = 0; i < 2; i++) {
      anim = self->panels[i];
      if (anim != (UiNetLobbySlideAnim *)0x0) {
        vt = (const VtblEntry *)anim->vtable;
        ((UiNetLobbyDeleteFn)vt[1].fn)((u8 *)anim + vt[1].delta, 3);
        self->panels[i] = (UiNetLobbySlideAnim *)0x0;
      }
    }
    panels = self->panels;
    if (panels != (UiNetLobbySlideAnim **)0x0) {
      MemLock();
      MemFree(panels, (char *)0x0, 0);
      MemUnlock();
      self->panels = (UiNetLobbySlideAnim **)0x0;
    }
    /* subTask: object with a CoreTask-style vtable at +0xc */
    task = (CoreTask *)self->subTask;
    if (task != (CoreTask *)0x0) {
      vt = (const VtblEntry *)task->vtable;
      ((UiNetLobbyDeleteFn)vt[1].fn)((u8 *)task + vt[1].delta, 3);
      self->subTask = (void *)0x0;
    }
    g_lastScreenTaskId = self->base.base.id;
    self->base.data = (void *)0x0;
    UiScreenDtor(&self->base, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self, (char *)0x0, 0);
      MemUnlock();
    }
  }
}
