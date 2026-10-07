// bdc 0x08839f34 BtlHudUpdateAdvice
#include "bdc.h"

/* HUD widget updater (`BtlHudPhaseMain`) for the battle advisor's comments. Does nothing once
   the battle is over (`g_btlBattleOver`), with profile flag 0 set, in rule mode 2 (script
   global 8), without a player Bakugan, while UI window 0xb is inactive, while the camera/battle
   main task (task 100) exists but is not in phase 1, or while the player's input is disabled.
   Otherwise it bumps the frame counter `adviceFrame`, takes the player's target (NULL when it is
   not in the Bakugan list or is dead), runs `BtlHudAdviceUpdateStageHints` and, unless the
   stage number (script global 1) is 0x24 or profile byte 0x6ab is 1, the generic triggers
   `BtlHudAdviceTrigger00`..`BtlHudAdviceTrigger04`; triggers 05..21 follow except on stages
   0x18 and 0x25. */

void BtlHudUpdateAdvice(BtlHud *self)
{
    BtlBakugan *player;
    BtlBakugan *target;
    s32 stage;

    player = (BtlBakugan *)BtlHudGetPlayerBakugan(self);
    if (g_btlBattleOver != 0) {
        return;
    }
    stage = g_scriptGlobalVars[1];
    if (SaveGetProfileFlag0() != 0) {
        return;
    }
    if (g_scriptGlobalVars[8] == 2) {
        return;
    }
    if (player == NULL) {
        return;
    }
    if (UiGetWindowActive(0xb) == 0) {
        return;
    }
    if (BtlCameraTaskExists() != 0 && ((BtlMain *)BtlGetCameraTask())->phase != 1) {
        return;
    }
    if (player->input->disabled != 0) {
        return;
    }
    self->adviceFrame = self->adviceFrame + 1;
    target = (BtlBakugan *)BtlBakuganListFind((BtlBakugan *)BtlBakuganGetTarget(player));
    if (target != NULL && target->combat.dead != 0) {
        target = NULL;
    }
    BtlHudAdviceUpdateStageHints(self, player);
    if (stage == 0x24) {
        return;
    }
    if (((SaveProfile *)SaveGetProfile())->data->adviceOff == 1) {
        return;
    }
    BtlHudAdviceTrigger00(self, player, 0);
    BtlHudAdviceTrigger01(self, player, 1);
    BtlHudAdviceTrigger02(self, player, 2);
    BtlHudAdviceTrigger03(self, player, target, 3);
    BtlHudAdviceTrigger04(self, player, target, 4);
    if (stage < 0x19) {
        if (stage >= 0x18) {
            return;
        }
    } else if (stage == 0x25) {
        return;
    }
    BtlHudAdviceTrigger05(self, player, 5);
    BtlHudAdviceTrigger06(self, player, 6);
    BtlHudAdviceTrigger07(self, player, 7);
    BtlHudAdviceTrigger08(self, player, 8);
    BtlHudAdviceTrigger09(self, player, 9);
    BtlHudAdviceTrigger10(self, player, 10);
    BtlHudAdviceTrigger11(self, player, 11);
    BtlHudAdviceTrigger12(self, player, 12);
    BtlHudAdviceTrigger13(self, player, 13);
    BtlHudAdviceTrigger14(self, player, 14);
    BtlHudAdviceTrigger15(self, player, 15);
    BtlHudAdviceTrigger16(self, player, target, 16);
    BtlHudAdviceTrigger17(self, player, target, 17);
    BtlHudAdviceTrigger18(self, player, target, 18);
    BtlHudAdviceTrigger19(self, player, 19);
    BtlHudAdviceTrigger20(self, player, 20);
    BtlHudAdviceTrigger21(self, player, 21);
}
