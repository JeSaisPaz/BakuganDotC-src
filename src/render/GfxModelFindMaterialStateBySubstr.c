// bdc 0x089df7bc GfxModelFindMaterialStateBySubstr
#include "bdc.h"

/* Returns the state record of the first material whose name contains `substr`
   (`GfxModelFindMaterialIndexBySubstr`, `GfxModelGetMaterialState`), or NULL. */

void *GfxModelFindMaterialStateBySubstr(GfxModel *self, const char *substr)

{
  s32 index;
  
  index = GfxModelFindMaterialIndexBySubstr(self,substr);
  if (index != -1) {
    return GfxModelGetMaterialState(self,index);
  }
  return (void *)0x0;
}

