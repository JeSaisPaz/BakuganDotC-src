// bdc 0x08837268 BtlHudUpdateRoundWinLamps
#include "bdc.h"

/* HUD widget (`BtlHudPhaseMain`, also driven by `BtlHudUpdateBattleEndBanner`) for the
   round-win lamps of a multi-round match. Returns at once while the camera task (the battle main
   task, `BtlCameraTaskExists`) is gone. Every frame the six lamp sprites `sprites[0xf1..0xf6]`
   pulse: their phase `scaleX` steps by 2 degrees (wrapping above 2*pi) and alpha =
   (1 - cos(phase * pi)) / 2 * 0.2. Then `lampState` steps:
   0 → 1, or → 10 when the battle rule mode (script global 8) is 2 and the round count (profile
   word 0x1b, clamped to 1..5) is at least 3; 10 clears the lamps and goes to 11, which waits for
   `g_btlBattleOutcome` 1/2/4 → 12; 12 tallies `roundResults[0..4]` of the battle main task
   (1 win, 2 draw, 3 loss; 1 and 3 swapped on a NetPlay guest via `NetPlayGetLocalSlot`) into
   player/opponent win counts (max 3 each, a draw counts for both) and picks the lamps of the
   current round (profile word 0x1e - 1): the player's `sprites[0xf0 + playerWins]` on a win or
   draw, the opponent's `sprites[0xf3 + opponentWins]` on a loss or draw → 13, or → 20 if none;
   13/14 fade them in (sin of an angle going 0 → pi/2 by 5 degrees; alpha times `hudAlpha` unless
   `BtlMainIsMatchUndecided`, only in rule mode 2) → 20 once sin reaches 1; 20 sets the
   `"shouri_on"` texture (`GfxFindTexture`) on the result marks `sprites[0xa2..0xa4]` (player)
   and `sprites[0xa5..0xa7]` (opponent) for every round won → 21; 21/22 fade the lamps back out
   (angle pi/2 → 0 by 10 degrees) until their alpha is no longer above the pulse alpha of
   `sprites[0xf1]` → 30; 30 goes back to 10 while `BtlMainIsBattleOver` is false. States 1-9,
   15-19, 23-29 and anything above 30 do nothing.
   Sines and cosines are VFPU `vsin.s`/`vcos.s` of angle * 2/pi (bank S703) in quarter turns,
   i.e. sin/cos of the angle in radians. */

#define LAMP_FIRST        0xf1          /* six pulsing lamps sprites[0xf1..0xf6] */
#define LAMP_COUNT        6
#define LAMP_PLAYER_BASE  0xf0          /* + playerWins (1..3) */
#define LAMP_OPP_BASE     0xf3          /* + opponentWins (1..3) */
#define MARK_PLAYER_BASE  0xa2          /* result marks, 3 per side */
#define MARK_OPP_BASE     0xa5
#define ROUND_MAX         5

#define PULSE_STEP     0.0349065848f    /* 0x3d0efa35, 2 degrees */
#define TWO_PI         6.28318548f      /* 0x40c90fdb */
#define PI_F           3.14159274f      /* 0x40490fdb */
#define PULSE_AMP      0.200000003f     /* 0x3e4ccccd */
#define HALF_PI        1.57079637f      /* 0x3fc90fdb */
#define FADE_IN_STEP   0.0872664601f    /* 0x3db2b8c2, 5 degrees */
#define FADE_OUT_STEP  0.174532920f     /* 0x3e32b8c2, 10 degrees */

/* VFPU vsin.s of angle * S703 (2/pi) quarter turns: sin(angle). */
static inline float LampVfpuSin(float angle)
{
    return __builtin_sinf(angle);
}

/* VFPU vcos.s of angle * S703 (2/pi) quarter turns: cos(angle). */
static inline float LampVfpuCos(float angle)
{
    return __builtin_cosf(angle);
}

