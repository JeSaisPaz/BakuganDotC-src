// bdc 0x0884f564 BtlMainPhaseBattle
#include "bdc.h"

/* Phase-1 update of the battle main task (task id 100, `BtlMainTaskCtor`): the running battle.
   Offline the battle runs every frame. In net play it runs only while the NetPlay manager is
   gone, window kind 12 is inactive, or the peer is synced; the first such frame instead starts the
   battle (random seed reset, `BtlNetSetBattleFlags`, frame counters zeroed, item spawning and
   crystal spawn request on, every Bakugan unit back to motion 0, dropped to the ground and
   unlinked). `netWaitFrames` counts the frames spent waiting and shows the net status message
   after 60. Task 0x1e1 is enabled while the battle runs and disabled while it waits. A running
   frame updates the scene (`BtlMainUpdateScene`), counts down the time limit (profile word 2,
   twice on a skipped frame), starts the HUD finish display in rule mode 2 and sets the BGM state
   from the map flash. Every frame the dim alpha fades out and the outcome is judged; a decided
   outcome sets `g_btlBattleOver`, starts the result demo and moves to phase 2, otherwise an
   accepted quit prompt opens the pause menu (phase 3) and hides the talk and cut-in tasks. */
void BtlMainPhaseBattle(BtlMain *self)
{
    bool running;
    bool playDemo;
    BtlBakugan **list;
    BtlBakugan *unit;
    const VtblEntry *vtbl;
    void *task;
    s32 word;
    float alpha;

    running = false;
    if (SaveGetProfileFlag0() == 0) {
        self->battleStarted = 1;
        running = true;
    } else {
        if (!NetPlayHasManager()) {
            running = true;
        } else if (UiGetWindowActive(0xc) == 0) {
            running = true;
        } else if (self->battleStarted == 0) {
            self->battleStarted = 1;
            CoreRandResetSeed();
            CoreRandLoadVfpuState();
            BtlNetSetBattleFlags(self, true);
            NetModeFlagSet();
            self->battleFrames = 0;
            g_btlFrameCount = 0;
            g_btlItemSpawnEnabled = 1;
            g_actorCrystalSpawnRequest = 1;
            list = BtlGetBakuganList();
            if (list != NULL) {
                for (unit = *list; unit != NULL; unit = (BtlBakugan *)unit->base.base.next) {
                    vtbl = (const VtblEntry *)unit->base.base.vtable;
                    if (((int (*)(void *))vtbl[10].fn)((u8 *)unit + vtbl[10].delta) == 0) {
                        continue;
                    }
                    GfxModelSwapMotionFrame(&unit->base, 0.0f);
                    BtlBakuganPlayMotion(0.0f, unit, 0, 1, 1);
                    BtlBakuganPlayMotion(0.0f, unit, 0, 1, 1);
                    BtlBakuganPlayMotion(0.0f, unit, 0, 1, 1);
                    CollisionRaycastPoint(unit->base.pos, unit->base.pos);
                    BtlBakuganResetGravity(unit);
                    BtlBakuganClearLink(unit);
                    unit->input->queuedActions = 0x40;
                }
            }
        } else if (NetPlayIsSynced(NetPlayGetManager())) {
            running = true;
        }
        if (running) {
            self->netWaitFrames = 0;
        } else {
            self->netWaitFrames = self->netWaitFrames + 1;
        }
        if (self->netWaitFrames < 61) {
            NetStatusSetMessage(0, 0);
        } else {
            NetStatusSetMessage(1, 0);
        }
    }

    task = CoreTaskFind(0x1e1);
    if (running) {
        if (task != NULL) {
            CoreTaskClearFlags(task, 1);
        }
    } else if (task != NULL) {
        CoreTaskSetFlags(task, 1);
    }

    if (running) {
        if (NetPlayHasManager()) {
            NetPlayGetManager();
            NetPlayNop();
        }
        BtlMainUpdateScene(self);
        if (g_btlControlLockApplied == 0) {
            if (SaveProfileGetWord(SaveGetProfile(), 2) != 0xffffffffu) {
                if (g_gfxDisplay->frameSkip != 0) {
                    SaveProfileSubWord(SaveGetProfile(), 2, 1);
                }
                SaveProfileSubWord(SaveGetProfile(), 2, 1);
            }
        }
        if (g_btlBattleOutcome == 0 && g_scriptGlobalVars[8] == 2) {
            word = (s32)SaveProfileGetWord(SaveGetProfile(), 2);
            if (word >= 0 && word < 0x709) {
                word = (s32)SaveProfileGetWord(SaveGetProfile(), 7);
                if (word < 1 || word >= 3) {
                    if (UiTalkTaskExists() != 0) {
                        BtlHudStartFinishMode2(UiGetTalkTask());
                    }
                } else if (UiTalkTaskExists() != 0) {
                    BtlHudStartFinishMode1(UiGetTalkTask());
                }
            }
        }
        if (g_btlMapFlashState != 0) {
            BtlMainSetBgmState(self, 1);
        } else {
            BtlMainSetBgmState(self, 0);
        }
        self->battleFrames = self->battleFrames + 1;
    }

    alpha = self->dimColor[3];
    if (alpha <= 0.005f) {
        alpha = 0.0f;
    } else {
        alpha = alpha * 0.9f;
    }
    self->dimColor[3] = alpha;
    if (alpha < 0.1f) {
        self->field12 = 1;
    }

    g_btlBattleOutcome = BtlMainJudgeOutcome(self, true);
    if (BtlMainIsMatchDrawn(self)) {
        g_btlBattleOutcome = 4;
    } else if (BtlMainIsMatchWon(self)) {
        g_btlBattleOutcome = 1;
    } else if (BtlMainIsMatchLost(self)) {
        g_btlBattleOutcome = 2;
    }

    if (g_btlBattleOutcome != 0) {
        g_btlBattleOver = 1;
        BtlEndCutIn();
        playDemo = g_btlBattleOutcome != 3;
        if (SaveGetProfileFlag0() != 0 && SaveHasProfile() &&
            SaveProfileHasFlags(SaveGetProfile(), 0x4880)) {
            playDemo = false;
        }
        if (playDemo) {
            self->resultDemo = BtlMainStartResultDemo(self, g_btlBattleOutcome);
            if (self->resultDemo == -1 && CoreTaskExists(0x14a) != 0) {
                if (self->stageEffects != NULL) {
                    GfxEffectMgrUpdate(self->stageEffects);
                }
                GfxEffectAdvanceTick();
            }
        }
        self->phaseStep = -1;
        self->phase = 2;
        self->drawPhase = 2;
    } else if (BtlMainUpdateQuitPrompt(self) != 0) {
        BtlMainOpenPauseMenu();
        self->phase = 3;
        self->drawPhase = 3;
        if (CoreTaskExists(0x6e) != 0) {
            CoreTaskSetFlags(CoreTaskFind(0x6e), 3);
        }
        if (CoreTaskExists(0x1e0) != 0) {
            CoreTaskSetFlags(CoreTaskFind(0x1e0), 3);
        }
        if (CoreTaskExists(0x1e1) != 0) {
            CoreTaskSetFlags(CoreTaskFind(0x1e1), 3);
        }
    }
}
