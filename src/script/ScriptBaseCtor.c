// bdc 0x089c9104 ScriptBaseCtor
#include "bdc.h"

/* Constructor of the script base class: runs the generic list-node constructor
   `CoreNodeCtor``(script, anchor)` (`anchor` is the caller's, passed through untouched: the node
   the new script is linked behind, NULL for none), installs the base vtable `g_scriptBaseVtbl`
   (`0x08af52a4`) in `vtable`, clears `entry`, `tracks`, `vars`, `flagBits`, `trackLocals` and
   `parentState`, stores `SndObjectCreate(SndGetObjectList(), 4)` in `soundObj` and clears
   `paused`. Returns `script`. */

Script *ScriptBaseCtor(Script *script, CoreNode *anchor)
{
  CoreNodeCtor((CoreNode *)script, anchor);
  script->vtable = g_scriptBaseVtbl;
  script->entry = NULL;
  script->tracks = NULL;
  script->vars = NULL;
  script->flagBits = NULL;
  script->trackLocals = NULL;
  script->parentState = NULL;
  script->soundObj = SndObjectCreate(SndGetObjectList(), 4);
  script->paused = 0;
  return script;
}
