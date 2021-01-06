#define _CRT_SECURE_NO_WARNINGS

#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include <stdio.h>
#include "yspng.h"
#include "InGameMenu.h"
#include <cstring>

TextString::TextString()
{
    str = nullptr;
}
TextString::TextString(const TextString& incoming)
{
    str = nullptr;
    CopyFrom(incoming);
}
TextString::~TextString()
{
    CleanUp();
}

TextString& TextString::operator=(const TextString& incoming)
{
    CopyFrom(incoming);
    return *this;
}

void TextString::CopyFrom(const TextString& incoming)
{
    Set(incoming.GetPointer());
}

void TextString::CleanUp(void)
{
    if (nullptr != str)
    {
        delete[] str;
        str = nullptr;
    }
}

void TextString::Add(char c)
{
    if (nullptr == str)
    {
        const char newStr[2] = { c,0 };
        Set(newStr);
    }
    else
    {
        // Allocate
        auto l = strlen(str);
        char* newStr = new char[l + 2];

        // Copy
        strcpy(newStr, str);
        newStr[l] = c;
        newStr[l + 1] = 0;

        // Set(newStr);
        // delete [] newStr;

        // CleanUp
        CleanUp();

        // Re-point
        str = newStr;
    }
}

void TextString::Add(const char str[])
{
    for (int i = 0; str[i] != 0; ++i)
    {
        Add(str[i]);
    }
}

void TextString::Set(const char incoming[])
{
    if (incoming != str)
    {
        auto l = strlen(incoming);
        char* newStr = new char[l + 1];
        strcpy(newStr, incoming);

        CleanUp();

        str = newStr;
    }
}

void TextString::BackSpace(void)
{
    if (nullptr != str)
    {
        auto l = strlen(str);

        if (0 < l)
        {
            str[l - 1] = 0;

            // Allocate
            char* newStr = new char[l];

            // Copy
            strcpy(newStr, str);

            // CleanUp
            CleanUp();

            // Re-point
            str = newStr;
        }
    }
}

const char* TextString::GetPointer(void) const
{
    if (nullptr != str)
    {
        return str;
    }
    else
    {
        return "";
    }
}

int TextString::Strlen(void) const
{
    return strlen(GetPointer());
}

const char* TextString::Fgets(FILE* fp)
{
    CleanUp();

    char str[16];
    for (;;)
    {
        if (nullptr != fgets(str, 15, fp))
        {
            for (int i = 0; str[i] != 0; ++i)
            {
                if (str[i] == '\n')
                {
                    return GetPointer();
                }
                Add(str[i]);
            }
        }
        else
        {
            if (0 < Strlen())
            {
                return GetPointer();
            }
            else
            {
                return nullptr;
            }
        }
    }
}

void InGameMenu::AddText(int level, char* score, int health, char* weapon)
{
    showlevel.CleanUp();
    showweapon.CleanUp();
    showscore.CleanUp();
    showlevel.Add("Live: ");
    showlevel.Add(level + '0');
    showscore.Add("Score: ");
    showscore.Add(score);
    showweapon.Add("Weapon: ");
    showweapon.Add(weapon);
}

int InGameMenu::EndMenu(void)
{
    GLuint texId[6];
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    YsRawPngDecoder map;
    map.Decode("map.png");
    glGenTextures(1, &texId[0]);  // Reserve one texture identifier
    glBindTexture(GL_TEXTURE_2D, texId[0]);  // Making the texture identifier current (or bring it to the deck)

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D
    (GL_TEXTURE_2D,
        0,    // Level of detail
        GL_RGBA,
        map.wid,
        map.hei,
        0,    // Border width, but not supported and needs to be 0.
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        map.rgba);

    for (;;)
    {
        FsPollDevice();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glColor4d(1.0, 1.0, 1.0, 2.0);
        //Transparent
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_TEXTURE_2D);  // Begin using texture mapping
        //Draw Background
        //glBindTexture(GL_TEXTURE_2D, texId[0]);

        glBegin(GL_QUADS);

        glTexCoord2d(0.0, 0.0);
        glVertex2i(0, 0);

        glTexCoord2d(1.0, 0.0);
        glVertex2i(600, 0);

        glTexCoord2d(1.0, 1.0);
        glVertex2i(600, 800);

        glTexCoord2d(0.0, 1.0);
        glVertex2i(0, 800);

        glEnd();

        glDisable(GL_TEXTURE_2D);  // End using texture mapping
        /*auto key = FsInkey();
        if (FSKEY_ESC == key)
        {
            break;
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (0 < 600 && 0 < 800)
        {
            glRasterPos2d(0, 799);
            glDrawPixels(map.wid, map.hei, GL_RGBA, GL_UNSIGNED_BYTE, map.rgba);
        }*/

        glColor3f(1.0, 1, 0);
        glRasterPos2d(200, 350);
        YsGlDrawFontBitmap20x32("You Lose!");
        glRasterPos2d(180, 420);
        YsGlDrawFontBitmap20x32("Play Again?");

        glRasterPos2d(180, 600);
        YsGlDrawFontBitmap16x24("ENTER to Game");
        glRasterPos2d(190, 700);
        YsGlDrawFontBitmap16x24("ESC to Exit");

        lastKey = FsInkey();
        if (FSKEY_ESC == lastKey)
        {
            return 0;
        }
        else if (FSKEY_ENTER == lastKey)
        {
            return 1;
        }
        FsSwapBuffers();

        FsSleep(10);
    }
}


