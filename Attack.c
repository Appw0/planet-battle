#include "project.h"

// this will be the most barebones of code

int Attack(int dir, int xpos, int ypos, int actorNum) {
  int targetActorNum;
  targetActorNum=-1;
  
  if (dir==1){
      targetActorNum=getActorsAt(xpos,ypos-1,0); 
  }
  if (dir==2){
      targetActorNum=getActorsAt(xpos-1,ypos,0); 
  }
  if (dir==3){
      targetActorNum=getActorsAt(xpos,ypos+1,0); 
  }
  if (dir==4){
      targetActorNum=getActorsAt(xpos+1,ypos,0); 
  }

  printf("%d",targetActorNum);
  if (targetActorNum>=0) {
    actors[targetActorNum][5]=actors[targetActorNum][5]-getInventoryProperty(actorNum, 0, 1);
    if (actors[targetActorNum][5]<1) {
      actors[targetActorNum][actorID]=0;
    }
    return 1;
  }
  return 0;
}
