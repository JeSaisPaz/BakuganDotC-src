// bdc 0x0888c1e0 BtlGetKindAttribute
#include "bdc.h"

/* Returns the default attribute of Bakugan kind `kind`: the kind is clamped to 0..32 through a
   float (negative -> 0, above 32 or NaN -> 32), and the result is the signed first byte of the
   kind's stat record `g_btlKindStatTables[kind]`, or 0 when that record pointer is NULL. Used by
   `BtlGetAttrSoundGroupId`. */
s32 BtlGetKindAttribute(s32 kind)
{
    float k = (float)kind;
    s32 index;

    if (k < 0.0f) {
        index = 0;
    } else if (k <= 32.0f) {
        index = (s32)k;
    } else {
        index = 32;
    }
    if (g_btlKindStatTables[index] != NULL) {
        return g_btlKindStatTables[index][0];
    }
    return 0;
}
