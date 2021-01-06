#include <stdio.h>
#include "fssimplewindow.h"

void DrawPix(int x, int y)
{
    x=x*16;
    y=y*16;

    glColor3ub(0,0,0);

    glBegin(GL_QUADS);



    glVertex2i(x,y);
    glVertex2i(x+16,y);
    glVertex2i(x+16,y+16);
    glVertex2i(x,y+16);

    glEnd();
}


int main(void)
{
  // Character for the cell (x,y) will be pattern[y*16+x]
  char pattern[]=
  {
    // By the way, if you want to define a long string, you can write like this.
    "...1........1..."
    "...11......11..."
    "...1.1....1.1..."
    "...1..1..1..1..."
    "...1..1111..1..."
    "..1..........1.."
    "..1..11..11..1.."
    "..1..........1.."
    ".1..111..111..1."
    ".1.1.1.11.1.1.1."
    "1..1.1.11.1.1..1"
    "1...111..111...1"
    ".1............1."
    ".1.....11.....1."
    "..111......111.."
    ".....111111....."
  };

  FsOpenWindow(16,16,256,256,0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  // Fill your code here.

  for(int x=0; x<16; x++){
    for(int y=0; y<16; y++){

      if(pattern[y*16+x]=='1'){
        DrawPix(x,y);
      }
    }
  }
  glFlush();
  while(FSKEY_NULL==FsInkey())
  {

    FsPollDevice();
  }

  return 0;
}
