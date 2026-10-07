// bdc 0x0891bcac UiHologramGalleryPickFirstHelp
#include "bdc.h"

/* During the tutorial (profile word 0x2b == 2, word 0x2e == 0) returns which first-visit help to
   show for the current stage (script global var 1, `g_scriptGlobalVars`): 1, 2 or 3 for the three
   groups of stages, marking it and every lower group seen in profile `helpSeen[0]` (bits 0..2).
   Returns 0 when the help was already seen, outside the tutorial, or for any other stage. */

int UiHologramGalleryPickFirstHelp(UiHologramGallery *self)
{
    int help = 0;

    if (SaveProfileGetWord(SaveGetProfile(), 0x2b) != 2) {
        return help;
    }
    if (SaveProfileGetWord(SaveGetProfile(), 0x2e) != 0) {
        return help;
    }
    switch ((u32)g_scriptGlobalVars[1]) {
    case 0: case 1: case 36:
        break;
    case 2: case 4: case 5: case 6: case 16: case 17: case 18:
        if ((SaveGetProfile()->data->helpSeen[0] & 1) == 0) {
            help = 1;
            SaveGetProfile()->data->helpSeen[0] |= 1;
        }
        break;
    case 12: case 13: case 14:
        if ((SaveGetProfile()->data->helpSeen[0] & 2) == 0) {
            SaveGetProfile()->data->helpSeen[0] |= 1;
            help = 2;
            SaveGetProfile()->data->helpSeen[0] |= 2;
        }
        break;
    case 8: case 9: case 10: case 20: case 21: case 22: case 24: case 25: case 26: case 37: case 38:
        if ((SaveGetProfile()->data->helpSeen[0] & 4) == 0) {
            SaveGetProfile()->data->helpSeen[0] |= 1;
            SaveGetProfile()->data->helpSeen[0] |= 2;
            help = 3;
            SaveGetProfile()->data->helpSeen[0] |= 4;
        }
        break;
    default:
        break;
    }
    return help;
}
