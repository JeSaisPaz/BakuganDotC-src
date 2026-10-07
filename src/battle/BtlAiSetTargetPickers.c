// bdc 0x0888dab8 BtlAiSetTargetPickers
#include "bdc.h"

/* Fills the four target-picker `MemberFnPtr` slots `targetPickers` (`+0x94c..+0x96b`) of a
   `BtlAi` according to its targeting style `targetStyle` (`+0x1a8`). All four slots are first
   set to `g_btlAiNullTargetPicker`; then, from `g_btlAiTargetPickers`:
   style 0: `BtlAiPickTargetId`, `BtlAiPickNearestClass84InRange` (slots 2-3 stay null);
   style 1: `BtlAiPickNearestClass84InRange`, `BtlAiPickNearestClass5COr7CInView`,
            `BtlAiPickNearestClass54InView`, `BtlAiPickNearestClass84InView`;
   style 2: `BtlAiPickNearestClass54InView`, `BtlAiPickNearestClass84InRange`,
            `BtlAiPickNearestClass5COr7CInView`, `BtlAiPickNearestClass84InView`;
   style 3: `BtlAiPickNearestClass84InRange`, `BtlAiPickNearestClass54InView`,
            `BtlAiPickNearestClass5COr7CInView`, `BtlAiPickNearestClass84InView`;
   any other style leaves all four null. */
void BtlAiSetTargetPickers(BtlAi *self)
{
    s8 i;

    for (i = 0; i < 4; i++) {
        self->targetPickers[i] = g_btlAiNullTargetPicker;
    }
    switch (self->targetStyle) {
    case 0:
        self->targetPickers[0] = g_btlAiTargetPickers[0];
        self->targetPickers[1] = g_btlAiTargetPickers[1];
        break;
    case 1:
        self->targetPickers[0] = g_btlAiTargetPickers[1];
        self->targetPickers[1] = g_btlAiTargetPickers[2];
        self->targetPickers[2] = g_btlAiTargetPickers[3];
        self->targetPickers[3] = g_btlAiTargetPickers[4];
        break;
    case 2:
        self->targetPickers[0] = g_btlAiTargetPickers[3];
        self->targetPickers[1] = g_btlAiTargetPickers[1];
        self->targetPickers[2] = g_btlAiTargetPickers[2];
        self->targetPickers[3] = g_btlAiTargetPickers[4];
        break;
    case 3:
        self->targetPickers[0] = g_btlAiTargetPickers[1];
        self->targetPickers[1] = g_btlAiTargetPickers[3];
        self->targetPickers[2] = g_btlAiTargetPickers[2];
        self->targetPickers[3] = g_btlAiTargetPickers[4];
        break;
    default:
        break;
    }
}
