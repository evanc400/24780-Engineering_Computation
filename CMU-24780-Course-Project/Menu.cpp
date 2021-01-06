#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include <stdio.h>
#include "yspng.h"
#include "Menu.h"



void GameMenu::DrawSelection(int choice) 
{
	int x = 380;
	int y;
	switch (choice) {
	case 1:
		y = 390;
		break;
	case 2:
		y = 490;
		break;
	//case 3:
	//	y = 590;
	//	break;
	}

	glColor3ub(255, 128, 0);
	glBegin(GL_TRIANGLES);
	glVertex2i(x, y);
	glVertex2i(x - 20, y - 10);
	glVertex2i(x - 20, y + 10);
	glEnd();

}

int GameMenu::Background(void)
{
	int choice = 1;
	YsRawPngDecoder background;
	background.Decode("background.png");
	background.Flip();

	for (;;)
	{
		FsPollDevice();
		auto key = FsInkey();
		if (FSKEY_ESC == key)
		{
			break;
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (0 < background.wid && 0 < background.hei)
		{
			glRasterPos2d(0, 799);
			glDrawPixels(background.wid, background.hei, GL_RGBA, GL_UNSIGNED_BYTE, background.rgba);
		}
		glColor3f(1.0, 0, 0);

		//glRasterPos2d(360, 150);
		glRasterPos2d(160, 150);
		YsGlDrawFontBitmap20x32("Space Shooter++");
		//glRasterPos2d(500, 300);
		glRasterPos2d(400, 400);
		YsGlDrawFontBitmap16x24("Start");
		//glRasterPos2d(400, 500);
		//YsGlDrawFontBitmap16x24("Tutorial");
		glRasterPos2d(400, 500);
		YsGlDrawFontBitmap16x24("Exit");

		if (key == FSKEY_UP && choice > 1) 
		{
			choice -= 1;
		}
		if (key == FSKEY_DOWN && choice < 2) 
		{
			choice += 1;
		}
		DrawSelection(choice);

		if (key == FSKEY_ENTER)
		{
			return choice;
		}

		FsSwapBuffers();

		FsSleep(10);
	}
	
	
}



//int main(void)
//{
//	GameMenu menu;
//	int choice = menu.Background();
//	printf("%d",choice);
//	return 0;
//}
