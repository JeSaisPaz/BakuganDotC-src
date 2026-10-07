// bdc 0x089ce6a4 PadGetLanguage
#include "bdc.h"

/* Returns the requested display language stored in the pad state (`pad+0x50`, PSP language id;
   written by PadSetLanguage and applied to the system by PadApplyLanguageMode). */
s32 PadGetLanguage(PadState *pad)
{
    return pad->id50;
}
