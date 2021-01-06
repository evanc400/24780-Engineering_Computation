#include <stdio.h>
#include "fssimplewindow.h"

void VisualizeArrayContents(int n,int x[],int movedIndex1,int movedIndex2)
{
  //(You fill this function)
  for(int i=0; i<n;i++){
    int xCurrent=x[i]*20;
    int yCurrent=i*20;

    if(x[i]==movedIndex1 || x[i] == movedIndex2)glColor3ub(255,0,0);
    else glColor3ub(0,0,255);;    
    glBegin(GL_QUADS);
    glVertex2i(0,yCurrent);
    glVertex2i(xCurrent,yCurrent);
    glVertex2i(xCurrent,yCurrent+20);
    glVertex2i(0,yCurrent+20);
    glVertex2i(0,yCurrent);
    glEnd();
  }


}

void SwapInt(int &a,int &b)
{
 //(You fill this function. Also correct errors.)
 int c;
 c=b;
 b=a;
 a=c;

}

void Show(int n,int x[],int toBeMoved1,int toBeMoved2)
{
 FsPollDevice();
   while(FSKEY_NULL==FsInkey())
   {
     glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
     VisualizeArrayContents(n,x,toBeMoved1,toBeMoved2);
     FsSwapBuffers();

     FsPollDevice();
     FsSleep(10);
   }
}

void BubbleSort(int n,int x[])
{
 // (Modify this function so that the lines about to be swapped are drawn red)
 int i,j;
 for(i=0; i<n; i++)
 {
   for(j=i+1; j<n; j++)
   {
      if(x[i]>x[j])
      {
        Show(n,x,x[i],x[j]);
        SwapInt(x[i],x[j]);
      }
    }
 }
 Show(n,x,-1,-1); // You can leave this line as is. You’ll see the first line red in the end.
}

int main(void)
{
 int x[20]={17,10,13,14,15,6,3,2,9,8,18,7,12,11,16,4,1,20,5,19};
 FsOpenWindow(16,16,400,400,1);
 BubbleSort(20,x);
 return 0;
}
