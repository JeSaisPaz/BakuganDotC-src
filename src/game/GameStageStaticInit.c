// bdc 0x088d4c7c GameStageStaticInit
#include "bdc.h"

/* Static initialiser (entry 19 of the static-constructor table `0x08af5bbc`) of the field stage
   module: zeroes the 45 vec4s of `g_gameStagePropPositions` and sets the 4x4 matrix
   `g_gameStageColliderCallbacks` to identity. */

void GameStageStaticInit(void)
{
  int i;

  for (i = 0; i < 45; i++) {
    g_gameStagePropPositions[i][0] = 0.0f;
    g_gameStagePropPositions[i][1] = 0.0f;
    g_gameStagePropPositions[i][2] = 0.0f;
    g_gameStagePropPositions[i][3] = 0.0f;
  }

  g_gameStageColliderCallbacks[0][0] = 1.0f;
  g_gameStageColliderCallbacks[0][1] = 0.0f;
  g_gameStageColliderCallbacks[0][2] = 0.0f;
  g_gameStageColliderCallbacks[0][3] = 0.0f;
  g_gameStageColliderCallbacks[1][0] = 0.0f;
  g_gameStageColliderCallbacks[1][1] = 1.0f;
  g_gameStageColliderCallbacks[1][2] = 0.0f;
  g_gameStageColliderCallbacks[1][3] = 0.0f;
  g_gameStageColliderCallbacks[2][0] = 0.0f;
  g_gameStageColliderCallbacks[2][1] = 0.0f;
  g_gameStageColliderCallbacks[2][2] = 1.0f;
  g_gameStageColliderCallbacks[2][3] = 0.0f;
  g_gameStageColliderCallbacks[3][0] = 0.0f;
  g_gameStageColliderCallbacks[3][1] = 0.0f;
  g_gameStageColliderCallbacks[3][2] = 0.0f;
  g_gameStageColliderCallbacks[3][3] = 1.0f;
}
