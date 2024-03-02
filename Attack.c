#include "project.h"

// this will be the most barebones of code

int Attack(int dir, int xpos, int ypos, int actorID) {
  int targetActorID;
  targetActorID=-1;
  
  if (dir==1){
      targetActorID=getActorsAt(xpos,ypos-1,0); 
  }
  if (dir==2){
      targetActorID=getActorsAt(xpos-1,ypos,0); 
  }
  if (dir==3){
      targetActorID=getActorsAt(xpos,ypos+1,0); 
  }
  if (dir==4){
      targetActorID=getActorsAt(xpos+1,ypos,0); 
  }
  
  // so monsters dont kill each other
  if ((actors[targetActorID][actorTypeID] > 1)&&(actors[actorID][actorTypeID] > 1)) {
    return 0;
  }
  
  printf("%d",targetActorID);
  if (targetActorID>=0) {
    actors[targetActorID][actorHealth]=actors[targetActorID][actorHealth]-getInventoryProperty(actorID, 0, 2);
    if (actors[targetActorID][actorHealth]<1) {
      actors[targetActorID][actorTypeID]=0;
    }
    return 1;
  }
  return 0;
}
