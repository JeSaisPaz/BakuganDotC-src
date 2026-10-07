// bdc 0x088a97a8 BtlItemRollKind
#include "bdc.h"

/* Rolls the kind of an item to drop from four percentages. Returns 6 (none) while a battle demo
   runs (camera task alive and BtlIsDemoRunning) or, in battle rule mode 2 (script global
   variable 8), when profile word 0x1a is 1. Otherwise draws r in [0, 1000): r < pctKind12*10
   gives kind 1 or 2 (coin flip), then the next pctKind3*10 gives kind 3, the next pctKind0*10
   kind 0, and everything else 6 (the pctNone*10 band and the rest alike). Kind 0 becomes 6 while
   a kind-0 item already exists or the arena colour flash is active. */

int BtlItemRollKind(int pctKind12, int pctKind3, int pctKind0, int pctNone)
{
    s32 kind12[2];
    s32 kind;
    s32 roll;
    s32 end12;
    s32 end3;
    s32 end0;

    kind = 6;
    if (BtlCameraTaskExists() != 0) {
        BtlGetCameraTask();
        if (BtlIsDemoRunning()) {
            return 6;
        }
    }
    end12 = pctKind12 * 10;
    if (g_scriptGlobalVars[8] == 2) {
        if (SaveProfileGetWord(SaveGetProfile(), 0x1a) == 1) {
            return 6;
        }
    }
    roll = (s32)CoreRandNext(1000);
    if (roll < end12) {
        kind12[0] = 1;
        kind12[1] = 2;
        kind = kind12[CoreRandNext(2)];
    } else {
        end3 = end12 + pctKind3 * 10;
        end0 = end3 + pctKind0 * 10;
        if (roll < end3) {
            kind = 3;
        } else if (roll < end0) {
            kind = 0;
        } else if (roll >= end0 + pctNone * 10) {
            return 6;
        }
    }
    if (kind == 0 && (BtlItemListCountKind(0) != 0 || g_btlMapFlashState != 0)) {
        kind = 6;
    }
    return kind;
}
