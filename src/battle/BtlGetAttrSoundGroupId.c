// bdc 0x0880e038 BtlGetAttrSoundGroupId
#include "bdc.h"

/* Returns the sound group id of the attribute voice pack for Bakugan kind `bakugan` (1..20):
   `g_btlAttrSoundGroupTable[attr] + 3`, where `attr` is the kind's default attribute
   (`BtlGetKindAttribute`) and the table maps the six attributes to {0,4,1,2,5,3}, giving group
   ids 3..8; -1 for a kind outside 1..20. `profile` (`SaveGetProfile` at the call site) is
   unused. */
s32 BtlGetAttrSoundGroupId(void *profile, s32 bakugan)
{
    (void)profile;
    if (bakugan > 0 && bakugan < 21) {
        return g_btlAttrSoundGroupTable[BtlGetKindAttribute(bakugan)] + 3;
    }
    return -1;
}
