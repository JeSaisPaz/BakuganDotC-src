// bdc 0x08a20538 sceSsSetEffectParam
#include "bdc.h"

/* Applies a block of sound-effect parameters to the Sony sound layer. `param[0]` is a bit mask of
   the groups present: bit 0 applies `param[1]` (`SndSasRevType`), bit 1 applies `{param[2],
   param[3]}` (`SndSasRevParam`), bit 2 `{param[4], param[5]}` (`SndSasRevEVOL`) and bit 3
   `{param[6], param[7]}` (`SndSasRevVON`); the first non-zero result is returned immediately, 0
   means success. Returns `0x80450001` when the layer is not initialised (`g_sndSsState == -1`). */

s32 sceSsSetEffectParam(u32 *param)
{
  s32 ret;

  ret = 0x80450001;
  if (g_sndSsState != -1) {
    if ((*param & 1) != 0) {
      ret = SndSasRevType(param[1]);
      if (ret != 0) {
        return ret;
      }
    }
    if ((*param & 2) != 0) {
      ret = SndSasRevParam(param[2], param[3]);
      if (ret != 0) {
        return ret;
      }
    }
    if ((*param & 4) != 0) {
      ret = SndSasRevEVOL(param[4], param[5]);
      if (ret != 0) {
        return ret;
      }
    }
    if ((*param & 8) == 0) {
      ret = 0;
    } else {
      ret = SndSasRevVON(param[6], param[7]);
    }
  }
  return ret;
}
