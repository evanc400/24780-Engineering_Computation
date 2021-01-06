#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "fssimplewindow.h"

void DrawCircle(int cx,int cy,int rad,int fill);
void DrawBoard(int cx, int cy, int rad, int numCategories);
void DrawDivider(int cx, int cy, int rad, double angle);
void DrawSpinner(int cx, int cy, double angle);
void MoveSpinner(double &angleSpinner, double &omegaSpinner, double dt);

int main(){

  int terminate=0;
  int cx=400;
  int cy=300;
  int rad=290;
  int state=0; //0 for static spinner, 1 for in motion
  int numCategories;

  printf("Enter number of categories: ");
  scanf ("%d",&numCategories);

  FsOpenWindow(16,16,800,600,1);

  srand(time(NULL));


  double angleSpinner=0;
  double omegaSpinner;

  while(0==terminate){

    FsPollDevice();
    int key=FsInkey();

    switch(key)
        {
        case FSKEY_ESC:
            terminate=1;
            break;
        case FSKEY_SPACE:
          if(state==0){
              state=1;
              omegaSpinner=250+rand()%500;
          }
            break;
          }

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    DrawBoard(cx,cy,rad,numCategories);

    DrawSpinner(cx,cy,angleSpinner);

    if(state==1){
      MoveSpinner(angleSpinner,omegaSpinner,0.025);

      if(omegaSpinner<0.1){
        state=0;
      }
    }

    FsSwapBuffers();
    FsSleep(25);
  }

  return 0;
}

void DrawBoard(int cx, int cy, int rad, int numCategories){
    glColor3ub(0,0,0);

    DrawCircle(cx,cy,rad,0); //board outlline
    DrawCircle(cx,cy,4,1); //middle pin

    int angle_per=360/numCategories;

    for(int i=0; i<numCategories; i++){
      int dividerAngle=i*angle_per;
      DrawDivider(cx, cy,rad,dividerAngle);
    }


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

void DrawDivider(int cx, int cy, int rad, double angle){

  const double DEG2RAD=3.141592/180.0;

  double angle_rad=angle*DEG2RAD;

  glEnable(GL_LINE_STIPPLE);
  glLineStipple(1,0xff00);

  glColor3ub(0,0,0);
  glBegin(GL_LINES);
  glVertex2i(cx,cy);
  glVertex2i(cx+rad*cos(angle_rad),cy+rad*sin(angle_rad));
  glEnd();

  glDisable(GL_LINE_STIPPLE);

}

void DrawSpinner(int cx, int cy, double angleSpinner){

  const double DEG2RAD=3.141592/180.0;

  double angleSpinner_rad=angleSpinner*DEG2RAD;
  int radSpinner=250;

  int tipX=cx+radSpinner*cos(angleSpinner_rad);
  int tipY=cy+radSpinner*sin(angleSpinner_rad);
  glColor3ub(255,0,0);
  glBegin(GL_LINES);
  glVertex2i(cx,cy);
  glVertex2i(tipX,tipY);
  glEnd();

  DrawCircle(tipX,tipY,3,1);

}

void MoveSpinner(double &angleSpinner, double &omegaSpinner, double dt){

  double alpha=-30.0;
  angleSpinner=angleSpinner+omegaSpinner*dt;
  omegaSpinner=omegaSpinner+dt*alpha;

}
