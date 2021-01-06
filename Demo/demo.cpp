#include <stdio.h>
#include <stdlib.h>
#include "fssimplewindow.h"
#include "yssimplesound.h"


class Pig{
  public:
    int state;
    int dx;
    int dy;
    int moveDummy;
    void Draw();
    void MoveRight();
    void MoveDown();
};

class Wolf{
  public:
    int state;
    int dx;
    int dy;
    int moveDummy;
    void Draw();
    void MoveRight();
    void DrawFlip();
    void DrawBlow();
    void MoveLeft();
};

void DrawRect(int x1,int y1,int x2,int y2,int fill);
void DrawRoof(int xOrgin, int yOrgin, int HouseSizeX, int HouseSizeY,int fill);
void DrawHutTop(int xOrgin, int yOrgin, int HouseSizeX, int HouseSizeY);
void DrawBrickBlock(int x1, int y1);
void DrawBrickBlock(int x1, int y1, int blockSizeX, int blockSizeY);
void DrawDoor(int x, int y, int doorSizeX, int doorSizeY, int state);
void DrawCircle(int cx,int cy,int rad,int fill);
void DrawWindow(int x, int y);
void DrawPix(int x, int y, int scale);
void DrawPig(int dx, int dy);
void DrawHouseHider(int xOrgin, int yOrgin, int xSize, int ySize, int dy, char type);
char *MyFgets(char str[],int limit,FILE *fp);

