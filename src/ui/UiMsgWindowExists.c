// bdc 0x08816a50 UiMsgWindowExists
#include "bdc.h"

/* Returns whether the message window singleton `g_uiMsgWindow` exists. */
bool UiMsgWindowExists(void)
{
    return g_uiMsgWindow != NULL;
}
