// bdc 0x08a00e90 ScriptOpSound
#include "bdc.h"

/* Script opcode 0x44 (group 0x40): sound/SE command. Operands u32 `cmd`, s32 `group`, u32 `sound`,
   u32 `loop`, float x/y/z. cmd 1 plays a sound at (x,y,z) on the script's sound object
   (`script->soundObj`): `SndObjectSetPos`, then `SndObjectAddEmitter` (group < 0) or
   `SndObjectPlayGroupCue` (group >= 0); cmd 2 loads sound group `group` (`SndManagerLoadGroup`,
   returns 2 while it fails); cmd 3 waits until all groups finished loading
   (`SndManagerQueryGroupLoad` with -1); cmd 4 unloads the group (`SndManagerUnloadGroup`; on
   failure `SndManagerFadeOutAllVoices` and returns 2); cmd 5 sets the 3D listener position from
   (x,y,z) via `SndListenerSet`; cmd 6 returns 2 while the sound is playing
   (`SndManagerIsSoundPlaying``(mgr, group, sound)`, or `SndManagerIsSoundWordPlaying``(mgr,
   sound)` for group < 0); cmd 0 and others stop the sound (`SndObjectStopGroupCue` /
   `SndObjectStopSound`). Returns 0 to continue, 2 to retry next frame; cmds 2..4 also return 2
   without a sound manager. */

int ScriptOpSound(Script *script)
{
  u32 cmd;
  s32 group;
  u32 sound;
  u32 loopArg;
  u8 loop;
  float pos[3];

  cmd = ScriptReadU32(script);
  group = (s32)ScriptReadU32(script);
  sound = ScriptReadU32(script);
  loopArg = ScriptReadU32(script);
  pos[0] = ScriptReadFloat(script);
  pos[1] = ScriptReadFloat(script);
  pos[2] = ScriptReadFloat(script);

  switch (cmd) {
  case 1:
    SndObjectSetPos(script->soundObj, pos[0], pos[1], pos[2]);
    loop = loopArg != 0;
    if (group < 0) {
      SndObjectAddEmitter(script->soundObj, (s32)sound, loop, 0);
    } else {
      SndObjectPlayGroupCue(script->soundObj, group, sound, loop);
    }
    return 0;
  case 2:
    if (!SndHasManager()) {
      return 2;
    }
    if (!SndManagerLoadGroup(SndGetManager(), group)) {
      return 2;
    }
    return 0;
  case 3:
    if (!SndHasManager()) {
      return 2;
    }
    if (!SndManagerQueryGroupLoad(SndGetManager(), -1)) {
      return 2;
    }
    return 0;
  case 4:
    if (!SndHasManager()) {
      return 2;
    }
    if (SndManagerUnloadGroup(SndGetManager(), group)) {
      return 0;
    }
    SndManagerFadeOutAllVoices(SndGetManager());
    return 2;
  case 5:
    if (SndHasListener()) {
      SndListenerSet(0.0f, SndGetListener(), pos, NULL);
    }
    return 0;
  case 6:
    if (!SndHasManager()) {
      return 0;
    }
    if (group < 0) {
      if (SndManagerIsSoundWordPlaying(SndGetManager(), sound) != 0) {
        return 2;
      }
      return 0;
    }
    if (SndManagerIsSoundPlaying(SndGetManager(), group, sound) != 0) {
      return 2;
    }
    return 0;
  default:
    if (group < 0) {
      SndObjectStopSound(script->soundObj, (s32)sound);
    } else {
      SndObjectStopGroupCue(script->soundObj, group, sound);
    }
    return 0;
  }
}