int main(){

  YsSoundPlayer player;
  YsSoundPlayer::SoundData door_openSE, door_closeSE,wolf_blowSE, victorySE;

  FsChangeToProgramDir();
  if(YSOK!=door_openSE.LoadWav("door_open.wav")){
    printf("Load error door open.\n");
    return 1;
  }

  if(YSOK!=door_closeSE.LoadWav("door_close.wav")){
    printf("Load error door close.\n");
    return 1;
  }

  if(YSOK!=wolf_blowSE.LoadWav("wolf_blow.wav")){
    printf("Load error wolf blow.\n");
    return 1;
  }

  if(YSOK!=victorySE.LoadWav("myVictorySound.wav")){
    printf("Load error victor.\n");
    return 1;
  }



  const int WINDOW_H =768;
  const int WINDOW_W=1024;


  FsOpenWindow(16,16,WINDOW_W,WINDOW_H,1);
  player.Start();


  int terminate=0;
  int brickDoorState=0;
  int hayDoorState=0;
  int stickDoorState=0;

  //House Params
  int HouseSizeX=150;
  int HouseSizeY=200;
  int HouseGap=100;
  int DoorSizeX=40;
  int DoorSizeY=67;

  int HayOrginX=360;
  int HayOrginY=WINDOW_H-30;
  int dyHay=0;
  int HayState=0;

  int StickOrginX=HayOrginX+HouseSizeX+HouseGap;
  int StickOrginY=WINDOW_H-30;
  int dyStick=0;
  int StickState=0;

  int BrickOrginX=StickOrginX+HouseSizeX+HouseGap;
  int BrickOrginY=WINDOW_H-30;
  int brickBlockSizeX=30;
  int brickBlockSizeY=10;
  int dyBrick=0;
  int BrickState=0;

  const int pigHSpawnX=4;
  const int pigSSpawnX=24;
  const int pigBSpawnX=44;
  const int pigSpawnY=235;

  Pig pigH;
  pigH.dx=pigHSpawnX;
  pigH.dy=pigSpawnY;
  pigH.state=0;
  pigH.moveDummy=0;
  Pig pigS;
  pigS.dx=pigSSpawnX;
  pigS.dy=pigSpawnY;
  pigS.state=0;
  pigS.moveDummy=0;
  Pig pigB;
  pigB.dx=pigBSpawnX;
  pigB.dy=pigSpawnY;
  pigB.state=0;
  pigB.moveDummy=0;

  Wolf wolf;
  wolf.dx=-100;
  wolf.dy=131;
  wolf.state=0;
  wolf.moveDummy=0;

  while(0==terminate){
    player.KeepPlaying();

    FsPollDevice();
    int key=FsInkey();

    switch(key)
      {
        case FSKEY_ESC:
          terminate=1;
          break;
        case FSKEY_SPACE:
          if(wolf.state==1){
            wolf.state=2;
          }
          else{
            wolf.state=0;

          }
          break;

      }

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    //Sky
    glColor3ub(220,244,255);
    DrawRect(0,0,WINDOW_W,WINDOW_H,1);

    //Earth
    glColor3ub(122,90,90);
    DrawRect(0,WINDOW_H-20,WINDOW_W,WINDOW_H,1);

    //Grass
    glColor3ub(76,153,0);
    DrawRect(0,WINDOW_H-20,WINDOW_W,WINDOW_H-30,1);

    //Hay House
    glColor3ub(255,255,0);
    DrawRect(HayOrginX,HayOrginY,HayOrginX+HouseSizeX,HayOrginY-HouseSizeY,1);
    DrawRoof(HayOrginX,HayOrginY,HouseSizeX,HouseSizeY,1);
    glColor3ub(0,0,0);
    DrawRect(HayOrginX,HayOrginY,HayOrginX+HouseSizeX,HayOrginY-HouseSizeY,0);
    DrawRoof(HayOrginX,HayOrginY,HouseSizeX,HouseSizeY,0);
    DrawDoor(HayOrginX+55,HayOrginY, DoorSizeX, DoorSizeY, hayDoorState);

    //Stick House
    glColor3ub(102,51,0);
    DrawRect(StickOrginX,StickOrginY,StickOrginX+HouseSizeX,StickOrginY-HouseSizeY,1);
    DrawRoof(StickOrginX,StickOrginY,HouseSizeX,HouseSizeY,1);
    glColor3ub(0,0,0);
    DrawRect(StickOrginX,StickOrginY,StickOrginX+HouseSizeX,StickOrginY-HouseSizeY,0);
    DrawRoof(StickOrginX,StickOrginY,HouseSizeX,HouseSizeY,0);
    DrawDoor(StickOrginX+55,StickOrginY, DoorSizeX, DoorSizeY, stickDoorState);

    //Brick House
    //int brickDoorState=0;
    for(int i=0; i<HouseSizeX/brickBlockSizeX; i++){
      for(int j=0; j<HouseSizeY/brickBlockSizeY; j++){
        DrawBrickBlock(BrickOrginX+i*brickBlockSizeX,BrickOrginY-j*brickBlockSizeY,brickBlockSizeX,brickBlockSizeY);
      }
    }


    glColor3ub(96,96,96);
    DrawRoof(BrickOrginX,BrickOrginY,HouseSizeX,HouseSizeY,1);
    DrawDoor(BrickOrginX+55,BrickOrginY, DoorSizeX, DoorSizeY, brickDoorState);

    //Window backdrop
    glColor3ub(255,255,0);
    int HayWindowX=HayOrginX+40;
    int HayWindowY=HayOrginY-100;
    DrawRect(HayWindowX,HayWindowY,HayWindowX+70,HayWindowY-95,1);

    int StickWindowX=StickOrginX+40;
    int StickWindowY=StickOrginY-100;
    DrawRect(StickWindowX,StickWindowY,StickWindowX+70,StickWindowY-95,1);

    int BrickWindowX=BrickOrginX+40;
    int BrickWindowY=BrickOrginY-100;
    DrawRect(BrickWindowX,BrickWindowY,BrickWindowX+70,BrickWindowY-95,1);

    //if(HayState==1|| HayState==2) pigH.Draw();
    //if(StickState==1|| StickState==2) pigS.Draw();
    pigH.Draw();
    pigS.Draw();
    pigB.Draw();

    DrawWindow(HayWindowX,HayWindowY);
    if(HayState==0 ){
      glColor3ub(220,244,255);
      DrawRect(HayOrginX-10,HayOrginY-100-HouseSizeY,HayOrginX+HouseSizeX+10,HayOrginY,1);
      pigH.Draw();
    }
    DrawWindow(StickWindowX,StickWindowY);
    if(StickState==0){
      glColor3ub(220,244,255);
      DrawRect(StickOrginX-10,StickOrginY-100-HouseSizeY,StickOrginX+HouseSizeX+10,StickOrginY,1);
      if(HayState==0) pigH.Draw();
      pigS.Draw();
    }
    DrawWindow(BrickWindowX,BrickWindowY);
    if(BrickState==0){
      glColor3ub(220,244,255);
      DrawRect(BrickOrginX-10,BrickOrginY-100-HouseSizeY,BrickOrginX+HouseSizeX+10,BrickOrginY,1);
      pigB.Draw();
    }

    if(pigH.state==0){
      //walking to house
      if(pigH.dx<HayOrginX/3-20){
        pigH.MoveRight();
      }
      else{
        pigH.state=1;
      }
    }
    if(pigS.state==0){
      //walking to house
      if(pigS.dx<StickOrginX/3-20){
        pigS.MoveRight();
      }
      else{
        pigS.state=1;
      }
    }
    if(pigB.state==0){
      //walking to house
      if(pigB.dx<BrickOrginX/3-20){
        pigB.MoveRight();
      }
      else{
        pigB.state=1;

      }

    }


    if(pigH.state==1 && pigS.state==1 && pigB.state==1){
      DrawHouseHider(HayOrginX,HayOrginY,HouseSizeX,HouseSizeY,dyHay--,'h');
      HayState=1;
      if(dyHay<(-HouseSizeY-100)*1){
        pigH.state=2;
        player.PlayOneShot(door_openSE);
      }
      //if(dyHay<(-HouseSizeY-100)*2) pigH.state=2;

    }
    if(pigS.state==1 && pigB.state==1){
      DrawHouseHider(StickOrginX,StickOrginY,HouseSizeX,HouseSizeY,dyStick--,'s');
      StickState=1;
      if(dyStick<(-HouseSizeY-100)*2){
        pigS.state=2;
        player.PlayOneShot(door_openSE);
      }
      //if(dyStick<(-HouseSizeY-100)*3) pigS.state=2;
    }
    if(pigB.state==1){
      DrawHouseHider(BrickOrginX, BrickOrginY,HouseSizeX,HouseSizeY,dyBrick--,'b');
      BrickState=1;
      if(dyBrick<(-HouseSizeY-100)*5){
        pigB.state=2;
        player.PlayOneShot(door_openSE);
      }
      //if(dyBrick<(-HouseSizeY-100)*8) pigB.state=2;
    }

    if(pigH.state==2){
      //Approach Door
        pigH.MoveRight();
        hayDoorState=1;
      //Open Door and Enter
      if(pigH.dx>HayOrginX/3+19){
        pigH.dx=HayWindowX/3 + 3;
        pigH.dy=HayWindowY/3 - 10;
        pigH.state=3;
      }
    }

    if(pigS.state==2){
      //Approach Door
        pigS.MoveRight();
        stickDoorState=1;
      //Open Door and Enter
      if(pigS.dx>StickOrginX/3+19){
        pigS.dx=StickWindowX/3 + 3;
        pigS.dy=StickWindowY/3 - 10;
        pigS.state=3;
      }
    }

    if(pigB.state==2){
      //Approach Door
        pigB.MoveRight();
        brickDoorState=1;
      //Open Door and Enter
      if(pigB.dx>BrickOrginX/3+19){
        pigB.dx=BrickWindowX/3 + 3;
        pigB.dy=BrickWindowY/3 - 10;
        pigB.state=3;
      }
    }

    //pig in house, close door
    if(pigH.state==3){
      hayDoorState=0;
      player.PlayOneShot(door_closeSE);
      pigH.state=4;
    }

    //pig in house, close door
    if(pigS.state==3){
      stickDoorState=0;
      pigS.state=4;
      player.PlayOneShot(door_closeSE);
    }

    //pig in house, close door
    if(pigB.state==3){
      brickDoorState=0;
      pigB.state=4;
      player.PlayOneShot(door_closeSE);
    }

    //Once all pigs in houses, wolf enters
    if(pigH.state==4 && pigS.state==4 && pigB.state==4 && wolf.state!=2){
      wolf.state=1;
    }

    //Standard draw wolf on all states not involved huffing and puffing
    if(wolf.state!=2 && wolf.state!=5 && wolf.state!=8 && wolf.state!=9){
      wolf.Draw();
    }

    if(wolf.state==1){
      if(wolf.dx<HayOrginX/5-30){
        wolf.MoveRight();
      }
      else{
        wolf.state=2;
      }
    }

    //wolf waits for effect then blows house down, prompting pig to fall
    if(wolf.state==2){
      if(wolf.moveDummy<50){
        wolf.Draw();
        wolf.moveDummy++;
      }
      else if(wolf.moveDummy<170){
          wolf.DrawBlow();
          wolf.moveDummy++;
          player.PlayOneShot(wolf_blowSE);
          if(wolf.moveDummy>80) HayState=0;
      }
      else{
            wolf.Draw();
            wolf.moveDummy++;
            if(wolf.moveDummy>190){
              wolf.moveDummy=0;
              wolf.state=3;
              pigH.state=5;
            }
      }
    }

    //pigH falls
    if(pigH.state==5){
        if(pigH.dy<pigSpawnY){
          pigH.MoveDown();
        }
        else{
          pigH.moveDummy++;
          if(pigH.moveDummy<15){
            pigH.state=6;
            pigH.moveDummy=0;
          }
        }
      }

      //pigH runs to stick house
      if(pigH.state==6){
        pigH.MoveRight();

        if(pigH.dx>StickOrginX/3-20){
          stickDoorState=1;
          player.PlayOneShot(door_openSE);
        }

        if(pigH.dx>StickOrginX/3+19){
          pigH.dx=StickWindowX/3+3;
          pigH.dy=StickWindowY/3-19;
          pigH.state=7;
          pigS.state=7;
          wolf.state=4;
          stickDoorState=0;
          player.PlayOneShot(door_closeSE);
        }
      }

      //Wolf approaches Stick house
      if(wolf.state==4){
        if(wolf.dx<StickOrginX/5-30){
          wolf.MoveRight();
        }
        else{
          wolf.state=5;
        }
      }

      //wolf waits for effect then blows stick house down, prompting pig to fall
      if(wolf.state==5){
        if(wolf.moveDummy<50){
          wolf.Draw();
          wolf.moveDummy++;
        }
        else if(wolf.moveDummy<170){
            wolf.DrawBlow();
            wolf.moveDummy++;
            player.PlayOneShot(wolf_blowSE);
            if(wolf.moveDummy>80) StickState=0;
        }
        else{
              wolf.Draw();
              wolf.moveDummy++;
              if(wolf.moveDummy>190){
                wolf.moveDummy=0;
                wolf.state=6;
                pigH.state=8;
                pigS.state=8;
              }
        }
      }

      if(pigH.state==8 && pigS.state==8){

      }

      //pigS falls
      if(pigS.state==8){
          if(pigS.dy<pigSpawnY){
            pigS.MoveDown();
          }
          else{
            pigS.moveDummy++;
            if(pigS.moveDummy<15){
              pigS.state=9;
              pigS.moveDummy=0;
            }
          }
        }

        //pigH falls
        if(pigH.state==8){
            if(pigH.dy<pigSpawnY-9){
              pigH.MoveDown();
            }
            else{
              pigH.moveDummy++;
              if(pigH.moveDummy<15){
                pigH.state=9;
                pigH.moveDummy=0;
              }
            }
          }


      //pigH runs to brick house
      if(pigH.state==9){
        pigH.MoveRight();

        if(pigH.dx>BrickOrginX/3-20){
          brickDoorState=1;
          player.PlayOneShot(door_openSE);
        }

        if(pigH.dx>BrickOrginX/3+19){
          pigH.dx=BrickWindowX/3+3;
          pigH.dy=BrickWindowY/3-19;
          pigH.state=10;
          wolf.state=7;
          brickDoorState=0;
          player.PlayOneShot(door_closeSE);
        }
      }

      //pigS runs to brick house
      if(pigS.state==9){
        pigS.MoveRight();

        if(pigS.dx>BrickOrginX/3-20){
          brickDoorState=1;
        }

        if(pigS.dx>BrickOrginX/3+19){
          pigS.dx=BrickWindowX/3+3;
          pigS.dy=BrickWindowY/3-28;
          pigS.state=10;
          wolf.state=7;
          brickDoorState=0;
        }
      }

      //Wolf approaches Brick house
      if(wolf.state==7){
        if(wolf.dx<BrickOrginX/5-30){
          wolf.MoveRight();
        }
        else{
          wolf.state=8;
        }
      }

      //wolf waits for effect then blows stick house down, prompting pig to fall
      if(wolf.state==8){
        if(wolf.moveDummy<50){
          wolf.Draw();
          wolf.moveDummy++;
        }
        else if(wolf.moveDummy<170){
            wolf.DrawBlow();
            player.PlayOneShot(wolf_blowSE);
            wolf.moveDummy++;
        }
        else if (wolf.moveDummy<190){
              wolf.Draw();
              wolf.moveDummy++;

        }
        else if (wolf.moveDummy<310){
              wolf.DrawBlow();
              player.PlayOneShot(wolf_blowSE);
              wolf.moveDummy++;

        }
        else if (wolf.moveDummy<360){
              wolf.Draw();
              wolf.moveDummy++;

        }
        else if (wolf.moveDummy<480){
              player.PlayOneShot(wolf_blowSE);
              wolf.DrawBlow();
              wolf.moveDummy++;

        }
        else if (wolf.moveDummy<500){
              wolf.Draw();
              wolf.moveDummy++;
        }
        else{
              wolf.moveDummy=0;
              wolf.state=9;

        }
      }

      if(wolf.state==9){
        player.PlayOneShot(victorySE);
        wolf.DrawFlip();
        wolf.MoveLeft();
        if(wolf.dx==-170) terminate=1;
      }


    FsSwapBuffers();
    FsSleep(25);
  }

  player.End();
}

