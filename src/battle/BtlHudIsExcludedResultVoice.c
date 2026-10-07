// bdc 0x0883a86c BtlHudIsExcludedResultVoice
#include "bdc.h"

/* Returns 1 for the five voice ids 0x2a41, 0x2a44, 0x2a47, 0x2a68, 0x2a6b that
   `BtlHudPickResultVoice` must not choose. */
int BtlHudIsExcludedResultVoice(BtlHud *self, int voiceId)
{
    (void)self;
    switch (voiceId) {
    case 0x2a41:
    case 0x2a44:
    case 0x2a47:
    case 0x2a68:
    case 0x2a6b:
        return 1;
    default:
        return 0;
    }
}
