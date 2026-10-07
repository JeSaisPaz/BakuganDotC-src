// bdc 0x088f54a4 GameEventFlagsAdvanceStage
#include "bdc.h"

/* Resets the story event flags at the start of the next play-through stage: first sets a list of
   event flags (a 0x54-entry list, or a shorter 0x15-entry list when event flag `0x215` is already
   set), then clears a 0x193-entry list of event flags. Called only by
   `ScriptOpStartNextPlaythrough`, right before `SaveProfileStoreEventFlags`. The name follows
   its only caller. */

void GameEventFlagsAdvanceStage(void)
{
    s32 i;

    if (GameEventFlagTest(0x215)) {
        for (i = 0; i < 0x15; i++) {
            GameEventFlagSet(g_eventFlagsSetShort[i]);
        }
    } else {
        for (i = 0; i < 0x54; i++) {
            GameEventFlagSet(g_eventFlagsSetFull[i]);
        }
    }
    for (i = 0; i < 0x193; i++) {
        GameEventFlagClear(g_eventFlagsClearList[i]);
    }
}