void DrawRect(int x1,int y1,int x2,int y2,int fill)
{
    if(0!=fill)
    {
        glBegin(GL_QUADS);
    }
    else
    {
        glBegin(GL_LINE_LOOP);
    }

    glVertex2i(x1,y1);
    glVertex2i(x2,y1);
    glVertex2i(x2,y2);
    glVertex2i(x1,y2);

    glEnd();
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

void DrawHutTop(int xOrgin, int yOrgin, int HouseSizeX, int HouseSizeY){

  int roofCenter=HouseSizeX/2;

  glColor3ub(230,254,255);

  glBegin(GL_TRIANGLES);
  glVertex2i(xOrgin-1,yOrgin-(HouseSizeY-roofCenter));
  glVertex2i(xOrgin+roofCenter+2,yOrgin-HouseSizeY-2);
  glVertex2i(xOrgin-1,yOrgin-HouseSizeY-2);

  glBegin(GL_TRIANGLES);
  glVertex2i(xOrgin+HouseSizeX+1,yOrgin-(HouseSizeY-roofCenter));
  glVertex2i(xOrgin+HouseSizeX-roofCenter,yOrgin-HouseSizeY-2);
  glVertex2i(xOrgin+HouseSizeX+1,yOrgin-HouseSizeY-2);

  glEnd();

  glColor3ub(0,0,0);
  glBegin(GL_LINE_LOOP);
  glVertex2i(xOrgin,yOrgin-(HouseSizeY-roofCenter));
  glVertex2i(xOrgin+roofCenter,yOrgin-HouseSizeY);
  glVertex2i(xOrgin+HouseSizeX,yOrgin-(HouseSizeY-roofCenter));


  glEnd();

}




void DrawRoof(int xOrgin, int yOrgin, int HouseSizeX, int HouseSizeY,int fill){

  if(0!=fill)
  {
      glBegin(GL_TRIANGLES);
  }
  else
  {
      glBegin(GL_LINE_LOOP);
  }

  glVertex2i(xOrgin-10,yOrgin-HouseSizeY);
  glVertex2i(xOrgin+HouseSizeX+10,yOrgin-HouseSizeY);
  glVertex2i(xOrgin+(HouseSizeX/2),yOrgin-HouseSizeY-100);

  glEnd();


}

void DrawDoor(int x, int y, int doorSizeX, int doorSizeY, int state){
  if(state==0){

    glColor3ub(153,76,0);
    glBegin(GL_QUADS);
    glVertex2i(x,y);
    glVertex2i(x,y-doorSizeY);
    glVertex2i(x+doorSizeX,y-doorSizeY);
    glVertex2i(x+doorSizeX,y);
    glEnd();

    glColor3ub(255,221,47);
    DrawCircle(x+7,y-22,2,1);
  }

  else{
    glColor3ub(0,0,0);
    glBegin(GL_QUADS);
    glVertex2i(x,y);
    glVertex2i(x,y-doorSizeY);
    glVertex2i(x+doorSizeX-7,y-doorSizeY);
    glVertex2i(x+doorSizeX-7,y);
    glEnd();

    glColor3ub(153,76,0);
    glBegin(GL_QUADS);
    glVertex2i(x+doorSizeX-7,y);
    glVertex2i(x+doorSizeX-7,y-doorSizeY);
    glVertex2i(x+doorSizeX,y-doorSizeY);
    glVertex2i(x+doorSizeX,y);
    glEnd();
  }
}


void DrawBrickBlock(int x1, int y1, int blockSizeX, int blockSizeY){

  int blockOrginX=x1;


  glColor3ub(204,0,0);
  DrawRect(x1,y1,x1+blockSizeX,y1-blockSizeY,1);

  glColor3ub(216,216,216);

  //bottom pattern
  int x2=x1+blockSizeX/3;
  int y2=y1-blockSizeY/2;
  DrawRect(x1,y1,x2,y2,0);
  x1=x2;
  x2=x1+blockSizeX/3;
  DrawRect(x1,y1,x2,y2,0);
  x1=x2;
  x2=x1+blockSizeX/3;
  DrawRect(x1,y1,x2,y2,0);

  //top pattern
  y1=y2;
  y2=y1-blockSizeY/2;
  x1=blockOrginX+blockSizeX/6;;
  x2=x1+blockSizeX/3;
  DrawRect(x1,y1,x2,y2,0);
  x1=x2;
  x2=x1+blockSizeX/3;
  DrawRect(x1,y1,x2,y2,0);
  x1=x2;
  x2=x1+blockSizeX/6;
  //DrawRect(x1,y1,x2,y2,0);

}

void DrawWindow(int x, int y){


  //Crossers
  glColor3ub(0,0,0);
  glBegin(GL_LINES);
  glVertex2i(x+35,y-95);
  glVertex2i(x+35,y);
  glEnd();

  glColor3ub(0,0,0);
  glBegin(GL_LINES);
  glVertex2i(x,y-35);
  glVertex2i(x+70,y-35);
  glEnd();

  //Glass
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glColor4ub(128,128,128,100);

  DrawRect(x,y,x+70,y-95,1);

  glDisable(GL_BLEND);
}

void DrawPix(int x, int y, int scale)
{
    x=x*scale;
    y=y*scale;


    glBegin(GL_QUADS);

    glVertex2i(x,y);
    glVertex2i(x+scale,y);
    glVertex2i(x+scale,y+scale);
    glVertex2i(x,y+scale);

    glEnd();
}

void Pig::Draw(){

  int scale=3;

  char pattern[]=
  {
    ".......1....1.."
    "....111111111.."
    "...1222222221.."
    "..122222222221."
    "..122222232231."
    ".11222222244444"
    "1.1222222241414"
    "..1222222244444"
    "..122222222221."
    "...1111111111.."
    "...1.1...1.1..."
  };

  for(int x=0; x<15; x++){
    for(int y=0; y<11; y++){

      if(pattern[y*15+x]=='1'){
        glColor3ub(201,130,130);
        DrawPix(x+dx,y+dy,scale);
      }
      if(pattern[y*15+x]=='2'){
        glColor3ub(255,181,228);
        DrawPix(x+dx,y+dy,scale);
      }
      if(pattern[y*15+x]=='3'){
        glColor3ub(0,0,0);
        DrawPix(x+dx,y+dy,scale);
      }
      if(pattern[y*15+x]=='4'){
        glColor3ub(218,109,123);
        DrawPix(x+dx,y+dy,scale);
      }

    }
  }
}

void Pig::MoveRight(){
  moveDummy++;
  if(moveDummy==1){
    dx++;
    moveDummy=0;
  }
}

void DrawHouseHider(int xOrgin, int yOrgin, int xSize, int ySize, int dy, char type){

  if(type=='h'){
    dy=dy;
    //dy=dy/2;
    }
  if(type=='s'){
    dy=dy/2;
    //dy=dy/3;
    }

  if(type=='b'){
    //dy=dy/3;
    dy=dy/5;
    }


  glColor3ub(220,244,255);
  int x1=xOrgin-10;
  int x2=xOrgin+xSize+10;
  int y1=yOrgin-100-ySize;
  int y2=yOrgin+dy;
  DrawRect(x1,y1,x2,y2,1);

}

void Wolf::Draw(){

  int scale=5;

  char pattern[]=
  {
    "...................1.1..."//1
    "..................1.1...."//2
    "..................11111.."//3
    ".................111112.."//4
    ".................11121111"//5
    "................11111111."//6
    "..............11111111..."//7
    "..........111111111111..."//8
    "........11111111111111..."//9
    ".......11111111111111...."//10
    ".......1111111111111....."//11
    "......111111111111.1....."//12
    "......1111.11....1.1....."//13
    "1....11.11.11....1.1....."//14
    ".111111.1.11....1..1....."//15
    "..111..1..1.....1..1....."//16
    ".......1..1.....1..1....."//17
  };

  for(int x=0; x<25; x++){
    for(int y=0; y<17; y++){

      if(pattern[y*25+x]=='1'){
        glColor3ub(0,0,0);
        DrawPix(x+dx,y+dy,scale);
      }
      if(pattern[y*25+x]=='2'){
        glColor3ub(255,0,0);
        DrawPix(x+dx,y+dy,scale);
      }

    }
  }
}

void Wolf::DrawBlow(){

  int scale=5;

  char pattern[]=
  {
"...................................."//1
"...................................."//2
"...................................."//3
"...................................."//4
".....................1.1............"//5
"......................1.1..........."//6
".................1111.11111........."//7
"...............111111111112........."//8
"...........111111111111221111...3333"//9
".........11111111111111111...333...."//10
"........111111111111111111.........."//11
"......11111111111111111...11....3333"//12
"1....11111111111111.1..............."//13
".111111.111.11....1.1..............."//14
"..1111.111.11....1.1................"//15
".......1..1.....1..1................"//16
".......1..1.....1..1................"//17
  };

  for(int x=0; x<36; x++){
    for(int y=0; y<17; y++){

      if(pattern[y*36+x]=='1'){
        glColor3ub(0,0,0);
        DrawPix(x+dx,y+dy,scale);
      }
      if(pattern[y*36+x]=='2'){
        glColor3ub(255,0,0);
        DrawPix(x+dx,y+dy,scale);
      }
      if(pattern[y*36+x]=='3'){
        glColor3ub(0,220,255);
        DrawPix(x+dx,y+dy,scale);
      }

    }
  }
}

void Wolf::MoveRight(){
  moveDummy++;
  if(moveDummy==2){
    dx++;
    moveDummy=0;
  }

}

void Pig::MoveDown(){
  dy++;
}

void Wolf::DrawFlip(){

    int scale=5;

    char pattern[]=
    {
      "...1.1..................."
      "....1.1.................."
      "..11111.................."
      "..211111................."
      "11112111................."
      ".11111111................"
      "...11111111.............."
      "...111111111111.........."
      "...11111111111111........"
      "....11111111111111......."
      ".....1111111111111......."
      ".....1.111111111111......"
      ".....1.1....11.1111......"
      ".....1.1....11.11.11....1"
      ".....1..1....11.1.111111."
      ".....1..1.....1..1..111.."
      ".....1..1.....1..1......."
    };

    for(int x=0; x<25; x++){
      for(int y=0; y<17; y++){

        if(pattern[y*25+x]=='1'){
          glColor3ub(0,0,0);
          DrawPix(x+dx,y+dy,scale);
        }
        if(pattern[y*25+x]=='2'){
          glColor3ub(255,0,0);
          DrawPix(x+dx,y+dy,scale);
        }

      }
    }
}

void Wolf::MoveLeft(){
  moveDummy++;
  if(moveDummy==2){
    dx--;
    moveDummy=0;
  }

}

char *MyFgets(char str[],int limit,FILE *fp)
{
   if(nullptr!=fgets(str,limit,fp))
   {
     int l=0;
     for(l=0; 0!=str[l]; ++l)
   {
   }
   for(l=l-1; 0<=l && ('\n'==str[l] || '\r'==str[l]); --l)
   {
     str[l]=0;
   }
   return str;
   }
   return nullptr;
}
