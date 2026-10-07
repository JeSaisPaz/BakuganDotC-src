// bdc 0x08a12a2c GmoImagePlanTakePalettesThunk
#include "bdc.h"

/* Thunk (`j` + `nop`) to `GmoImagePlanTakePalettes`: carves `n` 0x30-byte palette records from an
   image plan. Called by the palette/picture build passes (`GmoPaletteArrayCopy`,
   `GmoGimBuildPicture`, `GmoTim2Build`, `GmoTgaBuild`, `GmoBmpBuild`). */
void *GmoImagePlanTakePalettesThunk(int n, void *plan)
{
    /* `j 0x08a129ec`: Ghidra inlined the target's body; the binary only tail-jumps to it. */
    return GmoImagePlanTakePalettes(n, plan);
}
