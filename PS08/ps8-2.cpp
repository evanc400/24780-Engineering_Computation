#include <stdio.h>
#include <stdlib.h>
#include "fssimplewindow.h"
// Define your own Drawing class.

class Drawing{

protected:
  int numLines;
  int *x1;
  int *x2;
  int *y1;
  int *y2;
  int *r;
  int *g;
  int *b;
public:

  Drawing();
  ~Drawing();
  void clear();
  void copy_from(const Drawing &incoming);
  Drawing(const Drawing &incoming);
  Drawing &operator=(const Drawing &incoming);

  bool Load(const char fn[]);
  void Draw() const;

};

Drawing::Drawing(){
  numLines=0;
  x1=nullptr;
  x2=nullptr;
  y1=nullptr;
  y2=nullptr;
  r=nullptr;
  g=nullptr;
  b=nullptr;

}

Drawing::~Drawing(){
  clear();
}

void Drawing::clear(){

  delete [] x1;
  delete [] y1;
  delete [] x2;
  delete [] y2;
  delete [] r;
  delete [] g;
  delete [] b;

  x1=nullptr;
  x2=nullptr;
  y1=nullptr;
  y2=nullptr;
  r=nullptr;
  g=nullptr;
  b=nullptr;

}

Drawing::Drawing(const Drawing &incoming){

  numLines=0;
  x1=nullptr;
  x2=nullptr;
  y1=nullptr;
  y2=nullptr;
  r=nullptr;
  g=nullptr;
  b=nullptr;

  copy_from(incoming);
}

Drawing &Drawing::operator=(const Drawing &incoming){
  copy_from(incoming);
  return *this;
}

void Drawing::copy_from(const Drawing &incoming){
  clear();
  numLines=incoming.numLines;

  x1=new int[numLines];
  for(int i=0; i<numLines; i++){
    this->x1[i]=incoming.x1[i];
  }

  y1=new int[numLines];
  for(int i=0; i<numLines; i++){
    this->y1[i]=incoming.y1[i];
  }

  x2=new int[numLines];
  for(int i=0; i<numLines; i++){
    this->x2[i]=incoming.x2[i];
  }

  y2=new int[numLines];
  for(int i=0; i<numLines; i++){
    this->y2[i]=incoming.y2[i];
  }

  r=new int[numLines];
  for(int i=0; i<numLines; i++){
    this->r[i]=incoming.r[i];
  }

  g=new int[numLines];
  for(int i=0; i<numLines; i++){
    this->g[i]=incoming.g[i];
  }

  b=new int[numLines];
  for(int i=0; i<numLines; i++){
    this->b[i]=incoming.b[i];
  }
}

void SafeStrCpy(char dst[],char src[],int nLetters,int nLimit);
int ParseString(
    int &nWord,
    int wordTop[],
    int wordLength[],
    char str[],
    int maxNumWords);
char *MyFgets(char str[],int limit,FILE *fp);
Drawing LoadFile(const char fn[]);

bool Drawing::Load(const char filename[])
{

    FILE *fp;
    fp=fopen(filename,"r");

    bool open;

    if(NULL!=fp)
    {
        open=true;

        char str[256];

        MyFgets(str,255,fp);

        str[255]=0;
        int nWord,wordTop[16],wordLength[16];

        ParseString(nWord,wordTop,wordLength,str,16);

        char nLinesStr[256];
        SafeStrCpy(nLinesStr,str+wordTop[0],wordLength[0],255);

        numLines=atoi(nLinesStr);
        x1=new int[numLines];
        y1=new int[numLines];
        x2=new int[numLines];
        y2=new int[numLines];
        r=new int[numLines];
        g=new int[numLines];
        b=new int[numLines];

        for(int i=0; i<numLines; i++){

          MyFgets(str,255,fp);
          ParseString(nWord,wordTop,wordLength,str,16);
          char x1Str[256],y1Str[256],x2Str[256],y2Str[256],rStr[256],gStr[256],bStr[256];
          SafeStrCpy(x1Str,str+wordTop[0],wordLength[0],255);
          SafeStrCpy(y1Str,str+wordTop[1],wordLength[1],255);
          SafeStrCpy(x2Str,str+wordTop[2],wordLength[2],255);
          SafeStrCpy(y2Str,str+wordTop[3],wordLength[3],255);
          SafeStrCpy(rStr,str+wordTop[4],wordLength[4],255);
          SafeStrCpy(gStr,str+wordTop[5],wordLength[5],255);
          SafeStrCpy(bStr,str+wordTop[6],wordLength[6],255);


          x1[i]=atoi(x1Str);
          y1[i]=600-atoi(y1Str);
          x2[i]=atoi(x2Str);
          y2[i]=600-atoi(y2Str);
          r[i]=atoi(rStr);
          g[i]=atoi(gStr);
          b[i]=atoi(bStr);

        }

        fclose(fp);

    }
    else
    {
        open=false;
    }

    return open;
}

void Drawing::Draw() const{
  for(int i=0; i<numLines; i++){
    glColor3ub(r[i],g[i],b[i]);
    glBegin(GL_LINES);
    glVertex2i(x1[i],y1[i]);
    glVertex2i(x2[i],y2[i]);
    glEnd();
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

Drawing LoadFile(const char fn[])
{
  Drawing d;
  if(0==d.Load(fn))
  {
    printf("Cannot read the file.\n");
    exit(1);
  }
  return d;
}

int main(void)
{
  char fn[256];
  printf("Enter File Name>");
  MyFgets(fn,255,stdin);

  Drawing load=LoadFile(fn),drawing;
  drawing=load; // Force using operator=

  FsOpenWindow(16,16,800,600,1);
  for(;;)
  {
    FsPollDevice();
    auto key=FsInkey();
    if(FSKEY_ESC==key)
    {
      break;
    }
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    drawing.Draw();
    FsSwapBuffers();
    FsSleep(10);
  }
  return 0;
  }
  void SafeStrCpy(char dst[],char src[],int nLetters,int nLimit)
  {
      int i,stopper;
      if(nLetters<nLimit)
      {
          stopper=nLetters;
      }
      else
      {
          stopper=nLimit;
      }

      for(i=0; i<stopper; i++)
      {
          dst[i]=src[i];
      }
      dst[stopper]=0;
  }

  int ParseString(
      int &nWord,
      int wordTop[],
      int wordLength[],
      char str[],
      int maxNumWords)
  {
      int i,state;

      state=0;
      nWord=0;
      for(i=0; str[i]!=0; i++)
      {
          switch(state)
          {
          case 0:
              if(str[i]!=' ' &&
                 str[i]!='\t' &&
                 str[i]!=',')
              {
                  state=1;
                  wordTop[nWord]=i;
                  wordLength[nWord]=1;
                  nWord++;
              }
              break;
            case 1:
               if(str[i]!=' ' &&
                  str[i]!='\t' &&
                  str[i]!=',')
               {
                   wordLength[nWord-1]++;
               }
               else
               {
                   state=0;
                   if(nWord==maxNumWords)
                   {
                       goto LOOPOUT;
                   }
               }
               break;
           }
       }
   LOOPOUT:
       return nWord;
   }
