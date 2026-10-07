// bdc 0x08993530 UiUnlockCodeValidateCode
#include "bdc.h"

/* Checks the normalised code of `UiUnlockCode`. In 8-digit mode
   (`dimensionsMode` = 0) it must equal the fixed key sequence {7, 0x17, 0x15, 0xa6, 0x18, 0xa7, 1, 5}.
   In 10-digit ("Dimensions") mode it is rejected when all characters are equal, when it matches either
   of the two built-in codes `g_unlockCodeRejectedCodes`, when it contains a blank (-1 or 0x27), or
   when it equals a code already registered in one of the 8 profile slots (`unlockCodeSlots` bits,
   codes in `unlockCodes[slot]`); anything else is accepted. Returns 1 if valid, 0 otherwise. */

int UiUnlockCodeValidateCode(UiUnlockCode *self)
{
    int key[8];
    int rejected[2][10];
    int count;
    int matches;
    int i;
    int slot;
    int c;

    key[0] = 7;
    key[1] = 0x17;
    key[2] = 0x15;
    key[3] = 0xa6;
    key[4] = 0x18;
    key[5] = 0xa7;
    key[6] = 1;
    key[7] = 5;
    memcpy(rejected, g_unlockCodeRejectedCodes, sizeof(rejected));

    count = self->digitCount;
    if (self->dimensionsMode == 0) {
        for (i = 0; i < count; i++) {
            if (self->normalized[i] != key[i]) {
                return 0;
            }
        }
        return 1;
    }

    /* all characters equal */
    matches = 1;
    for (i = 1; i < count; i++) {
        if (self->normalized[0] == self->normalized[i]) {
            matches++;
        }
    }
    if (matches == count) {
        return 0;
    }

    /* first built-in code */
    matches = 0;
    for (i = 0; i < count; i++) {
        if (self->normalized[i] == rejected[0][i]) {
            matches++;
        }
    }
    if (matches == count) {
        return 0;
    }

    /* blanks */
    for (i = 0; i < count; i++) {
        c = self->normalized[i];
        if (c < 0) {
            if (!(c < -1)) {
                return 0;
            }
        } else if (c == 0x27) {
            return 0;
        }
    }

    /* second built-in code */
    matches = 0;
    for (i = 0; i < count; i++) {
        if (self->normalized[i] == rejected[1][i]) {
            matches++;
        }
    }
    if (matches == count) {
        return 0;
    }

    /* codes already registered in the profile */
    for (slot = 0; slot < 8; slot++) {
        if ((SaveGetProfile()->data->unlockCodeSlots & (1 << slot)) != 0) {
            matches = 0;
            for (i = 0; i < self->digitCount; i++) {
                const SaveProfileData *data = SaveGetProfile()->data;

                if (self->normalized[i] == data->unlockCodes[slot][i]) {
                    matches++;
                }
            }
            if (matches == self->digitCount) {
                return 0;
            }
        }
    }
    return 1;
}
