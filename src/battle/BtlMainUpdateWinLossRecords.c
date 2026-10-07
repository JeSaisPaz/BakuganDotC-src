// bdc 0x0884e474 BtlMainUpdateWinLossRecords
#include "bdc.h"

/* Updates the per-team win/loss profile counters after a battle (skipped in rule mode 1, script
   global 8). Unless profile word 7 is 1 or 2 (ranked), adds 1 to word `0x1f + team` for each team
   `BtlMainGetTeamOutcome` reports as won (1) and to `0x23 + team` for each lost (2). Ranked: counts
   the players present (profile words 3..6 > 0) and the first places (`BtlMainGetPlayerPlacing`
   == 0); every first place gets a win unless all present players placed first (then nothing),
   every other player a loss. */

void BtlMainUpdateWinLossRecords(BtlMain *self)
{
    s32 mode;
    s32 team;

    if (g_scriptGlobalVars[8] == 1) {
        return;
    }
    mode = (s32)SaveProfileGetWord(SaveGetProfile(), 7);
    if (mode < 1 || mode > 2) {
        for (team = 0; team < 4; team++) {
            int outcome = BtlMainGetTeamOutcome(self, team);

            if (outcome == 1) {
                SaveProfileAddWord(SaveGetProfile(), team + 0x1f, 1);
            } else if (outcome == 2) {
                SaveProfileAddWord(SaveGetProfile(), team + 0x23, 1);
            }
        }
    } else {
        s32 present = 0;
        s32 firsts = 0;
        bool allTied = false;

        for (team = 0; team < 4; team++) {
            if ((s32)SaveProfileGetWord(SaveGetProfile(), team + 3) > 0) {
                present++;
            }
        }
        for (team = 0; team < 4; team++) {
            if (BtlMainGetPlayerPlacing(self, team) == 0) {
                firsts++;
            }
        }
        if (present == firsts) {
            allTied = true;
        }
        for (team = 0; team < 4; team++) {
            if (BtlMainGetPlayerPlacing(self, team) == 0) {
                if (!allTied) {
                    SaveProfileAddWord(SaveGetProfile(), team + 0x1f, 1);
                }
            } else {
                SaveProfileAddWord(SaveGetProfile(), team + 0x23, 1);
            }
        }
    }
}