/* Pulse alpha of a lamp whose phase is `phase`: (1 - cos(phase * pi)) / 2 * 0.2, not below 0. */
static inline float LampPulseAlpha(float phase)
{
    float alpha = (1.0f - LampVfpuCos(phase * PI_F)) * 0.5f * PULSE_AMP;
    if (alpha < 0.0f) {
        alpha = 0.0f;
    }
    return alpha;
}

/* Result of round `round` (clamped to 0..4) from the battle main task, seen from the local
   player: on a NetPlay guest (local slot != 0) a win (1) and a loss (3) are swapped. */
static s32 BtlHudLocalRoundResult(s32 round)
{
    BtlMain *battle = BtlGetCameraTask();
    s32 result;

    if (round < 0) {
        round = 0;
    } else if (round > ROUND_MAX - 1) {
        round = ROUND_MAX - 1;
    }
    result = battle->roundResults[round];
    if (NetPlayHasManager() && NetPlayGetLocalSlot(NetPlayGetManager()) != 0) {
        if (result == 1) {
            result = 3;
        } else if (result == 3) {
            result = 1;
        }
    }
    return result;
}

/* Fade step shared by states 13/14 and 21/22: stores `angle`, clamps it to [0, pi/2] (NaN →
   pi/2), stores that, and in rule mode 2 sets both chosen lamps' alpha to its sine (times
   `hudAlpha` unless the match is undecided). Returns the sine. */
static float BtlHudLampFade(BtlHud *self, float angle)
{
    float level;

    self->lampAngle = angle;
    if (angle < 0.0f) {
        angle = 0.0f;
    } else if (!(angle <= HALF_PI)) {
        angle = HALF_PI;
    }
    self->lampAngle = angle;
    level = LampVfpuSin(angle);
    if (g_scriptGlobalVars[8] == 2) {
        if (BtlMainIsMatchUndecided(BtlGetCameraTask())) {
            if (self->lampLit != NULL) {
                self->lampLit->alpha = level;
            }
            if (self->lampPulse != NULL) {
                self->lampPulse->alpha = level;
            }
        } else {
            if (self->lampLit != NULL) {
                self->lampLit->alpha = level * self->hudAlpha;
            }
            if (self->lampPulse != NULL) {
                self->lampPulse->alpha = level * self->hudAlpha;
            }
        }
    }
    return level;
}

