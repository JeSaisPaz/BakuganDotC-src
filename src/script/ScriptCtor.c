// bdc 0x089ffdd4 ScriptCtor
#include "bdc.h"

/* Constructor of the concrete script class: runs `ScriptBaseCtor``(script, anchor)` (`anchor`,
   the list node `CoreNodeCtor` links the new script behind, is forwarded untouched) and then
   installs the derived vtable `0x08af59fc` at `+0x20`. Returns `script`. */

Script *ScriptCtor(Script *script, CoreNode *anchor)

{
  ScriptBaseCtor(script, anchor);
  script->vtable = g_scriptVtbl;
  return script;
}
