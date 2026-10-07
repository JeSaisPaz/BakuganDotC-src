// bdc 0x08805408 UiNameEntryPlayAvatarMotion
#include "bdc.h"

/* Plays motion `motion` on the avatar model of `UiNameEntry` (`+0x98`): stops the
   current one (`GfxModelEnableMotion`), loads `"editman_mot.gmo"` into the motion manager
   (`GmoMotionLoadFile`), looks up the motion name from the table `0x08ab9d1c` (8-byte entries,
   name at +4) with `GmoMotionIndexOfName`, stores the index at `+0xa0` and starts it with blend
   0.2 (`GfxModelPlayMotion`, `loop` flag). No-op without a motion manager. */

void UiNameEntryPlayAvatarMotion(UiNameEntry *self, s32 motion, u8 loop)

{
  void *mgr;
  s32 index;

  if (GmoMotionMgrExists()) {
    GfxModelEnableMotion(self->avatar);
    mgr = GmoMotionMgrGet();
    GmoMotionLoadFile(mgr,"editman_mot.gmo");
    mgr = GmoMotionMgrGet();
    index = GmoMotionIndexOfName(mgr,g_nameEntryMotionTable[motion * 2 + 1]);
    self->motion = index;
    GfxModelPlayMotion(0.2f,(GfxModel *)self->avatar,index,loop);
  }
  return;
}

