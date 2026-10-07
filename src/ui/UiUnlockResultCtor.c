// bdc 0x08937d38 UiUnlockResultCtor
#include "bdc.h"

/* Constructor of the unlock result screen, task id 375 (0x177) (base `UiScreenCtor`, vtable
   `g_uiUnlockResultVtbl`). Built by `CoreTaskNewByIdArg` (object size 0x7fc, one-byte
   argument stored as `rewardKind`). Allocates the 0x94-byte sprite table (low heap), primes the
   fader (sort key 20000 when it had to be created), makes the stick emulate the d-pad (old value
   saved), saves `g_gfxActiveCamera` in `savedCamera`, reads profile word 0x14 as `rewardIndex`
   (minus 1 for reward kind 1), and disables the update of task 500 if it exists. Returns `self`.
   It presents a newly unlocked item: card (`"DWCardName"`), hologram (`"DWHologramName"`), metal
   figure (`"DWMetalFigureName"`, `"*_N_U_figure.gmo"` models), Maxus part (`"*_N_P_*.gmo"` part
   models, `"DWMaxusPartsHelp"`, `"part_comp_moji"`) or a special unlock (`"DWSpecialUnlock"`). */

UiUnlockResult *UiUnlockResultCtor(UiUnlockResult *self, u32 arg)
{
    bool wasFromLow;
    void *data;
    PadState *pad;
    CoreTask *task;
    s32 i;

    UiScreenCtor((CoreTask *)self);
    self->base.base.vtable = g_uiUnlockResultVtbl;
    self->rewardKind = (u8)arg;
    MemLock();
    wasFromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    data = MemAlloc(0x94, NULL, 0);
    MemSetAllocFromLow(wasFromLow);
    MemUnlock();
    self->base.data = data;
    UiScreenSetFrameMode((CoreTask *)self, 1);
    self->unk06c = 0;
    if (!GfxFaderIsReady()) {
        GfxFaderSlotsInit(NULL);
        GfxGetActiveFader()->sortKey = 20000.0f;
    }
    pad = self->base.pad;
    self->unk070 = 0;
    self->savedStickEmulatesDpad = pad->stickEmulatesDpad;
    pad->stickEmulatesDpad = 1;
    self->unk5ec = 0;
    for (i = 0; i < 2; i++) {
        self->printers[i] = NULL;
    }
    self->model = NULL;
    self->camera = NULL;
    self->modelTimer = 0.0f;
    self->savedCamera = g_gfxActiveCamera;
    self->modelFadeFrom = 0.0f;
    memset(self->motionName, 0, 0x40);
    self->modelBaseY = 0.0f;
    self->rewardIndex = SaveProfileGetWord(SaveGetProfile(), 0x14);
    if (self->rewardKind == 1) {
        self->rewardIndex = self->rewardIndex - 1;
    }
    self->flashScale = 0;
    self->flashRequested = 0;
    self->flashTimer = 0;
    task = CoreTaskFind(500);
    if (task != NULL) {
        CoreTaskSetFlags(task, 1);
    }
    return self;
}
