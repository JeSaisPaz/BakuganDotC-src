// bdc 0x089bf108 GfxVectorConstantsStaticInit
#include "bdc.h"

/* Static constructor of the engine's global vec4 constants: `g_vecW1` = {0,0,0,1}, `g_gfxVecZero`
   = zero, `g_vecX` = +X, `g_vecNegX` = -X, `g_vecUp` = +Y, `g_vecDown` = -Y,
   `g_gfxSpriteSubList` = +Z, `g_vecNegZ` = -Z and `g_vecOneNegNeg` = {1,-1,-1,0}. */

void GfxVectorConstantsStaticInit(void)

{
  g_vecW1.x = 0.0f;
  g_vecW1.y = 0.0f;
  g_vecW1.z = 0.0f;
  g_vecW1.w = 1.0f;
  g_gfxVecZero.x = 0.0f;
  g_gfxVecZero.y = 0.0f;
  g_gfxVecZero.z = 0.0f;
  g_gfxVecZero.w = 0.0f;
  g_vecX.x = 1.0f;
  g_vecX.y = 0.0f;
  g_vecX.z = 0.0f;
  g_vecX.w = 0.0f;
  g_vecNegX.x = -1.0f;
  g_vecNegX.y = 0.0f;
  g_vecNegX.z = 0.0f;
  g_vecNegX.w = 0.0f;
  g_vecUp.x = 0.0f;
  g_vecUp.y = 1.0f;
  g_vecUp.z = 0.0f;
  g_vecUp.w = 0.0f;
  g_vecDown.x = 0.0f;
  g_vecDown.y = -1.0f;
  g_vecDown.z = 0.0f;
  g_vecDown.w = 0.0f;
  g_gfxSpriteSubList.x = 0.0f;
  g_gfxSpriteSubList.y = 0.0f;
  g_gfxSpriteSubList.z = 1.0f;
  g_gfxSpriteSubList.w = 0.0f;
  g_vecNegZ.x = 0.0f;
  g_vecNegZ.y = 0.0f;
  g_vecNegZ.z = -1.0f;
  g_vecNegZ.w = 0.0f;
  g_vecOneNegNeg.x = 1.0f;
  g_vecOneNegNeg.y = -1.0f;
  g_vecOneNegNeg.z = -1.0f;
  g_vecOneNegNeg.w = 0.0f;
}
