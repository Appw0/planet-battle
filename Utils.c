#include "project.h"

struct timespec requested, remaining;

void sleepMs(int millis) {
	double fractional, integral;
	fractional = modf(millis/1000.0d, &integral);
    requested.tv_sec = integral;
    requested.tv_nsec = fractional * 1000000000;
	//printf("\n%ld:%ld-", requested.tv_sec, requested.tv_nsec);
    
    nanosleep(&requested, &remaining);
}

int getActorsAt(int x, int y, int type) {
  int i;
  for (i=0; i<actorCount; i++) { 
    if ((actors[i][actorID]>0) && ((actors[i][actorX]==x) && (actors[i][actorY]==y))) {
      switch (type) {
        case 1:
          return actors[i][actorID];
          break;
        default:
          return i;
          break;
      }
    }
  }
  switch (type) {
    case 1:
      return 0;
      break;
    default:
      return -1;
      break;
  }
}
