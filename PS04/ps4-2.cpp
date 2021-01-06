#include <stdio.h>
#include <stdlib.h>
#include "fssimplewindow.h"
#include <math.h>
#include <time.h>

void DrawCannon(double angle);
void MoveCannon(double& angle);
void DrawBall(double x, double y);
void MoveBall(double &x, double &y, double &vx, double &vy, double dt);
void DrawTarget(double x, double y, int targetSizeX,int targetSizeY);
void MoveTarget(double &y);
int CheckHitTarget(double missileX, double missileY, double targetX, double targetY,int targetSizeX,int targetSizeY);
void DrawObstacle(int x, int y, int targetSizeX,int targetSizeY,int fill);
void GenerateRandObstacle(int &Xc, int &Yc, int &randSizeX,int &randSizeY);
void DrawCircle(int cx,int cy,int rad,int fill);

int main(){

  FsOpenWindow(16,16,800,600,1);

  srand(time(NULL));

  const double DEG2RAD=3.141592/180.0;

  int terminate=0;
  double angle=30;
  int ball=0;
  int nShot=0;

  double ballX,ballY,ballVX,ballVY;
  double targetX=750;
  double targetY=0;
  double dt=0.025;
  int targetSizeX=50;
  int targetSizeY=50;

  int Xr1,Yr1,randSizeX1,randSizeY1;
  int Xr2,Yr2,randSizeX2,randSizeY2;
  int Xr3,Yr3,randSizeX3,randSizeY3;
  int Xr4,Yr4,randSizeX4,randSizeY4;
  int Xr5,Yr5,randSizeX5,randSizeY5;

  int obst1State=1;
  int obst2State=1;
  int obst3State=1;
  int obst4State=1;
  int obst5State=1;

  GenerateRandObstacle(Xr1,Yr1,randSizeX1,randSizeY1);
  GenerateRandObstacle(Xr2,Yr2,randSizeX2,randSizeY2);
  GenerateRandObstacle(Xr3,Yr3,randSizeX3,randSizeY3);
  GenerateRandObstacle(Xr4,Yr4,randSizeX4,randSizeY4);
  GenerateRandObstacle(Xr5,Yr5,randSizeX5,randSizeY5);

  while(0==terminate){

      FsPollDevice();
      int key=FsInkey();

      switch(key)
      {
      case FSKEY_ESC:
        terminate=1;
        break;
      case FSKEY_SPACE:
        if(0==ball){
          ball=1;
          nShot++;
          ballVX=400*cos(-angle*DEG2RAD);
          ballVY=400*sin(-angle*DEG2RAD);
          ballX=55+20*cos(-angle*DEG2RAD);
          ballY=545+20*sin(-angle*DEG2RAD);
        }
        break;
      }

      glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

      DrawCannon(angle);
      MoveCannon(angle);

      if(ball!=0){

        MoveBall(ballX, ballY, ballVX, ballVY, dt);
        if(ballX>800 || ballY>600) ball=0;
        else DrawBall(ballX,ballY);

      }

      DrawTarget(targetX,targetY,targetSizeX,targetSizeY);
      MoveTarget(targetY);

      if(CheckHitTarget(ballX,ballY, Xr1,Yr1,randSizeX1,randSizeY1)==1 && obst1State==1){
        obst1State=0;
        ball=0;
      }
      if(CheckHitTarget(ballX,ballY, Xr2,Yr2,randSizeX2,randSizeY2)==1 && obst2State==1){
        obst2State=0;
        ball=0;
      }
      if(CheckHitTarget(ballX,ballY, Xr3,Yr3,randSizeX3,randSizeY3)==1 && obst3State==1){
        obst3State=0;
        ball=0;
      }
      if(CheckHitTarget(ballX,ballY, Xr4,Yr4,randSizeX4,randSizeY4)==1 && obst4State==1){
        obst4State=0;
        ball=0;
      }
      if(CheckHitTarget(ballX,ballY, Xr5,Yr5,randSizeX5,randSizeY5)==1 && obst5State==1){
        obst5State=0;
        ball=0;
      }

      DrawObstacle(Xr1,Yr1,randSizeX1,randSizeY1,obst1State);
      DrawObstacle(Xr2,Yr2,randSizeX2,randSizeY2,obst2State);
      DrawObstacle(Xr3,Yr3,randSizeX3,randSizeY3,obst3State);
      DrawObstacle(Xr4,Yr4,randSizeX4,randSizeY4,obst4State);
      DrawObstacle(Xr5,Yr5,randSizeX5,randSizeY5,obst5State);

      DrawTarget(targetX,targetY,targetSizeX,targetSizeY);
      MoveTarget(targetY);

      if(CheckHitTarget(ballX,ballY, targetX, targetY, targetSizeX, targetSizeY)==1){
        printf("\n YOU DID IT!!!!");
        printf(" You shot %d cannonballs to hit the target.\n",nShot);
        terminate=1;
      }

      FsSwapBuffers();
      FsSleep(25);

  }

  return 0;
}

