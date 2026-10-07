// bdc 0x0884c988 BtlMainStartResultDemo
#include "bdc.h"

/* Starts the end-of-battle demo for result `outcome`. Outcome 1 picks the variant-3 demo
   (`BtlDemoIdVariant3`) of the player's Bakugan kind (kind 1 without a player Bakugan) and, in
   rule mode 1, marks the current stage's profile state 2; outcomes 2..4 pick the variant-1 demo
   (`BtlDemoIdVariant1`) and mark the stage state 1; other outcomes pick none (-1). No demo
   either while another round must be fought (`BtlMainIsMatchUndecided`) or when profile flag 0
   is set, a profile exists and its word 0 has any of bits 0x4880. With a demo and no running
   intro demo task (0x65), it creates the demo task (`BtlDemoCreate`) and hands it model lists 1
   and 2. Finally it clears `g_btlCameraDefaultMode`, stops every unattached stage effect
   (`GfxEffectStopUnattached`) and returns the demo id, or -1. */

int BtlMainStartResultDemo(BtlMain *self, int outcome)
{
    BtlBakugan *player;
    BtlDemo *demo;
    SaveProfile *profile;
    int kind;
    int demoId;

    player = (BtlBakugan *)BtlGetPlayerBakugan();
    demoId = -1;
    if (outcome == 1) {
        kind = 1;
        if (player != NULL) {
            kind = player->base.base.unk08;
        }
        demoId = BtlDemoIdVariant3(kind);
        if (g_scriptGlobalVars[8] == 1) {
            profile = (SaveProfile *)SaveGetProfile();
            profile->data->stageStates[g_scriptGlobalVars[1]] = 2;
        }
    } else if (outcome >= 2 && outcome <= 4) {
        kind = 1;
        if (player != NULL) {
            kind = player->base.base.unk08;
        }
        demoId = BtlDemoIdVariant1(kind);
        if (g_scriptGlobalVars[8] == 1) {
            profile = (SaveProfile *)SaveGetProfile();
            profile->data->stageStates[g_scriptGlobalVars[1]] = 1;
        }
    }
    if (BtlMainIsMatchUndecided(self)) {
        demoId = -1;
    }
    if (SaveGetProfileFlag0() != 0 && SaveHasProfile() &&
        SaveProfileHasFlags((SaveProfile *)SaveGetProfile(), 0x4880)) {
        demoId = -1;
    }
    /* the binary also loads 1 into a1 for this call; the callee takes one argument */
    if (demoId != -1 && CoreTaskExists(0x65) == 0) {
        demo = (BtlDemo *)BtlDemoCreate(demoId);
        demo->mapModels = &self->modelLists[1];
        demo->models = &self->modelLists[2];
    }
    g_btlCameraDefaultMode = 0;
    GfxEffectStopUnattached(self->stageEffects, -1);
    return demoId;
}
