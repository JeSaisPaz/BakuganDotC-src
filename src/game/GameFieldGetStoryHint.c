// bdc 0x088c244c GameFieldGetStoryHint
#include "bdc.h"

/* Returns the story hint shown for the current stage (script variable 1) in
   `*outChapter`/`*outHint`: fixed values per stage (e.g. 1 → (1,1), 5 → (3,4), 8 → (4,3),
   …, −1 for stages without one) and, on the hub stage 0x20, the latest reached story flag in
   the bitset `g_gameEventFlags + 8` decides (later tests override earlier ones; (0,0) when none
   is set). Does nothing when either pointer is NULL. */

void GameFieldGetStoryHint(CoreTask *task, s32 *outChapter, s16 *outHint)
{
    const u32 *bits;

    if (outChapter == NULL || outHint == NULL) {
        return;
    }

    switch ((u32)g_scriptGlobalVars[1]) {
    case 0:
        *outChapter = 0;
        *outHint = 0;
        break;
    case 1:
        *outChapter = 1;
        *outHint = 1;
        break;
    case 4:
        *outChapter = 2;
        *outHint = 3;
        break;
    case 5:
        *outChapter = 3;
        *outHint = 4;
        break;
    case 8:
        *outChapter = 4;
        *outHint = 3;
        break;
    case 9:
        *outChapter = 5;
        *outHint = 3;
        break;
    case 12:
        *outChapter = 6;
        *outHint = 9;
        break;
    case 14:
        *outChapter = 7;
        *outHint = 1;
        break;
    case 16:
        *outChapter = 8;
        *outHint = 12;
        break;
    case 17:
        *outChapter = 9;
        *outHint = 13;
        break;
    case 20:
        *outChapter = 10;
        *outHint = 15;
        break;
    case 32:
        *outChapter = 0;
        *outHint = 0;
        break;
    default:
        *outChapter = -1;
        *outHint = 0;
        break;
    }

    if (g_scriptGlobalVars[1] != 0x20) {
        return;
    }

    *outChapter = 0;
    *outHint = 0;
    bits = (const u32 *)&g_gameEventFlags[8];
    if ((*bits >> 2) & 1) {
        *outChapter = 1;
        *outHint = 1;
    }
    if ((*bits >> 3) & 1) {
        *outChapter = 2;
        *outHint = 3;
    }
    if ((*bits >> 6) & 1) {
        *outChapter = 3;
        *outHint = 4;
    }
    if ((*bits >> 7) & 1) {
        *outChapter = 3;
        *outHint = 5;
    }
    if ((*bits >> 8) & 1) {
        *outChapter = 8;
        *outHint = 12;
    }
    if ((*bits >> 18) & 1) {
        *outChapter = 9;
        *outHint = 13;
    }
    if ((*bits >> 19) & 1) {
        *outChapter = 1;
        *outHint = 2;
    }
    if ((*bits >> 4) & 1) {
        *outChapter = 9;
        *outHint = 14;
    }
    if ((*bits >> 20) & 1) {
        *outChapter = 6;
        *outHint = 9;
    }
    if ((*bits >> 14) & 1) {
        *outChapter = 7;
        *outHint = 10;
    }
    if ((*bits >> 15) & 1) {
        *outChapter = 7;
        *outHint = 11;
    }
    if ((*bits >> 16) & 1) {
        *outChapter = 4;
        *outHint = 6;
    }
    if ((*bits >> 10) & 1) {
        *outChapter = 5;
        *outHint = 7;
    }
    if ((*bits >> 11) & 1) {
        *outChapter = 5;
        *outHint = 8;
    }
    if ((*bits >> 12) & 1) {
        *outChapter = 10;
        *outHint = 15;
    }
}
