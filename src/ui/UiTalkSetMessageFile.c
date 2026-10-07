// bdc 0x0882c3e8 UiTalkSetMessageFile
#include "bdc.h"

/* Builds `name` + `"_eu.bin"` in a 64-byte stack buffer and, when `slot` is 1, copies it to
   `win+0x824` (the message-file name of the 0x6e overlay task). */

void UiTalkSetMessageFile(BtlHud *win, s32 slot, const char *name)

{
  char buf [64];
  
  strcpy(buf,name);
  strcat(buf,"_eu.bin");
  if ((slot < 2) && (0 < slot)) {
    strcpy(win->nameBuf,buf);
  }
  return;
}

