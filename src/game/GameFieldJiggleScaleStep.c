// bdc 0x088c7564 GameFieldJiggleScaleStep
#include "bdc.h"

/* One step of a decaying scale wobble: increments `*frame`, halves `*amplitude` and writes `scale *
   (1 − cos(frame·0.73)·amplitude)` (the halved amplitude) into the diagonal of `matrix` (elements
   0, 5, 10), the factor into `*outFactor`, and returns 0; once the amplitude is <= 0.006 writes the
   plain scale, sets the factor to 1 and returns 1. */

s32 GameFieldJiggleScaleStep(float *matrix, float *scale, float *amplitude, s32 *frame, float *outFactor)
{
  float angle;
  float factor;

  *frame = *frame + 1;
  if (!(*amplitude <= 0.006f)) {
    *amplitude = *amplitude * 0.5f;
    angle = (float)*frame * 0.73f;
    /* vcos of angle·(2/π) quarter turns (bank S703) is cos(angle) */
    factor = 1.0f - __builtin_cosf(angle) * *amplitude;
    *outFactor = factor;
    matrix[0] = scale[0] * factor;
    matrix[5] = scale[1] * factor;
    matrix[10] = scale[2] * factor;
    return 0;
  }
  matrix[0] = scale[0];
  matrix[5] = scale[1];
  matrix[10] = scale[2];
  *outFactor = 1.0f;
  return 1;
}
