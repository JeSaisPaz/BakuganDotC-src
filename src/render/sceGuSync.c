// bdc 0x08a1ee74 sceGuSync
#include "bdc.h"

/* libgu `sceGuSync(mode, what)`: `mode 0` waits for drawing to finish (`sceGeDrawSync(what)`),
   modes 3 and 4 wait for a specific list (`sceGeListSync` with the list id saved at `g_guListId` /
   `g_guSubListId`); any other mode returns `SCE_GE_LIST_COMPLETED`. */

s32 sceGuSync(s32 mode, s32 what)

{
  SceGeListState state;
  s32 dlId;
  
  switch(mode) {
  case 0:
    state = sceGeDrawSync(what);
    return state;
  default:
    return 0;
  case 3:
    dlId = g_guListId;
    break;
  case 4:
    dlId = g_guSubListId;
  }
  state = sceGeListSync(dlId,what);
  return state;
}

