// bdc 0x0883363c BtlHudUpdateScoreGainPopup
#include "bdc.h"

/* Per-player "+N" score popup state machine (state `gainPopupState[player]`, run by
   `BtlHudUpdateScoreBoard`; does nothing when `BtlHudGetPlayerScore` returns -1 or the state
   is outside 0..7). Uses the player's score icon and two popup sprites
   (`BtlHudGetScoreSprite` parts 0, 1, 2). Each state falls into the next one in the same frame
   when it advances:
   0 clears the gain counters, the icon fade alpha and `scoreCountUp`, hides the three sprites;
   1 waits for a gain (`BtlHudPollScoreGain`); 2 clears `scoreCountUp`/`scoreCountDone`;
   3 fades the icon in by 0.2 per frame until its alpha is no longer < 1, then sets it to 1;
   4 puts the two popup sprites at their saved positions `gainPopupPos[player]`, makes them opaque
   and counts `gainShown` up by one per frame until it reaches `gainPending` (then clamps it and
   zeroes `gainHoldTimer`); 5 returns to state 2 on a new gain, else decrements the hold timer and
   moves on once it is <= 0; 6 moves both popup sprites up 1 unit per frame and fades all three
   sprites by 0.08: once the first popup sprite's alpha is <= 0 all three are hidden and the state
   advances, otherwise an alpha <= 0.8 starts the score count-up (`scoreCountUp` = 1); 7 waits for
   `scoreCountDone` and returns to 0. */
void BtlHudUpdateScoreGainPopup(BtlHud *self, s32 player)
{
    GfxSprite *icon;
    GfxSprite *popupA;
    GfxSprite *popupB;

    if (BtlHudGetPlayerScore(self, player) == -1) {
        return;
    }
    icon = BtlHudGetScoreSprite(self, 0, player);
    popupA = BtlHudGetScoreSprite(self, 1, player);
    popupB = BtlHudGetScoreSprite(self, 2, player);

    switch (self->gainPopupState[player]) {
    case 0:
        self->gainPending[player] = 0;
        self->gainIconAlpha[player] = 0.0f;
        self->gainShown[player] = 0;
        icon->alpha = 0.0f;
        popupA->alpha = 0.0f;
        popupB->alpha = 0.0f;
        self->scoreCountUp[player] = 0;
        self->gainPopupState[player]++;
        /* fall through */
    case 1:
        if (BtlHudPollScoreGain(self, player) == 0) {
            break;
        }
        self->gainPopupState[player]++;
        /* fall through */
    case 2:
        self->scoreCountUp[player] = 0;
        self->scoreCountDone[player] = 0;
        self->gainPopupState[player]++;
        /* fall through */
    case 3:
        if (self->gainIconAlpha[player] < 1.0f) {
            self->gainIconAlpha[player] = self->gainIconAlpha[player] + 0.200000003f;
            icon->alpha = self->gainIconAlpha[player];
            break;
        }
        self->gainIconAlpha[player] = 1.0f;
        icon->alpha = 1.0f;
        self->gainPopupState[player]++;
        /* fall through */
    case 4:
        /* two quad copies (lv.q/sv.q): the popup sprites go to their saved positions */
        popupA->posX = self->gainPopupPos[player][0][0];
        popupA->posY = self->gainPopupPos[player][0][1];
        popupA->posZ = self->gainPopupPos[player][0][2];
        popupA->posW = self->gainPopupPos[player][0][3];
        popupB->posX = self->gainPopupPos[player][1][0];
        popupB->posY = self->gainPopupPos[player][1][1];
        popupB->posZ = self->gainPopupPos[player][1][2];
        popupB->posW = self->gainPopupPos[player][1][3];
        popupA->alpha = 1.0f;
        popupB->alpha = 1.0f;
        self->gainShown[player]++;
        if (self->gainShown[player] < self->gainPending[player]) {
            break;
        }
        self->gainShown[player] = self->gainPending[player];
        self->gainHoldTimer[player] = 0;
        self->gainPopupState[player]++;
        /* fall through */
    case 5:
        if (BtlHudPollScoreGain(self, player) != 0) {
            self->gainPopupState[player] = 2;
            break;
        }
        self->gainHoldTimer[player]--;
        if (self->gainHoldTimer[player] > 0) {
            break;
        }
        self->gainPopupState[player]++;
        /* fall through */
    case 6:
        popupA->posY = popupA->posY - 1.0f;
        popupB->posY = popupB->posY - 1.0f;
        popupA->alpha = popupA->alpha - 0.0799999982f;
        popupB->alpha = popupB->alpha - 0.0799999982f;
        icon->alpha = icon->alpha - 0.0799999982f;
        if (popupA->alpha <= 0.0f) {
            icon->alpha = 0.0f;
            popupA->alpha = 0.0f;
            popupB->alpha = 0.0f;
            self->gainPopupState[player]++;
        } else if (popupA->alpha <= 0.800000012f) {
            self->scoreCountUp[player] = 1;
        }
        break;
    case 7:
        if (self->scoreCountDone[player] != 0) {
            self->gainPopupState[player] = 0;
        }
        break;
    }
}
