#include "project.h"
                                             
void spawnEnemies(int totalPoints, int maxCount) {
  int xguess, yguess, chooseType, i, valid;
  int spawnpointTotal=totalPoints;
  
  for(i=1; i<maxCount+1; i++) {
    do {
      xguess=rand()%(mapw)+1;
      yguess=rand()%(maph)+1;
      valid = map[yguess][xguess] > Barrier ? 1 : 0;
    } while(valid != 1);  
    
    chooseType = rand()%(actorTypeCount-2)+2;
    spawnpointTotal -= actorTypes[chooseType][actorTypeSpawnPoints];
    
    actors[i][actorTypeID]=chooseType;
    actors[i][actorX]=xguess;
    actors[i][actorY]=yguess;
    actors[i][actorMoveCool]=actorTypes[chooseType][actorTypeMoveCool];
    actors[i][actorHealth]=actorTypes[chooseType][actorTypeHealth];
    
    if (spawnpointTotal<=0) break;
     
  }
}

