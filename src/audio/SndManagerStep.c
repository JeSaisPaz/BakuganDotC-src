// bdc 0x089c6cd4 SndManagerStep
#include "bdc.h"

/* One iteration of the sound manager's state machine (the body of the sound thread's loop): runs
   the handler for the current `state` (`SndManager +8`) and then sleeps. States: 1
   `SndManagerStateLoadModules`, 2 `SndManagerStateStartModules`, 3
   `SndManagerStateInitAudio`, 4 `SndManagerStateCreateDecOut`, 5 `SndManagerStateRun` (steady
   state, commands are accepted only here), 6 `SndManagerStateStopModules`, 7
   `SndManagerStateUnloadModules`; 0 does nothing. Each handler advances `state` by itself. After
   the handler it waits one vertical blank (`sceDisplayWaitVblankStartCB`) while the state is 5
   (running), otherwise it sleeps 100 us (`sceKernelDelayThreadCB(100)`). */

void SndManagerStep(SndManager *mgr)

{
  int state;

  state = mgr->state;
  switch(state) {
  case 1:
    SndManagerStateLoadModules(mgr);
    state = mgr->state;
    break;
  case 2:
    SndManagerStateStartModules(mgr);
    state = mgr->state;
    break;
  case 3:
    SndManagerStateInitAudio(mgr);
    state = mgr->state;
    break;
  case 4:
    SndManagerStateCreateDecOut(mgr);
    state = mgr->state;
    break;
  case 5:
    SndManagerStateRun(mgr);
    state = mgr->state;
    break;
  case 6:
    SndManagerStateStopModules(mgr);
    state = mgr->state;
    break;
  case 7:
    SndManagerStateUnloadModules(mgr);
    state = mgr->state;
  }
  if (state == 5) {
    sceDisplayWaitVblankStartCB();
  }
  else {
    sceKernelDelayThreadCB(100);
  }
  return;
}