void DrawCannon(double angle){
  const double PI=3.141592;

 double angle_rad = angle*PI/180.0;
  //Square
  glColor3ub(0,0,255);
  glBegin(GL_QUADS);
  glVertex2i(50,550);
  glVertex2i(50,540);
  glVertex2i(60,540);
  glVertex2i(60,550);
  glEnd();

  //line
  glColor3ub(0,0,255);
  glBegin(GL_LINES);
  glVertex2i(55,545);
  glVertex2i(55+20*cos(-angle_rad),545+20*sin(-angle_rad));
  glEnd();
}

void MoveCannon(double& angle){
  angle++;
  if(angle>90) angle=0;
}

void MoveBall(double &x, double &y, double &vx, double &vy, double dt){

  double fx,fy,ax,ay;

  x=x+vx*dt;
  y=y+vy*dt;

  ax=0.0;
  ay=98.0;

  vx=vx+dt*ax;
  vy=vy+dt*ay;

}

void DrawBall(double x, double y){

  glColor3ub(255,0,0);
  DrawCircle( x, y, 5, 1);

}

void DrawTarget(double x, double y, int targetSizeX,int targetSizeY){

  glColor3ub(255,0,0);
  glBegin(GL_QUADS);
  glVertex2i(x,y+targetSizeY);
  glVertex2i(x,y);
  glVertex2i(x+targetSizeX,y);
  glVertex2i(x+targetSizeX,y+targetSizeY);
  glEnd();

}

void MoveTarget(double &y){

  y=y+2.5;
  if(y>550) y=0;
}

int CheckHitTarget(double missileX,double missileY,double targetX,double targetY,int targetSizeX,int targetSizeY){
    int relativeX,relativeY;
    relativeX=missileX-targetX;
    relativeY=missileY-targetY;
    if(0<=relativeX && relativeX<targetSizeX && 0<=relativeY && relativeY<targetSizeY)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void DrawObstacle(int x, int y, int targetSizeX,int targetSizeY,int fill){

  glColor3ub(0,255,0);

  if(0!=fill){
    glBegin(GL_QUADS);
  }
  else{
    glBegin(GL_LINE_LOOP);
  }

  glVertex2i(x,y+targetSizeY);
  glVertex2i(x,y);
  glVertex2i(x+targetSizeX,y);
  glVertex2i(x+targetSizeX,y+targetSizeY);
  glEnd();

}

void GenerateRandObstacle(int &Xr, int &Yr, int &randSizeX,int &randSizeY){

  Xr=40+rand()%721; //guarentees effective size will always be at least 800x800 pixels (max x center of 760);
  Yr=40+rand()%521; //same but with y (max y center of 560);

  randSizeX=80+rand()%71;
  randSizeY=80+rand()%71;

}

void DrawCircle(int cx,int cy,int rad,int fill)
{

    const double YS_PI=3.1415927;

    if(0!=fill)
    {
        glBegin(GL_POLYGON);
    }
    else
    {
        glBegin(GL_LINE_LOOP);
    }

    int i;
    for(i=0; i<64; i++)
    {
        double angle=(double)i*YS_PI/32.0;
        double x=(double)cx+cos(angle)*(double)rad;
        double y=(double)cy+sin(angle)*(double)rad;
        glVertex2d(x,y);
    }

    glEnd();
}
