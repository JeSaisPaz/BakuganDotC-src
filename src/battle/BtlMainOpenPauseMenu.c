// bdc 0x0884e954 BtlMainOpenPauseMenu
#include "bdc.h"

/* Creates the pause menu task (id 0x19a, `UiPause`, priority 100) and suspends the stage event
   script (`BtlStageSuspendEventScript`). When the task exists it picks the menu kind: 0, or for
   rule mode 1 (script global variable 8) kind 1, 2 when global script bit 3 is set, and, when
   profile word 0x2e is non-zero, 3 (word 0x2b == 1) or 10 (word 0x2b == 2) unless bit 3 is set
   (2). Then fades out BGM channel 1 over 0.5 and opens the menu (`UiPauseSetMenuKind`). */

void BtlMainOpenPauseMenu(void)
{
    UiPause *pause;
    int kind;

    pause = (UiPause *)CoreTaskCreate(0x19a, 100);
    BtlStageSuspendEventScript();
    if (pause == NULL) {
        return;
    }
    kind = 0;
    if (g_scriptGlobalVars[8] == 1) {
        kind = 1;
        if (CoreBitsetTest(3, g_scriptGlobalBits) == 1) {
            kind = 2;
        }
        if (SaveProfileGetWord(SaveGetProfile(), 0x2e) != 0) {
            if (SaveProfileGetWord(SaveGetProfile(), 0x2b) == 1) {
                kind = 3;
            } else if (SaveProfileGetWord(SaveGetProfile(), 0x2b) == 2) {
                kind = 10;
            }
            if (CoreBitsetTest(3, g_scriptGlobalBits) == 1) {
                kind = 2;
            }
        }
    }
    SndBgmQueueStop(0.5f, 1);
    UiPauseSetMenuKind(pause, kind);
}
