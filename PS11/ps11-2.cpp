#include <stdio.h>
#include "fssimplewindow.h"

class Drawable
{
public:
	virtual void Draw(void) const;
};

class Movable : public Drawable
{
protected:
	int x,y;
public:
	virtual void Move(void);
	void SetPosition(int xx,int yy);
};

class MouseChaser : public Movable
{
public:
	void Move(void);
	void Draw(void) const;
};

class MovableByArrowKeys : public Movable
{
public:
	void Move(void);
	void Draw(void) const;
};

void Drawable::Draw(void) const
{
}

void Movable::Move(void)
{
}

void Movable::SetPosition(int xx,int yy)
{
	x=xx;
	y=yy;
}

void MouseChaser::Move(void)
{
	int lb,mb,rb,mx,my;
	FsGetMouseEvent(lb,mb,rb,mx,my);
	if(x<mx-3)
	{
		x+=3;
	}
	else if(mx+3<x)
	{
		x-=3;
	}
	if(y<my-3)
	{
		y+=3;
	}
	else if(my+3<y)
	{
		y-=3;
	}
}

void MouseChaser::Draw(void) const
{
	glColor3ub(0,255,0);
	glBegin(GL_LINE_LOOP);
	glVertex2i(x-10,y);
	glVertex2i(x,y-10);
	glVertex2i(x+10,y);
	glVertex2i(x,y+10);
	glEnd();
}

void MovableByArrowKeys::Move(void)
{
	if(0!=FsGetKeyState(FSKEY_LEFT))
	{
		x-=5;
	}
	if(0!=FsGetKeyState(FSKEY_RIGHT))
	{
		x+=5;
	}
	if(0!=FsGetKeyState(FSKEY_UP))
	{
		y-=5;
	}
	if(0!=FsGetKeyState(FSKEY_DOWN))
	{
		y+=5;
	}
}

void MovableByArrowKeys::Draw(void) const
{
	glColor3ub(0,0,255);
	glBegin(GL_QUADS);
	glVertex2i(x-10,y-10);
	glVertex2i(x+10,y-10);
	glVertex2i(x+10,y+10);
	glVertex2i(x-10,y+10);
	glEnd();
}

int main(void)
{
	FsOpenWindow(16,16,800,600,1);

	MouseChaser diamond;
	MovableByArrowKeys square;

	const int nMovable=2;
	Movable *movables[nMovable]={&diamond,&square};

	for(int i=0; i<nMovable; ++i)
	{
		movables[i]->SetPosition(400,300);
	}

	for(;;)
	{
		FsPollDevice();

		const int key=FsInkey();
		if(FSKEY_ESC==key)
		{
			break;
		}

		for(int i=0; i<nMovable; ++i)
		{
			movables[i]->Move();
		}

		glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
		for(int i=0; i<nMovable; ++i)
		{
			movables[i]->Draw();
		}
		FsSwapBuffers();

		FsSleep(25);
	}

	return 0;
}
