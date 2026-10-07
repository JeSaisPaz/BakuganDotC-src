// bdc 0x0882bb8c GameFieldMovieTaskCtor
#include "bdc.h"

/* Constructor of the field movie task (task id 106 = 0x6a, 0x38 bytes, vtable `0x08af178c`, created
   by `CoreTaskNewByIdArg`): a story movie (`GameStoryMovieCtor`) subclass that plays one movie
   `movie` (`+0x1c`) in the middle of field/battle play; clears the step `+0x14`. */

CoreTask * GameFieldMovieTaskCtor(CoreTask *task, u32 movie)

{
  GameStoryMovie *self = (GameStoryMovie *)task;

  GameStoryMovieCtor(self,0);
  task->vtable = g_gameFieldMovieTaskVtbl;
  self->movie = movie;
  self->step = 0;
  return task;
}
