// bdc 0x0883a8a0 BtlHudPickResultVoice
#include "bdc.h"

/* Chooses the voice line for the battle result screen (`BtlHudRatingResultScreen`,
   `BtlHudArenaResultScreen`). Returns -1 when profile flag 0 is set but window kind 4 is not
   active. In battle rule mode 1 (script global 8) the speaker is the player's own Bakugan
   (`SaveGetTeamBakuganOrder(-1)`, character 0 without a profile). Otherwise profile word 7
   picks the rule: 0 = the slot whose `BtlMainGetTeamOutcome` is 1 (win), 1..2 = the slot whose
   `BtlMainGetPlayerPlacing` is 0 (first; skipped when every active player is first), both over
   the camera task; with profile flag 0 set it is the player's own Bakugan instead. Exactly one
   speaker must be found, else -1 (also for word 7 < 0 or >= 3). Returns `0x2a3f + 3*character +
   random%3`, rerolling ids rejected by `BtlHudIsExcludedResultVoice`. */

int BtlHudPickResultVoice(BtlHud *self)
{
    s32 character = 0;
    u8 speakers;
    u8 active;
    u8 slot;
    s32 rule;
    s32 positive;
    s32 first;
    s32 i;
    u32 rnd;
    u32 scaled;
    s32 base;
    s32 voice;

    if (SaveGetProfileFlag0() != 0 && UiGetWindowActive(4) == 0) {
        return -1;
    }
    if (g_scriptGlobalVars[8] == 1) {
        if (SaveHasProfile()) {
            character = SaveGetTeamBakuganOrder(SaveGetProfile(), -1);
        }
    } else {
        speakers = 0;
        active = 0;
        for (slot = 0; slot < 4; slot++) {
            if ((s32)SaveProfileGetWord(SaveGetProfile(), slot + 3) > 0) {
                active++;
            }
        }
        rule = (s32)SaveProfileGetWord(SaveGetProfile(), 7);
        if (rule == 0) {
            if (SaveGetProfileFlag0() != 0) {
                if (SaveHasProfile()) {
                    character = SaveGetTeamBakuganOrder(SaveGetProfile(), -1);
                    speakers = 1;
                }
            } else if (BtlCameraTaskExists() && SaveHasProfile()) {
                for (slot = 0; slot < active; slot++) {
                    if (BtlMainGetTeamOutcome((BtlMain *)BtlGetCameraTask(), slot) == 1) {
                        character = SaveGetTeamBakuganOrder(SaveGetProfile(), slot);
                        speakers++;
                    }
                }
            }
        } else if (rule > 0 && rule < 3) {
            if (SaveGetProfileFlag0() != 0) {
                if (SaveHasProfile()) {
                    character = SaveGetTeamBakuganOrder(SaveGetProfile(), -1);
                    speakers = 1;
                }
            } else if (BtlCameraTaskExists()) {
                positive = 0;
                for (i = 0; i < 4; i++) {
                    if ((s32)SaveProfileGetWord(SaveGetProfile(), i + 3) > 0) {
                        positive++;
                    }
                }
                first = 0;
                for (i = 0; i < 4; i++) {
                    if (BtlMainGetPlayerPlacing((BtlMain *)BtlGetCameraTask(), i) == 0) {
                        first++;
                    }
                }
                if (positive != first && SaveHasProfile()) {
                    for (slot = 0; slot < active; slot++) {
                        if (BtlMainGetPlayerPlacing((BtlMain *)BtlGetCameraTask(), slot) == 0) {
                            character = SaveGetTeamBakuganOrder(SaveGetProfile(), slot);
                            speakers++;
                        }
                    }
                }
            }
        }
        if (speakers != 1) {
            return -1;
        }
    }
    base = character * 3 + 0x2a3f;
    do {
        rnd = PlatformRandU32();
        scaled = ((rnd >> 16) * 0xffff) >> 16;
        voice = base + (s32)((scaled + (CoreRtcGetMicrosecond() & 0xffff)) % 3);
    } while (BtlHudIsExcludedResultVoice(self, voice) != 0);
    return voice;
}
