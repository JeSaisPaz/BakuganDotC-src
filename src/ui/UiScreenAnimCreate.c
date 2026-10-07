// bdc 0x08909e4c UiScreenAnimCreate
#include "bdc.h"

/* Creates a timeline animation player (0xb0-byte object, constructor `GfxFabCtorFromPack`; tracks
   at `+0x88`, frame `track+0x34`) for animation data `data` in slot `slot` of the screen's
   animation array (`screen+0x50`, list `screen+0x54`), and records the first track's current frame
   (`+0x60`) and length (`+0x64`), rewinding it to frame 0 (`GfxFabSeek`). */

void UiScreenAnimCreate(void *screen, void *data, int slot)

{
  UiScreen *s = (UiScreen *)screen;
  bool fromLow;
  GfxFab *fab;
  GfxFab *created;
  GfxFabClip *clip;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  fab = MemAlloc(sizeof(GfxFab), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  created = NULL;
  if (fab != NULL) {
    GfxFabCtorFromPack(fab, data, &s->bgAnimList);
    created = fab;
  }
  ((GfxFab **)s->bgData)[slot] = created;
  clip = GfxFabGetClip(((GfxFab **)s->bgData)[slot], 0);
  if (clip != NULL) {
    s->unk60 = clip->frame;
    if (clip->def != NULL) {
      s->unk64 = clip->def->lastFrame;
      GfxFabSeek(((GfxFab **)s->bgData)[slot], 0);
    }
  }
}
