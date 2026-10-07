// bdc 0x0882bbd8 GameFieldMovieTaskDtor
#include "bdc.h"

/* Destructor of the field movie task (`GameFieldMovieTaskCtor`) (vtable `0x08af178c` slot 1):
   restores the vtable, runs `GameStoryMovieDtor` (which applies the movie's story consequences)
   and frees the object when `flags & 1`. */

void GameFieldMovieTaskDtor(CoreTask *task, u32 flags)

{
  if (task != (CoreTask *)0x0) {
    task->vtable = g_gameFieldMovieTaskVtbl;
    GameStoryMovieDtor((GameStoryMovie *)task,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(task,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

