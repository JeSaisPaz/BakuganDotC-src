// bdc 0x088b38bc StopWallSpawnEffects
#include "bdc.h"

/* Spawns the two effects of a stop wall into `wall + 0x20` and `wall + 0x24`
   (`StopWallSpawnEffect` twice, both with `width = A.x - B.x`, `height = A.y - B.y` from the
   corners `cornerA`/`cornerB`). */

void StopWallSpawnEffects(float unused, StopWall *self, float *cornerA, float *cornerB, char *texName)
{
  float pos[4];
  int i;

  pos[0] = (cornerA[0] + cornerB[0]) * 0.5f;
  pos[1] = cornerB[1];
  pos[2] = cornerB[2];
  pos[3] = 0.0f;
  for (i = 0; i < 2; i++) {
    self->effect[i] = StopWallSpawnEffect(cornerA[0] - cornerB[0], cornerA[1] - cornerB[1], unused, self, pos, texName);
  }
}
