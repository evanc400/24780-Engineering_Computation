#include <stdio.h>
#include "fssimplewindow.h"
#include <iostream>

class CharBitmap
{
protected:
  int wid,hei;
  char *dat;
public:
  CharBitmap();
  ~CharBitmap();
  void CleanUp(void);
  int GetWidth(void) const;
  int GetHeight(void) const;
  void SetBitmap(int bmpWid,int bmpHei,char bmp[]);
  void SetPixel(int x,int y,char c);
  void Print(void) const;
  void Draw(void) const;
  char GetPixel(int x,int y) const;

  CharBitmap(const CharBitmap & incoming);
  CharBitmap &operator=(const CharBitmap &incoming);
  void copy_from(const CharBitmap &incoming);
};

void DrawPix(int x, int y);

CharBitmap::CharBitmap()
{
 // Write this function.
 dat=NULL;
 wid=0;
 hei=0;

}
CharBitmap::~CharBitmap()
{
  // Write this function.
  CleanUp();
}
void CharBitmap::CleanUp(void)
{
  // Write this function.
  delete [] dat;
  dat=NULL;
  wid=0;
  hei=0;

}
int CharBitmap::GetWidth(void) const
{
  return wid;
}
int CharBitmap::GetHeight(void) const
{
  return hei;
}
void CharBitmap::SetBitmap(int bmpWid,int bmpHei,char bmp[])
{
  // Write this function.
  wid=bmpWid;
  hei=bmpHei;
  dat= new char[wid*hei];

  for(int i=0; i<wid; i++){
    for(int j=0; j<hei; j++){
      dat[j*wid+i]=bmp[j*wid+i];
    }
  }
}
void CharBitmap::SetPixel(int x,int y,char c)
{
  // Write this function.
  if(x<wid && y< hei) dat[y*wid+x]=c;
}
void CharBitmap::Print(void) const
{
  int x,y;
  for(y=0; y<hei; y++)
  {
  for(x=0; x<wid; x++)
  {
  printf("%c",GetPixel(x,y));
  }
  printf("\n");
  }
}

void CharBitmap::Draw(void) const
{
 // Write this function.
 for(int x=0; x<wid; x++){
   for(int y=0; y<hei; y++){
     if(GetPixel(x,y)=='1'){
       DrawPix(x,y);
     }

   }
 }

}
char CharBitmap::GetPixel(int x,int y) const
{
  if(0<=x && x<wid && 0<=y && y<hei)
  {
    return dat[y*wid+x];
  }
  return 0;
}

int main(void)
{
  char pattern[]=
  {
  "....11....11...."
  "...1..1..1..1..."
  "...1.11..1.11..."
  "...1.11..1.11..."
  "...1.11..1.11..."
  "...1.11..1.11..."
  "...1.11..1.11..."
  "...1.11111.11..."
  "..1..........1.."
  ".1............1."
  "1..............1"
  "1..............1"
  "1..............1"
  "1......11......1"
  ".11..........11."
  "...1111111111..."
  };

  CharBitmap bmp;
  bmp.SetBitmap(16,16,pattern);
  bmp.SetPixel(4,10,'1');
  bmp.SetPixel(5,10,'1');
  bmp.SetPixel(4,11,'1');
  bmp.SetPixel(5,11,'1');
  bmp.SetPixel(10,10,'1');
  bmp.SetPixel(11,10,'1');
  bmp.SetPixel(10,11,'1');
  bmp.SetPixel(11,11,'1');
  bmp.Print();
  FsOpenWindow(16,16,256,256,1);
  while(FSKEY_NULL==FsInkey())
  {
    FsPollDevice();
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    bmp.Draw();
    FsSwapBuffers();

    FsSleep(10);
  }
  return 0;
}

void DrawPix(int x, int y)
{
    int scale=16;
    x=x*scale;
    y=y*scale;

    glColor3ub(0,0,0);

    glBegin(GL_QUADS);



    glVertex2i(x,y);
    glVertex2i(x+scale,y);
    glVertex2i(x+scale,y+scale);
    glVertex2i(x,y+scale);

    glEnd();
}

void CharBitmap::copy_from(const CharBitmap &incoming){

  CleanUp();
  wid=incoming.wid;
  hei=incoming.hei;

  dat=new char[wid*hei];

  for(int i=0; i<wid; i++){
    for(int j=0; j<hei; j++){
      dat[j*wid+i]=incoming.dat[j*wid+i];
    }
  }

}

CharBitmap::CharBitmap(const CharBitmap & incoming){

  wid=0;
  hei=0;

  dat=NULL;

  copy_from(incoming);

}

CharBitmap &CharBitmap::operator=(const CharBitmap &incoming){
  copy_from(incoming);
  return *this;
}
