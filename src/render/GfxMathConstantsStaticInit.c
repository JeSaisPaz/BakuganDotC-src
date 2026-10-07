// bdc 0x089f385c GfxMathConstantsStaticInit
#include "bdc.h"

/* Static initialiser (entry 46 of the static-constructor table `0x08af5bbc`, run by
   `CxxRunStaticCtors`) of the engine's global float vector/matrix constants: `g_quatIdentity`
   `(0,0,0,1)`, `g_quatNegY` `(0,-1,0,0)`, `g_quatRot90X` `(0.7071,0,0,0.7071)`,
   `g_gfxIdentityMatrix`, `diag(1,-1,-1,1)` in both `g_gfxFlipYZMatrixB` and `g_gfxFlipYZMatrix`,
   the Y/Z axis swap `g_gfxSwapYZMatrix` and 90-degree rotations about Y (`g_gfxRotY90Matrix`)
   and Z (`g_gfxRotZ90Matrix`). Returns nothing. */

void GfxMathConstantsStaticInit(void)

{
  g_quatIdentity.x = 0.0f;
  g_quatIdentity.y = 0.0f;
  g_quatIdentity.z = 0.0f;
  g_quatIdentity.w = 1.0f;
  g_quatNegY.x = 0.0f;
  g_quatNegY.y = -1.0f;
  g_quatNegY.z = 0.0f;
  g_quatNegY.w = 0.0f;
  g_quatRot90X.x = 0x1.6a09eep-1f; /* 0x3f3504f7, ~0.707107 */
  g_quatRot90X.y = 0.0f;
  g_quatRot90X.z = 0.0f;
  g_quatRot90X.w = 0x1.6a09eep-1f; /* 0x3f3504f7, ~0.707107 */
  g_gfxIdentityMatrix.x.x = 1.0f;
  g_gfxIdentityMatrix.x.y = 0.0f;
  g_gfxIdentityMatrix.x.z = 0.0f;
  g_gfxIdentityMatrix.x.w = 0.0f;
  g_gfxIdentityMatrix.y.x = 0.0f;
  g_gfxIdentityMatrix.y.y = 1.0f;
  g_gfxIdentityMatrix.y.z = 0.0f;
  g_gfxIdentityMatrix.y.w = 0.0f;
  g_gfxIdentityMatrix.z.x = 0.0f;
  g_gfxIdentityMatrix.z.y = 0.0f;
  g_gfxIdentityMatrix.z.z = 1.0f;
  g_gfxIdentityMatrix.z.w = 0.0f;
  g_gfxIdentityMatrix.w.x = 0.0f;
  g_gfxIdentityMatrix.w.y = 0.0f;
  g_gfxIdentityMatrix.w.z = 0.0f;
  g_gfxIdentityMatrix.w.w = 1.0f;
  g_gfxFlipYZMatrixB.x.x = 1.0f;
  g_gfxFlipYZMatrixB.x.y = 0.0f;
  g_gfxFlipYZMatrixB.x.z = 0.0f;
  g_gfxFlipYZMatrixB.x.w = 0.0f;
  g_gfxFlipYZMatrixB.y.x = 0.0f;
  g_gfxFlipYZMatrixB.y.y = -1.0f;
  g_gfxFlipYZMatrixB.y.z = 0.0f;
  g_gfxFlipYZMatrixB.y.w = 0.0f;
  g_gfxFlipYZMatrixB.z.x = 0.0f;
  g_gfxFlipYZMatrixB.z.y = 0.0f;
  g_gfxFlipYZMatrixB.z.z = -1.0f;
  g_gfxFlipYZMatrixB.z.w = 0.0f;
  g_gfxFlipYZMatrixB.w.x = 0.0f;
  g_gfxFlipYZMatrixB.w.y = 0.0f;
  g_gfxFlipYZMatrixB.w.z = 0.0f;
  g_gfxFlipYZMatrixB.w.w = 1.0f;
  g_gfxSwapYZMatrix.x.x = 1.0f;
  g_gfxSwapYZMatrix.x.y = 0.0f;
  g_gfxSwapYZMatrix.x.z = 0.0f;
  g_gfxSwapYZMatrix.x.w = 0.0f;
  g_gfxSwapYZMatrix.y.x = 0.0f;
  g_gfxSwapYZMatrix.y.y = 0.0f;
  g_gfxSwapYZMatrix.y.z = 1.0f;
  g_gfxSwapYZMatrix.y.w = 0.0f;
  g_gfxSwapYZMatrix.z.x = 0.0f;
  g_gfxSwapYZMatrix.z.y = 1.0f;
  g_gfxSwapYZMatrix.z.z = 0.0f;
  g_gfxSwapYZMatrix.z.w = 0.0f;
  g_gfxSwapYZMatrix.w.x = 0.0f;
  g_gfxSwapYZMatrix.w.y = 0.0f;
  g_gfxSwapYZMatrix.w.z = 0.0f;
  g_gfxSwapYZMatrix.w.w = 1.0f;
  g_gfxFlipYZMatrix.x.x = 1.0f;
  g_gfxFlipYZMatrix.x.y = 0.0f;
  g_gfxFlipYZMatrix.x.z = 0.0f;
  g_gfxFlipYZMatrix.x.w = 0.0f;
  g_gfxFlipYZMatrix.y.x = 0.0f;
  g_gfxFlipYZMatrix.y.y = -1.0f;
  g_gfxFlipYZMatrix.y.z = 0.0f;
  g_gfxFlipYZMatrix.y.w = 0.0f;
  g_gfxFlipYZMatrix.z.x = 0.0f;
  g_gfxFlipYZMatrix.z.y = 0.0f;
  g_gfxFlipYZMatrix.z.z = -1.0f;
  g_gfxFlipYZMatrix.z.w = 0.0f;
  g_gfxFlipYZMatrix.w.x = 0.0f;
  g_gfxFlipYZMatrix.w.y = 0.0f;
  g_gfxFlipYZMatrix.w.z = 0.0f;
  g_gfxFlipYZMatrix.w.w = 1.0f;
  g_gfxRotY90Matrix.x.x = 0.0f;
  g_gfxRotY90Matrix.x.y = 0.0f;
  g_gfxRotY90Matrix.x.z = -1.0f;
  g_gfxRotY90Matrix.x.w = 0.0f;
  g_gfxRotY90Matrix.y.x = 0.0f;
  g_gfxRotY90Matrix.y.y = 1.0f;
  g_gfxRotY90Matrix.y.z = 0.0f;
  g_gfxRotY90Matrix.y.w = 0.0f;
  g_gfxRotY90Matrix.z.x = 1.0f;
  g_gfxRotY90Matrix.z.y = 0.0f;
  g_gfxRotY90Matrix.z.z = 0.0f;
  g_gfxRotY90Matrix.z.w = 0.0f;
  g_gfxRotY90Matrix.w.x = 0.0f;
  g_gfxRotY90Matrix.w.y = 0.0f;
  g_gfxRotY90Matrix.w.z = 0.0f;
  g_gfxRotY90Matrix.w.w = 1.0f;
  g_gfxRotZ90Matrix.x.x = 0.0f;
  g_gfxRotZ90Matrix.x.y = 1.0f;
  g_gfxRotZ90Matrix.x.z = 0.0f;
  g_gfxRotZ90Matrix.x.w = 0.0f;
  g_gfxRotZ90Matrix.y.x = -1.0f;
  g_gfxRotZ90Matrix.y.y = 0.0f;
  g_gfxRotZ90Matrix.y.z = 0.0f;
  g_gfxRotZ90Matrix.y.w = 0.0f;
  g_gfxRotZ90Matrix.z.x = 0.0f;
  g_gfxRotZ90Matrix.z.y = 0.0f;
  g_gfxRotZ90Matrix.z.z = 1.0f;
  g_gfxRotZ90Matrix.z.w = 0.0f;
  g_gfxRotZ90Matrix.w.x = 0.0f;
  g_gfxRotZ90Matrix.w.y = 0.0f;
  g_gfxRotZ90Matrix.w.z = 0.0f;
  g_gfxRotZ90Matrix.w.w = 1.0f;
  return;
}

