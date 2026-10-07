// bdc 0x089a3d44 UiMainMenuDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the UiMainMenu screen (task id 300): reinstalls
   `g_uiMainMenuVtbl`, waits for the GE, restores the pad's stick-as-d-pad byte, stores its task
   id in `g_lastScreenTaskId` (last closed screen) and profile word 0x1d, runs
   `UiMainMenuReleaseModels`, frees all GMO motions, moves the menu textures back to main RAM
   (`GfxTextureMoveToMainRam`), runs `UiHelpLineDestroy` and `GfxCameraDtor`; then
   `UiScreenDtor``(this, 0)` and frees the object when `flags & 1`. */

void UiMainMenuDtor(UiMainMenu *self, u32 flags)
{
    SaveProfile *profile;

    if (self == NULL) {
        return;
    }
    self->base.base.vtable = g_uiMainMenuVtbl;
    GfxWaitGeIdle();
    self->base.pad->stickEmulatesDpad = 0;
    g_lastScreenTaskId = self->base.base.id;
    profile = SaveGetProfile();
    SaveProfileSetWord(profile, 0x1d, self->base.base.id);
    UiMainMenuReleaseModels(self);
    GmoMotionFreeAll(GmoMotionMgrGet(), 0);
    GfxTextureMoveToMainRam("main_bg00");
    GfxTextureMoveToMainRam("line");
    GfxTextureMoveToMainRam("f0_z_sea01");
    GfxTextureMoveToMainRam("worldmap");
    UiHelpLineDestroy();
    GfxCameraDtor((CoreNode *)self->camera, 2);
    UiScreenDtor(&self->base, 0);
    if ((flags & 1) != 0) {
        MemLock();
        MemFree(self, NULL, 0);
        MemUnlock();
    }
}