void BtlHudUpdateRoundWinLamps(BtlHud *self)
{
    GfxSprite *playerLamp;
    GfxSprite *opponentLamp;
    GfxSprite *mark;
    s32 state;
    s32 rounds;
    s32 outcome;
    s32 result;
    s32 playerWins;
    s32 opponentWins;
    s32 i;
    float level;

    if (!BtlCameraTaskExists()) {
        return;
    }

    for (i = 0; i < LAMP_COUNT; i++) {
        self->sprites[LAMP_FIRST + i]->scaleX += PULSE_STEP;
        if (!(self->sprites[LAMP_FIRST + i]->scaleX <= TWO_PI)) {
            self->sprites[LAMP_FIRST + i]->scaleX -= TWO_PI;
        }
        self->sprites[LAMP_FIRST + i]->alpha =
            LampPulseAlpha(self->sprites[LAMP_FIRST + i]->scaleX);
    }

    state = self->lampState;
    switch ((u32)state) {
    case 0:
        rounds = (s32)SaveProfileGetWord(SaveGetProfile(), 0x1b);
        if (rounds < 1) {
            rounds = 1;
        } else if (rounds > 5) {
            rounds = 5;
        }
        self->lampState = 1;
        if (g_scriptGlobalVars[8] == 2 && rounds >= 3) {
            self->lampState = 10;
        }
        break;

    case 10:
        self->lampLit = NULL;
        self->lampPulse = NULL;
        self->lampAngle = 0.0f;
        self->lampState = state + 1;
        /* fall through */
    case 11:
        outcome = g_btlBattleOutcome;
        if (outcome == 1 || outcome == 2 || outcome == 4) {
            self->lampState++;
        }
        break;

    case 12:
        playerWins = 0;
        opponentWins = 0;
        for (i = 0; i < ROUND_MAX; i++) {
            result = BtlHudLocalRoundResult(i);
            if (result < 2) {
                if (result > 0 && playerWins < 3) {
                    playerWins++;
                }
            } else if (result < 3) {
                if (playerWins < 3) {
                    playerWins++;
                }
                if (opponentWins < 3) {
                    opponentWins++;
                }
            } else if (result < 4) {
                if (opponentWins < 3) {
                    opponentWins++;
                }
            }
        }
        result = BtlHudLocalRoundResult((s32)SaveProfileGetWord(SaveGetProfile(), 0x1e) - 1);
        playerLamp = NULL;
        opponentLamp = NULL;
        if (result < 2) {
            if (result > 0) {
                playerLamp = self->sprites[LAMP_PLAYER_BASE + playerWins];
            }
        } else if (result < 3) {
            playerLamp = self->sprites[LAMP_PLAYER_BASE + playerWins];
            opponentLamp = self->sprites[LAMP_OPP_BASE + opponentWins];
        } else if (result < 4) {
            opponentLamp = self->sprites[LAMP_OPP_BASE + opponentWins];
        }
        if (playerLamp != NULL || opponentLamp != NULL) {
            self->lampLit = playerLamp;
            self->lampPulse = opponentLamp;
            self->lampState++;
        } else {
            self->lampState = 20;
        }
        break;

    case 13:
        if (self->lampLit != NULL) {
            self->lampLit->flags |= 1;
            self->lampLit->alpha = 0.0f;
        }
        if (self->lampPulse != NULL) {
            self->lampPulse->flags |= 1;
            self->lampPulse->alpha = 0.0f;
        }
        self->lampAngle = 0.0f;
        self->lampState++;
        /* fall through */
    case 14:
        if (BtlHudLampFade(self, self->lampAngle + FADE_IN_STEP) < 1.0f) {
            break;
        }
        self->lampState = 20;
        /* fall through */
    case 20:
        playerWins = 0;
        opponentWins = 0;
        for (i = 0; i < ROUND_MAX; i++) {
            result = BtlHudLocalRoundResult(i);
            if (result < 2) {
                if (result > 0 && playerWins < 3) {
                    mark = self->sprites[MARK_PLAYER_BASE + playerWins];
                    playerWins++;
                    mark->texture = GfxFindTexture("shouri_on");
                }
            } else if (result < 3) {
                if (playerWins < 3) {
                    mark = self->sprites[MARK_PLAYER_BASE + playerWins];
                    playerWins++;
                    mark->texture = GfxFindTexture("shouri_on");
                }
                if (opponentWins < 3) {
                    mark = self->sprites[MARK_OPP_BASE + opponentWins];
                    opponentWins++;
                    mark->texture = GfxFindTexture("shouri_on");
                }
            } else if (result < 4) {
                if (opponentWins < 3) {
                    mark = self->sprites[MARK_OPP_BASE + opponentWins];
                    opponentWins++;
                    mark->texture = GfxFindTexture("shouri_on");
                }
            }
        }
        state = self->lampState + 1;
        self->lampState = state;
        /* fall through */
    case 21:
        self->lampAngle = HALF_PI;
        self->lampState = state + 1;
        /* fall through */
    case 22:
        level = BtlHudLampFade(self, self->lampAngle - FADE_OUT_STEP);
        if (level <= LampPulseAlpha(self->sprites[LAMP_FIRST]->scaleX)) {
            self->lampState = 30;
        }
        break;

    case 30:
        (void)BtlGetCameraTask();
        if (!BtlMainIsBattleOver()) {
            self->lampState = 10;
        }
        break;

    default:
        break;
    }
}
