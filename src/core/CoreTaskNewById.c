// bdc 0x0881289c CoreTaskNewById
#include "bdc.h"

/* Inlined in every case: allocates `size` bytes from the low end of the game heap (the placement
   policy is saved and restored around `MemAlloc`, all under `MemLock`). */
static inline void *CoreTaskAllocLow(s32 size)
{
    bool fromLow;
    void *mem;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(size, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    return mem;
}

/* Task factory behind `CoreTaskCreate`: a big `switch` over the task id that allocates the task
   object from the low end of the heap and runs the constructor of the matching class (the
   compiler built it from compare chains and three jump tables: ids 100-110, 300-510 and
   10009-10030). Ids with no class get a bare 0x10-byte `CoreTask` (`CoreTaskInit`); id 20000 is
   such a bare task with the vtable `g_coreTask20000Vtbl`. Returns the new, not yet inserted task,
   or NULL when the allocation fails (the constructor is then skipped). */
CoreTask *CoreTaskNewById(s32 id)
{
    void *mem;
    CoreTask *task;

    switch (id) {
    case 100:
        mem = CoreTaskAllocLow(sizeof(BtlMain));
        if (mem != NULL) {
            BtlMainTaskCtor(mem);
        }
        return mem;
    case 104:
        mem = CoreTaskAllocLow(sizeof(BtlTutorialTask));
        if (mem != NULL) {
            BtlTutorialTaskCtor(mem);
        }
        return mem;
    case 105:
        mem = CoreTaskAllocLow(sizeof(ActorCrystalSpawnTask));
        if (mem != NULL) {
            ActorCrystalSpawnTaskCtor(mem);
        }
        return mem;
    case 108:
        mem = CoreTaskAllocLow(sizeof(BtlArenaPhotoTask));
        if (mem != NULL) {
            BtlArenaPhotoTaskCtor(mem);
        }
        return mem;
    case 110:
        mem = CoreTaskAllocLow(sizeof(BtlHud));
        if (mem != NULL) {
            BtlHudCtor(mem);
        }
        return mem;
    case 198:
        mem = CoreTaskAllocLow(sizeof(UiCopyrightTask));
        if (mem != NULL) {
            UiCopyrightTaskCtor(mem);
        }
        return mem;
    case 199:
        mem = CoreTaskAllocLow(sizeof(UiLanguageSelect));
        if (mem != NULL) {
            UiLanguageSelectCtor(mem);
        }
        return mem;
    case 200:
        mem = CoreTaskAllocLow(sizeof(UiTitle));
        if (mem != NULL) {
            UiTitleCtor(mem);
        }
        return mem;
    case 300:
        mem = CoreTaskAllocLow(sizeof(UiMainMenu));
        if (mem != NULL) {
            UiMainMenuCtor(mem);
        }
        return mem;
    case 301:
        mem = CoreTaskAllocLow(sizeof(UiPauseSettings));
        if (mem != NULL) {
            UiPauseSettingsCtor(mem);
        }
        return mem;
    case 302:
        mem = CoreTaskAllocLow(sizeof(UiEquip));
        if (mem != NULL) {
            UiEquipCtor(mem);
        }
        return mem;
    case 303:
        mem = CoreTaskAllocLow(sizeof(UiCardEquip));
        if (mem != NULL) {
            UiCardEquipCtor(mem);
        }
        return mem;
    case 304:
        mem = CoreTaskAllocLow(sizeof(UiOption));
        if (mem != NULL) {
            UiOptionCtor(mem);
        }
        return mem;
    case 310:
        mem = CoreTaskAllocLow(sizeof(UiWorldMap));
        if (mem != NULL) {
            UiWorldMapCtor(mem);
        }
        return mem;
    case 311:
        mem = CoreTaskAllocLow(sizeof(UiCollectionMenu));
        if (mem != NULL) {
            UiCollectionMenuCtor(mem);
        }
        return mem;
    case 315:
        mem = CoreTaskAllocLow(sizeof(UiCollectionTheater));
        if (mem != NULL) {
            UiCollectionTheaterCtor(mem);
        }
        return mem;
    case 316:
        mem = CoreTaskAllocLow(sizeof(UiUnlockCode));
        if (mem != NULL) {
            UiUnlockCodeCtor(mem);
        }
        return mem;
    case 320:
        mem = CoreTaskAllocLow(sizeof(UiScreen));
        if (mem != NULL) {
            UiSharedBgCtor(mem);
        }
        return mem;
    case 340:
        mem = CoreTaskAllocLow(sizeof(UiBattleRuleSelect));
        if (mem != NULL) {
            UiBattleRuleSelectCtor(mem);
        }
        return mem;
    case 350:
        mem = CoreTaskAllocLow(sizeof(UiBattleModeSelect));
        if (mem != NULL) {
            UiBattleModeSelectCtor(mem);
        }
        return mem;
    case 360:
        mem = CoreTaskAllocLow(sizeof(UiScreen));
        if (mem != NULL) {
            UiEmptyScreen360Ctor(mem);
        }
        return mem;
    case 365:
        mem = CoreTaskAllocLow(sizeof(UiScreen));
        if (mem != NULL) {
            UiEmptyScreen365Ctor(mem);
        }
        return mem;
    case 370:
        mem = CoreTaskAllocLow(sizeof(UiScreen));
        if (mem != NULL) {
            UiEmptyScreen370Ctor(mem);
        }
        return mem;
    case 371:
        mem = CoreTaskAllocLow(sizeof(UiBakuganSelect));
        if (mem != NULL) {
            UiBakuganSelectCtor(mem);
        }
        return mem;
    case 372:
        mem = CoreTaskAllocLow(sizeof(UiScreen));
        if (mem != NULL) {
            UiEmptyScreen372Ctor(mem);
        }
        return mem;
    case 373:
        mem = CoreTaskAllocLow(sizeof(UiGauntletSetup));
        if (mem != NULL) {
            UiGauntletSetupCtor(mem);
        }
        return mem;
    case 376:
        mem = CoreTaskAllocLow(sizeof(UiAdvSelect));
        if (mem != NULL) {
            UiAdvSelectCtor(mem);
        }
        return mem;
    case 380:
        mem = CoreTaskAllocLow(sizeof(UiScreen));
        if (mem != NULL) {
            UiEmptyScreen380Ctor(mem);
        }
        return mem;
    case 390:
        mem = CoreTaskAllocLow(sizeof(UiScreen390));
        if (mem != NULL) {
            UiScreen390Ctor(mem);
        }
        return mem;
    case 391:
        mem = CoreTaskAllocLow(sizeof(UiHologramGallery));
        if (mem != NULL) {
            UiHologramGalleryCtor(mem);
        }
        return mem;
    case 400:
        mem = CoreTaskAllocLow(sizeof(UiScreen));
        if (mem != NULL) {
            UiEmptyScreen400Ctor(mem);
        }
        return mem;
    case 410:
        mem = CoreTaskAllocLow(sizeof(UiPause));
        if (mem != NULL) {
            UiPauseCtor(mem);
        }
        return mem;
    case 430:
        mem = CoreTaskAllocLow(sizeof(UiRepair));
        if (mem != NULL) {
            UiRepairCtor(mem);
        }
        return mem;
    case 440:
        mem = CoreTaskAllocLow(sizeof(UiScreen));
        if (mem != NULL) {
            UiEmptyScreen440Ctor(mem);
        }
        return mem;
    case 450:
        mem = CoreTaskAllocLow(sizeof(UiScreen));
        if (mem != NULL) {
            UiEmptyScreen450Ctor(mem);
        }
        return mem;
    case 460:
        mem = CoreTaskAllocLow(sizeof(UiScreen));
        if (mem != NULL) {
            UiEmptyScreen460Ctor(mem);
        }
        return mem;
    case 470:
        mem = CoreTaskAllocLow(sizeof(GameEvent470));
        if (mem != NULL) {
            GameEvent470Ctor(mem);
        }
        return mem;
    case 482:
        mem = CoreTaskAllocLow(sizeof(GfxLensFlareTask));
        if (mem != NULL) {
            GfxLensFlareTaskCtor(mem);
        }
        return mem;
    case 483:
        mem = CoreTaskAllocLow(sizeof(BtlBakuganTexLoaderTask));
        if (mem != NULL) {
            BtlBakuganTexLoaderTaskCtor(mem);
        }
        return mem;
    case 490:
        mem = CoreTaskAllocLow(sizeof(UiUpgrade));
        if (mem != NULL) {
            UiUpgradeCtor(mem);
        }
        return mem;
    case 500:
        mem = CoreTaskAllocLow(sizeof(GameFieldTask));
        if (mem != NULL) {
            GameFieldCtor(mem);
        }
        return mem;
    case 501:
        mem = CoreTaskAllocLow(sizeof(GameDebugStageSelect));
        if (mem != NULL) {
            GameDebugStageSelectCtor(mem);
        }
        return mem;
    case 510:
        mem = CoreTaskAllocLow(sizeof(UiConfirmDialog));
        if (mem != NULL) {
            UiConfirmDialogCtor(mem);
        }
        return mem;
    case 1000:
        mem = CoreTaskAllocLow(sizeof(UiTitleMenu));
        if (mem != NULL) {
            UiTitleMenuCtor(mem);
        }
        return mem;
    case 1999:
        mem = CoreTaskAllocLow(sizeof(UiNetMenu));
        if (mem != NULL) {
            UiNetMenuCtor(mem);
        }
        return mem;
    case 2000:
        mem = CoreTaskAllocLow(sizeof(UiNetLobby));
        if (mem != NULL) {
            UiNetLobbyCtor(mem);
        }
        return mem;
    case 2001:
        mem = CoreTaskAllocLow(sizeof(NetBattleSyncTask));
        if (mem != NULL) {
            NetBattleSyncTaskCtor(mem);
        }
        return mem;
    case 2002:
        mem = CoreTaskAllocLow(sizeof(NetStatusTask));
        if (mem != NULL) {
            NetStatusTaskCtor(mem);
        }
        return mem;
    case 2003:
        mem = CoreTaskAllocLow(sizeof(UiLoadIconTask));
        if (mem != NULL) {
            UiLoadIconTaskCtor(mem);
        }
        return mem;
    case 3000:
        mem = CoreTaskAllocLow(sizeof(UiNameEntry));
        if (mem != NULL) {
            UiNameEntryCtor(mem);
        }
        return mem;
    case 3001:
        mem = CoreTaskAllocLow(sizeof(UiFieldHud));
        if (mem != NULL) {
            UiFieldHudCtor(mem);
        }
        return mem;
    case 3002:
        mem = CoreTaskAllocLow(sizeof(UiComboList));
        if (mem != NULL) {
            UiComboListCtor(mem);
        }
        return mem;
    case 3004:
        mem = CoreTaskAllocLow(sizeof(UiStaffCredit));
        if (mem != NULL) {
            UiStaffCreditCtor(mem);
        }
        return mem;
    case 3005:
        mem = CoreTaskAllocLow(sizeof(UiBattleRecord));
        if (mem != NULL) {
            UiBattleRecordCtor(mem);
        }
        return mem;
    case 10009:
        mem = CoreTaskAllocLow(sizeof(SaveAutoLoadTask));
        if (mem != NULL) {
            SaveAutoLoadTaskCtor(mem);
        }
        return mem;
    case 10010:
        mem = CoreTaskAllocLow(sizeof(SaveLoadTask));
        if (mem != NULL) {
            SaveLoadTaskCtor(mem);
        }
        return mem;
    case 10011:
        mem = CoreTaskAllocLow(sizeof(SaveNopTask));
        if (mem != NULL) {
            SaveNopTaskCtor(mem);
        }
        return mem;
    case 10020:
        mem = CoreTaskAllocLow(sizeof(SaveSaveTask));
        if (mem != NULL) {
            SaveSaveTaskCtor(mem);
        }
        return mem;
    case 10021:
        mem = CoreTaskAllocLow(sizeof(SaveLanguageTask));
        if (mem != NULL) {
            SaveLanguageTaskCtor(mem);
        }
        return mem;
    case 10022:
        mem = CoreTaskAllocLow(sizeof(SaveAutoSaveTask));
        if (mem != NULL) {
            SaveAutoSaveTaskCtor(mem);
        }
        return mem;
    case 10030:
        mem = CoreTaskAllocLow(sizeof(SaveDeleteTask));
        if (mem != NULL) {
            SaveDeleteTaskCtor(mem);
        }
        return mem;
    case 10040:
        mem = CoreTaskAllocLow(sizeof(GfxMovieTask));
        if (mem != NULL) {
            GfxMovieTaskCtor(mem);
        }
        return mem;
    case 10050:
        mem = CoreTaskAllocLow(sizeof(UiTextTask));
        if (mem != NULL) {
            UiTextTaskCtor(mem);
        }
        return mem;
    case 10060:
        mem = CoreTaskAllocLow(sizeof(CoreTask));
        if (mem != NULL) {
            GfxFaderTaskCtor(mem);
        }
        return mem;
    case 10070:
        mem = CoreTaskAllocLow(sizeof(SndBgmCmd));
        if (mem != NULL) {
            SndBgmCmdInit(mem);
        }
        return mem;
    case 10090:
        mem = CoreTaskAllocLow(sizeof(UiSpriteMng));
        if (mem != NULL) {
            UiSpriteMngTaskCtor(mem);
        }
        return mem;
    case 10100:
        mem = CoreTaskAllocLow(sizeof(UiLoading));
        if (mem != NULL) {
            UiLoadingCtor(mem);
        }
        return mem;
    case 10110:
        mem = CoreTaskAllocLow(sizeof(CoreTask));
        if (mem != NULL) {
            IoPacLoaderTaskCtor(mem);
        }
        return mem;
    case 20000:
        task = CoreTaskAllocLow(sizeof(CoreTask));
        if (task != NULL) {
            CoreTaskInit(task);
            task->vtable = g_coreTask20000Vtbl;
        }
        return task;
    default:
        /* every id without a class (holes of the ranges such as 101-103, 3003 and 10012-10019,
           and all other ids): a bare base task */
        task = CoreTaskAllocLow(sizeof(CoreTask));
        if (task != NULL) {
            CoreTaskInit(task);
        }
        return task;
    }
}
