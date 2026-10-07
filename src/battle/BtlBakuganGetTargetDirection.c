// bdc 0x0886298c BtlBakuganGetTargetDirection
#include "bdc.h"

/* `BtlBakuganClassifyTargetDirection` with the heading stored in the unit's control object
   (`input->heading`, see `BtlBakuganSetHeading`): 1 ahead (or no target), 0 behind, 3 or 2 for
   the negative / positive side. */
int BtlBakuganGetTargetDirection(BtlBakugan *bakugan)
{
    return BtlBakuganClassifyTargetDirection(bakugan->input->heading, bakugan);
}