void InGameMenu::Display(int level, char* score, int health, char* weapon)
{
    //char pattern[] =
    //{
    //	"..11...11.."
    //	".1111.1111."
    //	"11111111111"
    //	"11111111111"
    //	".111111111."
    //	"..1111111.."
    //	"...11111..."
    //	"....111...."
    //	".....1....."
    //};
    char pattern[] =
    {
        "...11......11..."
        "..1111....1111.."
        ".111111..111111."
        "1111111111111111"
        "1111111111111111"
        ".11111111111111."
        "..111111111111.."
        "...1111111111..."
        "....11111111...."
        ".....111111....."
        "......1111......"
        ".......11......."
        "................"
        /*"................"
        "................"
        "................"*/
    };


    AddText(level, score, health, weapon);

    glColor3ub(255, 128, 0);
    glRasterPos2d(15, 60);
    YsGlDrawFontBitmap16x20(showlevel.GetPointer());
    glRasterPos2d(15, 90);
    YsGlDrawFontBitmap16x20(showweapon.GetPointer());
    glRasterPos2d(15, 120);
    YsGlDrawFontBitmap16x20(showscore.GetPointer());

    if (health >= 1)
    {
        glBegin(GL_QUADS);
        for (int y = 0;y < 16;y++)
        {
            for (int x = 0;x < 16;x++)
            {
                if (pattern[y * 16 + x] == '1')
                {
                    glVertex2i(x * 2 + 10, y * 2 + 130);
                    glVertex2i(x * 2 + 2 + 10, y * 2 + 130);
                    glVertex2i(x * 2 + 2 + 10, y * 2 + 5 + 130);
                    glVertex2i(x * 2 + 10, y * 2 + 2 + 130);
                    glColor3f(1.0, 0.0, 0.0);
                }

            }
        }
        glEnd();
    }
    if (health >= 2)
    {
        glBegin(GL_QUADS);
        for (int y = 0;y < 16;y++)
        {
            for (int x = 0;x < 16;x++)
            {
                if (pattern[y * 16 + x] == '1')
                {
                    glVertex2i(x * 2 + 50, y * 2 + 130);
                    glVertex2i(x * 2 + 2 + 50, y * 2 + 130);
                    glVertex2i(x * 2 + 2 + 50, y * 2 + 5 + 130);
                    glVertex2i(x * 2 + 50, y * 2 + 2 + 130);
                    glColor3f(1.0, 0.0, 0.0);
                }

            }
        }
        glEnd();
    }
    if (health >= 3)
    {
        glBegin(GL_QUADS);
        for (int y = 0;y < 16;y++)
        {
            for (int x = 0;x < 16;x++)
            {
                if (pattern[y * 16 + x] == '1')
                {
                    glVertex2i(x * 2 + 90, y * 2 + 130);
                    glVertex2i(x * 2 + 2 + 90, y * 2 + 130);
                    glVertex2i(x * 2 + 2 + 90, y * 2 + 5 + 130);
                    glVertex2i(x * 2 + 90, y * 2 + 2 + 130);
                    glColor3f(1.0, 0.0, 0.0);
                }

            }
        }
        glEnd();
    }
    if (health >= 4)
    {
        glBegin(GL_QUADS);
        for (int y = 0;y < 16;y++)
        {
            for (int x = 0;x < 16;x++)
            {
                if (pattern[y * 16 + x] == '1')
                {
                    glVertex2i(x * 2 + 130, y * 2 + 130);
                    glVertex2i(x * 2 + 2 + 130, y * 2 + 130);
                    glVertex2i(x * 2 + 2 + 130, y * 2 + 5 + 130);
                    glVertex2i(x * 2 + 130, y * 2 + 2 + 130);
                    glColor3f(1.0, 0.0, 0.0);
                }

            }
        }
        glEnd();
    }
    if (health ==5)
    {
        glBegin(GL_QUADS);
        for (int y = 0;y < 16;y++)
        {
            for (int x = 0;x < 16;x++)
            {
                if (pattern[y * 16 + x] == '1')
                {
                    glVertex2i(x * 2 + 170, y * 2 + 130);
                    glVertex2i(x * 2 + 2 + 170, y * 2 + 130);
                    glVertex2i(x * 2 + 2 + 170, y * 2 + 5 + 130);
                    glVertex2i(x * 2 + 170, y * 2 + 2 + 130);
                    glColor3f(1.0, 0.0, 0.0);
                }

            }
        }
        glEnd();
    }
}
