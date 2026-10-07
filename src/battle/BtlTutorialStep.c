// bdc 0x08846a30 BtlTutorialStep
#include "bdc.h"

/* Per-frame tutorial script of task 104 (`BtlTutorialTaskUpdate`), a switch on the tutorial step
   (script global var 11, `g_scriptGlobalVars`). Does nothing unless the player's Bakugan exists
   and its vtable predicate (entry 18) holds. Steps below 0xf call `BtlCombatArtSlotsNoOp`;
   odd steps wait for the lesson's goal via `BtlTutorialAdvance`: 3 stats counter 0x10 > 0,
   5/7 `knockoutCount` >= 3/6, 9 the dummy's retreat finished and the player's z below 900
   (locks control; z below 3300 sets `stage36HintFlag`), 0xb stop wall 1 missing or its `shown` byte clear. Step 0 makes the
   kind-2 dummy's collider 0 permanently hit; after step 3 the dummy turns untargetable and the
   caption closes; after step 0xb its colliders are re-armed and control unlocked. Step 0xd counts
   `stepTimer`: at 0 the dummy's allowed actions are cleared, at 60 set to `~0xda8`. Step 0x17
   moves to step 0x18 and locks control. */

void BtlTutorialStep(void *task)
{
    BtlTutorialTask *self = (BtlTutorialTask *)task;
    BtlBakugan *unit;
    BtlBakugan *player;
    BtlUnitMode4 *dummy;
    StopWall *wall;
    CollisionCollider *col;
    const VtblEntry *pred;
    s32 step;
    s32 count;
    s32 timer;

    unit = (BtlBakugan *)BtlGetPlayerBakugan();
    step = g_scriptGlobalVars[11];
    if (unit == NULL) {
        return;
    }
    player = NULL;
    pred = &((const VtblEntry *)unit->base.base.vtable)[18];
    if (((s32 (*)(void *))pred->fn)((u8 *)unit + pred->delta) != 0) {
        player = unit;
    }
    if (player == NULL) {
        return;
    }

    switch (step) {
    case 0:
        BtlCombatArtSlotsNoOp();
        dummy = (BtlUnitMode4 *)BtlTutorialFindUnitByKind(task, 2);
        if (dummy != NULL && (col = dummy->base.collider0) != NULL) {
            col->flags |= 1;
            col->hitTimer = -1;
        }
        break;
    case 3:
        count = 0;
        if (player->stats != NULL) {
            count = BtlStatsGetCounter(player->stats, 0x10);
        }
        BtlCombatArtSlotsNoOp();
        if (BtlTutorialAdvance(task, count > 0) != 0) {
            dummy = (BtlUnitMode4 *)BtlTutorialFindUnitByKind(task, 2);
            if (dummy != NULL) {
                dummy->base.untargetable = 1;
            }
            if (UiTalkTaskExists() != 0) {
                UiCaptionSetText(UiGetTalkTask(), 0, 0, NULL, -1);
            }
        }
        break;
    case 5:
        count = player->knockoutCount;
        BtlCombatArtSlotsNoOp();
        BtlTutorialAdvance(task, count >= 3);
        break;
    case 7:
        count = player->knockoutCount;
        BtlCombatArtSlotsNoOp();
        BtlTutorialAdvance(task, count >= 6);
        break;
    case 9:
        count = 0;
        BtlCombatArtSlotsNoOp();
        dummy = (BtlUnitMode4 *)BtlTutorialFindUnitByKind(task, 2);
        if (dummy == NULL) {
            count = 1;
        } else if (dummy->arrived != 0) {
            if (player->base.pos[2] < 900.0f) {
                BtlTutorialSetControlLock(task, true, NULL);
                count = 1;
            } else if (player->base.pos[2] < 3300.0f && player->stage36HintFlag == 0) {
                player->stage36HintFlag = 1;
            }
        }
        BtlTutorialAdvance(task, count > 0);
        break;
    case 11:
        count = 0;
        wall = (StopWall *)BtlTutorialFindStageObject(task, 1);
        if (wall == NULL) {
            count = 1;
        } else if (wall->shown == 0) {
            count = 1;
        }
        BtlCombatArtSlotsNoOp();
        if (BtlTutorialAdvance(task, count > 0) != 0) {
            dummy = (BtlUnitMode4 *)BtlTutorialFindUnitByKind(task, 2);
            if (dummy != NULL) {
                col = dummy->base.collider0;
                if (col != NULL) {
                    col->flags &= ~1u;
                    col->hitTimer = 0;
                }
                col = dummy->base.collider1;
                if (col != NULL) {
                    col->flags &= ~0x40u;
                }
            }
            BtlTutorialSetControlLock(task, false, NULL);
        }
        break;
    case 13:
        dummy = (BtlUnitMode4 *)BtlTutorialFindUnitByKind(task, 2);
        if (dummy == NULL) {
            break;
        }
        timer = self->stepTimer;
        if (timer > 0) {
            if (timer == 60) {
                dummy->base.input->allowedActions = ~0xda8u;
                self->stepTimer++;
                break;
            }
        } else if (timer >= 0) {
            dummy->base.input->allowedActions = 0;
            self->stepTimer++;
            break;
        }
        self->stepTimer = timer + 1;
        break;
    case 15:
    case 17:
    case 19:
    case 21:
        break;
    case 23:
        g_scriptGlobalVars[11] = 0x18;
        BtlTutorialSetControlLock(task, true, NULL);
        break;
    default:
        /* even steps 1..0xe, 0x10..0x16, and negative steps */
        if (step < 15) {
            BtlCombatArtSlotsNoOp();
        }
        break;
    }
}
