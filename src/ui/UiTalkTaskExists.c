// bdc 0x0882c0fc UiTalkTaskExists
#include "bdc.h"

/* `CoreTaskExists(0x6e)`: tests whether the talk/message window task (task id 0x6e) is alive; guard
   of the script opcodes that show, close or configure the message window. */

int UiTalkTaskExists(void)

{
  return CoreTaskExists(0x6e);
}

