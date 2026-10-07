// bdc 0x0884cd50 BtlGetExitScene
#include "bdc.h"

/* Returns the scene/transition code to use after the battle, from the battle rule mode (script
   global 8), the battle kind (script global 3), the outcome `g_btlBattleOutcome` and profile
   word 0x2e. Mode 1: 1 (8 when profile word 0x2e is set), overridden by kind 4 -> 4, 6 -> 9,
   9 -> 2, 11 -> 5; for code 5 it also records the outcome (`SaveProfileSetBattleOutcome`:
   outcome 0 -> 2, 1 -> 0, 2 -> 1, else 0). Other modes: outcome 6 -> 6, 7 -> 10, else by kind
   3 -> 3, 4 -> 4, 9 -> 2, 10 -> 7, 12 -> 6, other kinds 0. */
int BtlGetExitScene(void *main)
{
    s32 kind = g_scriptGlobalVars[3];
    int scene;
    u8 outcome;

    (void)main;
    if (g_scriptGlobalVars[8] != 1) {
        if (g_btlBattleOutcome < 7) {
            if (g_btlBattleOutcome >= 6) {
                return 6;
            }
        } else if (g_btlBattleOutcome < 8) {
            return 10;
        }
        switch (kind) {
        case 3:
            return 3;
        case 4:
            return 4;
        case 9:
            return 2;
        case 10:
            return 7;
        case 12:
            return 6;
        default:
            return 0;
        }
    }
    scene = 1;
    if (SaveProfileGetWord(SaveGetProfile(), 0x2e) != 0) {
        scene = 8;
    }
    switch (kind) {
    case 4:
        scene = 4;
        break;
    case 6:
        scene = 9;
        break;
    case 9:
        scene = 2;
        break;
    case 11:
        scene = 5;
        break;
    default:
        break;
    }
    if (scene == 5) {
        switch (g_btlBattleOutcome) {
        case 0:
            outcome = 2;
            break;
        case 1:
            outcome = 0;
            break;
        case 2:
            outcome = 1;
            break;
        default:
            outcome = 0;
            break;
        }
        SaveProfileSetBattleOutcome(outcome);
    }
    return scene;
}
