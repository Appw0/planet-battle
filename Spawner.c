#include "project.h"

// spawnPoints 0
// icon 1
// health 2
// moveCool 3
// weaponID 4
int monsterProperty[monsterTypes][monsterPropertyCount] = {{0,0,0,0,0},  // this is dead, leave as such (for now)
                                                           {0,0,0,0,0},  // this is player, leave as such (for now)
                                                           {1,2,4,0,3},
                                                           {2,3,6,2,3}};
                                                           
                                                           
                                                           
                                                           
                                                           
                                                           
void spawnEnemies(int totalPoints, int maxCount) {
  int xguess, yguess, chooseType, i, valid;
  int spawnpointTotal=totalPoints;
  
  for(i=1; i<maxCount+1; i++) {
    do {
      xguess=rand()%(mapw)+1;
      yguess=rand()%(maph)+1;
      valid = map[yguess][xguess] > Barrier ? 1 : 0;
    } while(valid != 1);  
    
    chooseType = rand()%(monsterTypes-2)+2;
    spawnpointTotal -= monsterProperty[chooseType][spawnPoints];
    
    actors[i][actorTypeID]=chooseType;
    actors[i][actorX]=xguess;
    actors[i][actorY]=yguess;
    actors[i][actorMoveCool]=monsterProperty[chooseType][moveCool];
    actors[i][actorHealth]=monsterProperty[chooseType][health];
    
    if (spawnpointTotal<=0) break;
     
  }
}
