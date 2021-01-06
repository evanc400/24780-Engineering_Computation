#include <stdio.h>
#include <stdlib.h>
#include "fssimplewindow.h"

class Block
{
public:
  int state;
  int x,y;
  int w,h;
};

void DrawRacket(int &racketX,int &racketY, int racketSizeX, int racketSizeY);
void DrawBall(double x, double y);
void MoveBall(int &x,int &y,int vx,int vy);
void Bounce(int ballX,int ballY,int &vx,int &vy,int racketX, int blockY,int blockSizeX,int blockSizeY);
bool CheckBlockCollision(int ballX,int ballY,int blockX,int blockY,int blockSizeX,int blockSizeY);
void DrawBlock(int blockX,int blockY, int blockSizeX, int blockSizeY, int state);

int main(){

  FsOpenWindow(16,16,800,600,1);
  int terminate=0;
  int ballX=100;
  int ballY=200;
  int ballVX=8;
  int ballVY=8;
  int racketX;
  int racketY;
  int racketSizeX=50;
  int racketSizeY=10;
  int lb,mb,rb,mx,my;
  int nHit=0;

  const int nBlocksRow=8;
  const int nBlocksCol=2;
  int nBlocks=nBlocksRow*nBlocksCol;
  Block blocks[nBlocksRow][nBlocksCol];

  for(int i=0; i<nBlocksRow; i++){
    for(int j=0; j<nBlocksCol; j++){
      blocks[i][j].state=1;
      blocks[i][j].x=50+100*i;
      blocks[i][j].y=40+20*j;
      blocks[i][j].w=50;
      blocks[i][j].h=10;
    }
  }

  while(0==terminate){

    FsGetMouseEvent(lb,mb,rb,mx,my);

    racketX=mx;
    racketY=my;//set racket,x,y as center


    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    if(ballY>600){
      printf("Miss! \n");
      terminate=1;
    }

    if(nHit==nBlocks){
      printf("You win! \n");
      terminate=1;
    }

    DrawRacket(racketX,racketY,racketSizeX,racketSizeY);
    DrawBall(ballX, ballY);
    Bounce(ballX, ballY,ballVX,ballVY,racketX,racketY,racketSizeX,racketSizeY);
    MoveBall(ballX, ballY,ballVX,ballVY);

    for(int i=0; i<nBlocksRow; i++){
      for(int j=0; j<nBlocksCol; j++){
        DrawBlock(blocks[i][j].x,blocks[i][j].y,blocks[i][j].w,blocks[i][j].h, blocks[i][j].state);
      }
    }

    for(int i=0; i<nBlocksRow; i++){
      for(int j=0; j<nBlocksCol; j++){
        if(blocks[i][j].state!=0){
          if(CheckBlockCollision(ballX,ballY,blocks[i][j].x,blocks[i][j].y,blocks[i][j].w,blocks[i][j].h)){
            blocks[i][j].state=0;
            ballVY=-ballVY;
            nHit++;
        }
        }
      }
    }

    FsSwapBuffers();
    FsSleep(25);
  }

  return 0;
}

void DrawRacket(int &racketX,int &racketY, int racketSizeX, int racketSizeY)
{
  //RacketX, RacketY defined in center of racket

  if(racketY<490) racketY=490;
  if(racketY>590) racketY=590;

  glColor3ub(0,0,0);
  glBegin(GL_QUADS);
  glVertex2i(racketX-racketSizeX,racketY+racketSizeY);
  glVertex2i(racketX-racketSizeX,racketY-racketSizeY);
  glVertex2i(racketX+racketSizeX,racketY-racketSizeY);
  glVertex2i(racketX+racketSizeX,racketY+racketSizeY);
  glEnd();
}

void DrawBall(double x, double y){

  glColor3ub(0,0,0);
  glBegin(GL_QUADS);
  glVertex2i(x-5,y+5);
  glVertex2i(x-5,y-5);
  glVertex2i(x+5,y-5);
  glVertex2i(x+5,y+5);
  glEnd();

}

void MoveBall(int &x,int &y,int vx,int vy)
{
  x=x+vx;
  y=y+vy;
}

void Bounce(int ballX,int ballY,int &vx,int &vy,int racketX, int racketY,int racketSizeX,int racketSizeY)
{
    bool hit=CheckBlockCollision(ballX,ballY,racketX,racketY,racketSizeX,racketSizeY);

    if(ballY<5 || hit)
    {
        vy=-vy;
    }

    if(ballX<5 || ballX>795)
    {
        vx=-vx;
    }
}

bool CheckBlockCollision(int ballX,int ballY,int blockX,int blockY,int blockSizeX,int blockSizeY){

    int blockLeftEdge=blockX-blockSizeX;
    int blockRightEdge=blockX+blockSizeX;
    int blockTopEdge=blockY-blockSizeY;
    int blockBottomEdge=blockY+blockSizeY;
    int ballLeftEdge=ballX-5;
    int ballRightEdge=ballX+5;
    int ballBottomEdge=ballY+5;

    int relativeRight=blockRightEdge-ballLeftEdge;
    int relativeLeft=ballRightEdge-blockLeftEdge;

    if( relativeRight>0 && relativeLeft>0 && ballBottomEdge >= blockTopEdge && ballBottomEdge <= blockBottomEdge)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void DrawBlock(int blockX,int blockY, int blockSizeX, int blockSizeY, int state)
{
  if(state==0) return;

  glColor3ub(0,0,255);
  glBegin(GL_QUADS);
  glVertex2i(blockX-blockSizeX,blockY+blockSizeY);
  glVertex2i(blockX-blockSizeX,blockY-blockSizeY);
  glVertex2i(blockX+blockSizeX,blockY-blockSizeY);
  glVertex2i(blockX+blockSizeX,blockY+blockSizeY);
  glEnd();
}
