/*
Move: arrows
Space: Shoot
change to standard missile: S
change to missile B: B
Simulate hit: H
*/

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <math.h>
#include <cstring>

#include "yssimplesound.h"
#include "yspng.h"
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "yspng.h"
#include "InGameMenu.h"
#include "Menu.h"
#include <cstring>

const int nMissiles = 100;

int playerLives = 3;

using namespace std;
YsRawPngDecoder png[8];
GLuint texId[8];

void DrawAllMissiles();
void DrawRect(int blockX, int blockY, int blockSizeX, int blockSizeY); //used for demo health bar
void DrawCircle(int cx, int cy, int rad, int fill); //used for demo force field

//sound object
YsSoundPlayer dynamicPlayer;
YsSoundPlayer::SoundData dynamicWav[7];
//

int absValue(int i)
{
    return i < 0 ? -i : i;
}

bool checkCollision(int objectX, int objectY, int objectW, int objectH, int targetX, int targetY, int targetW, int targetH)
{
    int dx = absValue(objectX - targetX);
    int dy = absValue(objectY - targetY);
    if (0 <= dx && dx < (targetW + objectW) && 0 <= dy && dy < (targetH + objectW))
    {
        return true;
    }
    return false;
}

int CheckCollision(
    int pX, int pY,
    int objX, int objY, int objsizeX, int objsizeY)
{
    int relativeX, relativeY;
    relativeX = pX - objX;
    relativeY = pY - objY;
    if (-9 - objsizeX <= relativeX && relativeX <= objsizeX + 9 && -4 <= relativeY && relativeY < objsizeY + 19)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

const int nParticlePerExplosion = 100;
class Explosion
{
public:
    int state = 0;
    int counter;
    int xy[nParticlePerExplosion * 2];
    int vxvy[nParticlePerExplosion * 2];
    void Initialize(void);
    bool Begin(int x, int y);
    void Move(void);
    void Draw(void);
    void Disappear(void);
};
void Explosion::Initialize(void)
{
    state = 0;
}
bool Explosion::Begin(int x, int y)
{
    if (0 == state)
    {
        state = 1;
        counter = 0;
        for (int i = 0; i < nParticlePerExplosion; ++i)
        {
            xy[i * 2] = x;
            xy[i * 2 + 1] = y;
            vxvy[i * 2] = rand() % 11 - 5;
            vxvy[i * 2 + 1] = rand() % 11 - 5;
        }
        return true;
    }
    return false;
}
void Explosion::Move(void)
{
    for (int i = 0; i < nParticlePerExplosion; ++i)
    {
        xy[i * 2] += vxvy[i * 2];
        xy[i * 2 + 1] += vxvy[i * 2 + 1];
    }

    ++counter;
    if (50 < counter)
    {
        Disappear();
    }
}
void Explosion::Draw(void)
{
    glPointSize(5);
    glColor3ub(255, 0, 0);
    glBegin(GL_POINTS);
    for (int i = 0; i < nParticlePerExplosion; ++i)
    {
        glVertex2i(xy[i * 2], xy[i * 2 + 1]);
    }
    glEnd();
}
void Explosion::Disappear(void)
{
    state = 0;
}

class Player {
public:
    int x;
    int y;
    int health = 0;
    int state = 0; //Whether ship is able to take damage: ie 0 if either dead or in the midst of respawning, 1 if in regular play
    bool respawn = true; //expression used to check if ships coordinates should be reset to respawn location
    bool spawning = true; //expersssion used if ship is in midst of respawning
    Explosion e;
    string missileType = "Standard";
    int initializeCount = 0;
    void Shoot();
    void ChangeMissile(string newType);
    void Draw();
    void Initialize();
};



//Just for demo: This class is just a sample missile class.
class MissileStandard
{
public:
    int x, y, state;
    int yStart;
    int distanceTraveled = 0;
    bool IsFlying(void);
    void Initialize(void);
    //bool CheckCollision(Target t);
    bool GoneOutOfWindow(void);
    bool TryShoot(int x0, int y0, int index);
    void Move(void);
    void Draw(void);
};

MissileStandard missileStandard[nMissiles];
int shootWait = 10;//Change

bool MissileStandard::IsFlying(void)
{
    return 0 != state;
}
void MissileStandard::Initialize(void)
{
    state = 0;
}
/*bool MissileStandard::CheckCollision(Target t)
{
    return ::CheckCollision(x,y,t.x,t.y,t.w,t.h);
}*/
bool MissileStandard::GoneOutOfWindow(void)
{
    return y < 0;
}
void MissileStandard::Move(void)
{
    if (0 != state)
    {
        y -= 10;
        distanceTraveled = distanceTraveled + 10;
        if (true == GoneOutOfWindow())
        {
            state = 0;
            distanceTraveled = 0;
        }
    }
}
bool MissileStandard::TryShoot(int x0, int y0, int index)//Change
{

    if (0 == state && shootWait > 100)
    {
        state = 1;
        x = x0;
        y = y0;
        shootWait = 0;
        return true;


    }
    shootWait++;
    return false;
}
void MissileStandard::Draw(void)
{
    //glColor3ub(255, 0, 0);
    glBegin(GL_QUADS);
    glVertex2i(x - 2, y);
    glVertex2i(x + 2, y);
    glVertex2i(x + 2, y + 9);
    glVertex2i(x - 2, y + 9);
    glEnd();
}

//Just for demo: This class is just a second sample missile class
class MissileRapid
{
public:
    int x, y, state;
    bool IsFlying(void);
    void Initialize(void);
    //bool CheckCollision(Target t);
    bool GoneOutOfWindow(void);
    bool TryShoot(int x0, int y0);
    void Move(void);
    void Draw(void);
};

MissileRapid missileRapid[nMissiles];

bool MissileRapid::IsFlying(void)
{
    return 0 != state;
}
void MissileRapid::Initialize(void)
{
    state = 0;
}
/*bool MissileRapid::CheckCollision(Target t)
{
    return ::CheckCollision(x,y,t.x,t.y,t.w,t.h);
}*/
bool MissileRapid::GoneOutOfWindow(void)
{
    return y < 0;
}
void MissileRapid::Move(void)
{
    if (0 != state)
    {
        y -= 10;
        if (true == GoneOutOfWindow())
        {
            state = 0;
        }
    }
}
bool MissileRapid::TryShoot(int x0, int y0)
{
    if (0 == state)
    {
        state = 1;
        x = x0;
        y = y0;
        return true;
    }
    return false;
}
void MissileRapid::Draw(void)
{
    //glColor3ub(0, 255, 255);
    glBegin(GL_QUADS);
    glVertex2i(x - 2, y);
    glVertex2i(x + 2, y);
    glVertex2i(x + 2, y + 9);
    glVertex2i(x - 2, y + 9);
    glEnd();
}

//SpreadM

//SpreadM
class MissileSpreadM
{
public:
    int x, y, state;
    bool IsFlying(void);
    void Initialize(void);
    //bool CheckCollision(Target t);
    bool GoneOutOfWindow(void);
    bool TryShoot(int x0, int y0);
    void Move(void);
    void Draw(void);
};

MissileSpreadM missileSpreadM[nMissiles];

bool MissileSpreadM::IsFlying(void)
{
    return 0 != state;
}
void MissileSpreadM::Initialize(void)
{
    state = 0;
}
/*bool MissileSpreadM::CheckCollision(Target t)
{
    return ::CheckCollision(x,y,t.x,t.y,t.w,t.h);
}*/
bool MissileSpreadM::GoneOutOfWindow(void)
{
    return y < 0;
}
void MissileSpreadM::Move(void)
{
    if (0 != state)
    {
        y -= 10;
        if (true == GoneOutOfWindow())
        {
            state = 0;
        }
    }
}
bool MissileSpreadM::TryShoot(int x0, int y0)
{
    if (0 == state)
    {
        state = 1;
        x = x0;
        y = y0;
        return true;
    }
    return false;
}
void MissileSpreadM::Draw(void)
{
    //glColor3ub(0, 255, 0);
    glBegin(GL_QUADS);
    glVertex2i(x - 2, y);
    glVertex2i(x + 2, y);
    glVertex2i(x + 2, y + 9);
    glVertex2i(x - 2, y + 9);
    glEnd();
}

///Left
class MissileSpreadL
{
public:
    int x, y, state;
    bool IsFlying(void);
    void Initialize(void);
    //bool CheckCollision(Target t);
    bool GoneOutOfWindow(void);
    bool TryShoot(int x0, int y0);
    void Move(void);
    void Draw(void);
};

MissileSpreadL missileSpreadL[nMissiles];

bool MissileSpreadL::IsFlying(void)
{
    return 0 != state;
}
void MissileSpreadL::Initialize(void)
{
    state = 0;
}
/*bool MissileSpreadL::CheckCollision(Target t)
{
    return ::CheckCollision(x,y,t.x,t.y,t.w,t.h);
}*/
bool MissileSpreadL::GoneOutOfWindow(void)
{
    return y < 0;
}
void MissileSpreadL::Move(void)
{
    if (0 != state)
    {
        y -= 10;
        x -= 1;
        if (true == GoneOutOfWindow())
        {
            state = 0;
        }
    }
}
bool MissileSpreadL::TryShoot(int x0, int y0)
{
    if (0 == state)
    {
        state = 1;
        x = x0;
        y = y0;
        return true;
    }
    return false;
}
void MissileSpreadL::Draw(void)
{
    /*glColor3ub(0, 255, 0);*/
    glBegin(GL_QUADS);
    glVertex2i(x - 2, y);
    glVertex2i(x + 2, y);
    glVertex2i(x + 2, y + 9);
    glVertex2i(x - 2, y + 9);
    glEnd();
}

//Right Spread
class MissileSpreadR
{
public:
    int x, y, state;
    bool IsFlying(void);
    void Initialize(void);
    //bool CheckCollision(Target t);
    bool GoneOutOfWindow(void);
    bool TryShoot(int x0, int y0);
    void Move(void);
    void Draw(void);
};

MissileSpreadR missileSpreadR[nMissiles];

bool MissileSpreadR::IsFlying(void)
{
    return 0 != state;
}
void MissileSpreadR::Initialize(void)
{
    state = 0;
}
/*bool MissileSpreadR::CheckCollision(Target t)
{
    return ::CheckCollision(x,y,t.x,t.y,t.w,t.h);
}*/
bool MissileSpreadR::GoneOutOfWindow(void)
{
    return y < 0;
}
void MissileSpreadR::Move(void)
{
    if (0 != state)
    {
        y -= 10;
        x += 1;
        if (true == GoneOutOfWindow())
        {
            state = 0;
        }
    }
}
bool MissileSpreadR::TryShoot(int x0, int y0)
{
    if (0 == state)
    {
        state = 1;
        x = x0;
        y = y0;
        return true;
    }
    return false;
}
void MissileSpreadR::Draw(void)
{
    /*glColor3ub(0, 255, 0);*/
    glBegin(GL_QUADS);
    glVertex2i(x - 2, y);
    glVertex2i(x + 2, y);
    glVertex2i(x + 2, y + 9);
    glVertex2i(x - 2, y + 9);
    glEnd();
}


//We no longer need this one, just kept it so the rest of code would compile while eveyrone is making changes
class MissileB
{
public:
    int x, y, state;
    bool IsFlying(void);
    void Initialize(void);
    //bool CheckCollision(Target t);
    bool GoneOutOfWindow(void);
    bool TryShoot(int x0, int y0);
    void Move(void);
    void Draw(void);
};

MissileB missileB[nMissiles];

bool MissileB::IsFlying(void)
{
    return 0 != state;
}
void MissileB::Initialize(void)
{
    state = 0;
}
/*bool MissileB::CheckCollision(Target t)
{
  return ::CheckCollision(x,y,t.x,t.y,t.w,t.h);
}*/
bool MissileB::GoneOutOfWindow(void)
{
    return y < 0;
}
void MissileB::Move(void)
{
    if (0 != state)
    {
        y -= 10;
        if (true == GoneOutOfWindow())
        {
            state = 0;
        }
    }
}
bool MissileB::TryShoot(int x0, int y0)
{
    if (0 == state)
    {
        state = 1;
        x = x0;
        y = y0;
        return true;
    }
    return false;
}
void MissileB::Draw(void)
{
    /*glColor3ub(0, 0, 0);*/
    glBegin(GL_QUADS);
    glVertex2i(x - 2, y);
    glVertex2i(x + 2, y);
    glVertex2i(x + 2, y + 9);
    glVertex2i(x - 2, y + 9);
    glEnd();
}

class Obstacles
{
public:
    int x, y, state;
    void GenerateObstacle();
    void Draw();
    int Collide(Player& player);
};

void Obstacles::GenerateObstacle()
{

    state = 1;
    x = rand() % 500;
    y = rand() % 500;

}

void Obstacles::Draw()
{
    if (state != 0)
    {

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texId[3]);

        glBegin(GL_QUADS);

        glTexCoord2d(0.0, 0.0);
        glVertex2i(x - 20, y - 20);

        glTexCoord2d(1.0, 0.0);
        glVertex2i(x + 20, y - 20);

        glTexCoord2d(1.0, 1.0);
        glVertex2i(x + 20, y + 20);

        glTexCoord2d(0.0, 1.0);
        glVertex2i(x - 20, y + 20);

        glEnd();

        glDisable(GL_TEXTURE_2D);
    }

}

int Obstacles::Collide(Player& player)
{
    return CheckCollision(player.x, player.y, x, y, 20, 30);
}



class Life
{
public:
    int x, y, state;
    void GenerateLife();
    void Draw();
    int Collide(Player& player);
};

void Life::GenerateLife()
{

    state = 1;
    x = rand() % 500;
    y = rand() % 500;

}

void Life::Draw()
{
    if (state != 0)
    {
        /*glColor3ub(255, 192, 203);
        glBegin(GL_QUADS);
        glVertex2i(x - 10, y);
        glVertex2i(x + 10, y);
        glVertex2i(x + 10, y + 20);
        glVertex2i(x - 10, y + 20);
        glEnd();*/

        glEnable(GL_TEXTURE_2D);  // Begin using texture mapping
        glBindTexture(GL_TEXTURE_2D, texId[5]);

        glBegin(GL_QUADS);

        glTexCoord2d(0.0, 0.0);
        glVertex2i(x - 15, y - 15);

        glTexCoord2d(1.0, 0.0);
        glVertex2i(x + 15, y - 15);

        glTexCoord2d(1.0, 1.0);
        glVertex2i(x + 15, y + 15);

        glTexCoord2d(0.0, 1.0);
        glVertex2i(x - 15, y + 15);

        glEnd();
        glDisable(GL_TEXTURE_2D);
    }

}

int Life::Collide(Player& player)
{
    return CheckCollision(player.x, player.y, x, y, 10, 20);
}


class Shield
{
public:
    int x, y, state, d = 0,n=200;
    void GenerateShield();
    void Draw();
    int Collide(Player& player);
};

void Shield::GenerateShield()
{

    state = 1;
    x = rand() % 500;
    y = rand() % 500;

}

void Shield::Draw()
{
    if (state != 0)
    {
        /*glColor3ub(0, 0, 128);
        glBegin(GL_QUADS);
        glVertex2i(x - 10, y);
        glVertex2i(x + 10, y);
        glVertex2i(x + 10, y + 20);
        glVertex2i(x - 10, y + 20);
        glEnd();*/

        glEnable(GL_TEXTURE_2D);  // Begin using texture mapping
        glBindTexture(GL_TEXTURE_2D, texId[4]);

        glBegin(GL_QUADS);

        glTexCoord2d(0.0, 0.0);
        glVertex2i(x - 15, y - 15);

        glTexCoord2d(1.0, 0.0);
        glVertex2i(x + 15, y - 15);

        glTexCoord2d(1.0, 1.0);
        glVertex2i(x + 15, y + 15);

        glTexCoord2d(0.0, 1.0);
        glVertex2i(x - 15, y + 15);

        glEnd();
        glDisable(GL_TEXTURE_2D);
    }

}

int Shield::Collide(Player& player)
{
    return CheckCollision(player.x, player.y, x, y, 10, 20);
}

class Coins
{
public:
    int x, y, state;
    void GenerateCoins();
    void Draw();
    int Collide(Player& player);
};

void Coins::GenerateCoins()
{

    state = 1;
    x = rand() % 500;
    y = rand() % 500;

}

void Coins::Draw()
{
    if (state != 0)
    {
        /*glColor3ub(255, 255, 0);
        glBegin(GL_QUADS);
        glVertex2i(x - 10, y);
        glVertex2i(x + 10, y);
        glVertex2i(x + 10, y + 20);
        glVertex2i(x - 10, y + 20);
        glEnd();*/

        glEnable(GL_TEXTURE_2D);  // Begin using texture mapping
        glBindTexture(GL_TEXTURE_2D, texId[6]);

        glBegin(GL_QUADS);

        glTexCoord2d(0.0, 0.0);
        glVertex2i(x - 15, y - 15);

        glTexCoord2d(1.0, 0.0);
        glVertex2i(x + 15, y - 15);

        glTexCoord2d(1.0, 1.0);
        glVertex2i(x + 15, y + 15);

        glTexCoord2d(0.0, 1.0);
        glVertex2i(x - 15, y + 15);

        glEnd();
        glDisable(GL_TEXTURE_2D);
    }

}

int Coins::Collide(Player& player)
{
    return CheckCollision(player.x, player.y, x, y, 10, 20);
}


class Weapons
{
public:
    int x, y, state,type=1;
    void GenerateWeapons();
    void Draw();
    int Collide(Player& player);
};

void Weapons::GenerateWeapons()
{

    state = 1;
    x = rand() % 500;
    y = rand() % 500;
}

void Weapons::Draw()
{
    if (state != 0)
    {
        /*if(type==1)
            glColor3ub(255,0,0);
        if(type==2)
            glColor3ub(0, 255, 0);
        if(type==3)
            glColor3ub(255,255,255);
        glBegin(GL_QUADS);
        glVertex2i(x - 10, y);
        glVertex2i(x + 10, y);
        glVertex2i(x + 10, y + 20);
        glVertex2i(x - 10, y + 20);
        glEnd();*/

        glEnable(GL_TEXTURE_2D);  // Begin using texture mapping
        glBindTexture(GL_TEXTURE_2D, texId[7]);

        glBegin(GL_QUADS);

        glTexCoord2d(0.0, 0.0);
        glVertex2i(x - 15, y - 15);

        glTexCoord2d(1.0, 0.0);
        glVertex2i(x + 15, y - 15);

        glTexCoord2d(1.0, 1.0);
        glVertex2i(x + 15, y + 15);

        glTexCoord2d(0.0, 1.0);
        glVertex2i(x - 15, y + 15);

        glEnd();
        glDisable(GL_TEXTURE_2D);
    }

}

int Weapons::Collide(Player& player)
{
    return CheckCollision(player.x, player.y, x, y, 25, 25);
}




// Enemy and enemy missiles

int score = 0;

class Enemy
{
public:
    int x, y, xSize, ySize, state;
    //void Move();
};

class missileEnemy : public Enemy
{
public:
    void Move(Player& playerObject);
    void checkEnemyCollision(Player& playerObject);
};

class standardEnemy : public Enemy
{
public:
    int pathPlace, state, fireMode;
    std::vector<int> movementPathX, movementPathY;
    std::vector<missileEnemy> missilesFired;
    standardEnemy();
    //~standardEnemy(); // may need to write destructor due to vector
    void generatePath1(const int startingYLocation);
    void generatePath2(const int startingYLocation);
    void generatePath3(const int startingYLocation);
    void generatePath4(const int startingYLocation);
    void generatePath5(const int startingyLocation);
    void generatePath6(const int startingYLocation);
    void generatePath7(const int startingyLocation);
    void generateRandomPath(int startingYLocation);
    void Move(Player& playerObject, MissileStandard missileStandard[nMissiles], MissileRapid missileRapid[nMissiles], MissileSpreadM missileSpreadM[nMissiles], MissileSpreadL missileSpreadL[nMissiles], MissileSpreadR missileSpreadR[nMissiles], vector<Shield> shield);
    void checkEnemyCollision(Player& playerObject);
    void Draw();
    void Fire();
    void resetPosition();
};

class magnetEnemy : public Enemy
{
    void Move(const int playerX, const int playerY); // could replace with a player object
    //void Draw();
};

void missileEnemy::Move(Player& playerObject)
{
    y += 5;
    checkEnemyCollision(playerObject);
}

void missileEnemy::checkEnemyCollision(Player& playerObject)
{
    bool hit = checkCollision(x, y, xSize, ySize, playerObject.x, playerObject.y, 10, 20);
    if (hit == true) {
        state = 0;
        playerObject.health--; // player loses 1 hp if hit by a missile
        // player takes a hit - play sound
        dynamicPlayer.PlayOneShot(dynamicWav[5]);
    }
}

void standardEnemy::Fire()
{
    missileEnemy newMissile;
    newMissile.state = 1;
    newMissile.x = x + xSize / 2;
    newMissile.y = y;
    newMissile.xSize = 5;
    newMissile.ySize = 5;
    missilesFired.emplace_back(newMissile);
    // enemy shooting - play sound (something quietish? - this sound happens a lot)
}

standardEnemy::standardEnemy()
{
    x = 800;
    y = 800;
    xSize = 20;
    ySize = 20;
    state = 3;
    pathPlace = 0;
    generateRandomPath(rand() % 100 + 450); // generate some path
}

void standardEnemy::generatePath1(const int startingYLocation)
{
    // Mapping some equation to the path
    for (float i = 0; i < 600; i++) { //assuming 100 fps (10ms sleep/frame)
        // equation used is sin(x)
        movementPathX.emplace_back(i); // need to change this to append to vector
        movementPathY.emplace_back((int)(startingYLocation + (float)100 * sin(i / 100.0)));
    }
}

void standardEnemy::generatePath2(const int startingYLocation)
{
    // Mapping some equation to the path
    for (float i = 600; i > 0; i--) { // 200 elements for a 4 second enemy on screen time, assuming 50 fps (25ms sleep/frame)
        // faster and larger than path 1
        movementPathX.emplace_back(i); // need to change this to append to vector
        movementPathY.emplace_back((int)(startingYLocation + (float)200 * sin(i / 50.0)));
    }
}

void standardEnemy::generatePath3(const int startingYLocation)
{
    for (float i = 0; i < 600; i++) {
        movementPathX.emplace_back(i);
        movementPathY.emplace_back((0.5 * i) + startingYLocation);
    }
}

void standardEnemy::generatePath4(const int startingYLocation)
{
    for (float i = 599; i >= 0; i--) {
        movementPathX.emplace_back(i);
        movementPathY.emplace_back((0.5 * i) + startingYLocation);
    }
}

void standardEnemy::generatePath5(const int startingYLocation)
{
    for (float i = 0; i < 600; i++) {
        movementPathX.emplace_back(i);
        movementPathY.emplace_back((-0.1) * (i - 300) * (i - 300) + 200 + startingYLocation);
    }
}

void standardEnemy::generatePath6(const int startingYLocation)
{
    for (float i = 599; i >= 0; i--) {
        movementPathX.emplace_back(i);
        movementPathY.emplace_back((-0.1) * (i - 300) * (i - 300) + 200 + startingYLocation);
    }
}

void standardEnemy::generatePath7(const int startingYLocation)
{
    for (float i = 0; i < 600; i++) {
        movementPathX.emplace_back(i);
        movementPathY.emplace_back(startingYLocation);
    }
}

void standardEnemy::generateRandomPath(int startingYLocation)
{
    int randomNum = (rand() % 7) + 1; // 1 to 7
    int randomizeY = (rand() % 150) + 1 - 300; // -150 to 150
    startingYLocation = startingYLocation + randomizeY;
    switch (randomNum) {
    case 1:
        generatePath1(startingYLocation);
        break;
    case 2:
        generatePath2(startingYLocation);
        break;
    case 3:
        generatePath3(startingYLocation);
        break;
    case 4:
        generatePath4(startingYLocation);
        break;
    case 5:
        generatePath5(startingYLocation);
        break;
    case 6:
        generatePath6(startingYLocation);
        break;
    case 7:
        generatePath7(startingYLocation);
        break;
    default:
        generatePath2(startingYLocation);
        break;
    }
}

void standardEnemy::checkEnemyCollision(Player& playerObject) {
    bool hit = checkCollision(x, y, xSize, ySize, playerObject.x, playerObject.y, 30, 30);
    if (hit == true) {
        state--;
        playerObject.health--; // player loses 1 hp if hit by a missile
        // player takes a hit - play sound
        //dynamicPlayer.PlayOneShot(dynamicWav[5]);
    }
}

void standardEnemy::Move(Player& playerObject, MissileStandard missileStandard[nMissiles], MissileRapid missileRapid[nMissiles], MissileSpreadM missileSpreadM[nMissiles], MissileSpreadL missileSpreadL[nMissiles], MissileSpreadR missileSpreadR[nMissiles], vector<Shield> shield)
{
    int shieldOn = 0;
    if (state > 0) {
        checkEnemyCollision(playerObject);
        for (int i = 0; i < shield.size(); i++) {
            if (shield[i].d == 1) {
                shieldOn = 1;
                break;
            }
        }
        bool isShot = 0;
        for (int i = 0; i < nMissiles; i++)
        {
            isShot = checkCollision(missileStandard[i].x, missileStandard[i].y, 2, 2, x, y, xSize, ySize);
            if (isShot && (shieldOn == 0)) {
                state--;
                missileStandard[i].state = 0;
                if (state == 0) {
                    score+=2;
                    // enemy death - play sound
                    dynamicPlayer.PlayOneShot(dynamicWav[6]);
                }
            }
        }
        for (int i = 0; i < nMissiles; i++)
        {
            isShot = checkCollision(missileRapid[i].x, missileRapid[i].y, 2, 2, x, y, xSize, ySize);
            if (isShot && (shieldOn == 0)) {
                state--;
                missileRapid[i].state = 0;
                if (state == 0) {
                    score+=2;
                    // enemy death - play sound
                    dynamicPlayer.PlayOneShot(dynamicWav[6]);
                }
            }
        }
        for (int i = 0; i < nMissiles; i++)
        {
            isShot = checkCollision(missileSpreadM[i].x, missileSpreadM[i].y, 2, 2, x, y, xSize, ySize);
            if (isShot && (shieldOn == 0)) {
                state--;
                missileSpreadM[i].state = 0;
                if (state == 0) {
                    score+=2;
                    // enemy death - play sound
                    dynamicPlayer.PlayOneShot(dynamicWav[6]);
                }
            }
        }
        for (int i = 0; i < nMissiles; i++)
        {
            isShot = checkCollision(missileSpreadL[i].x, missileSpreadL[i].y, 2, 2, x, y, xSize, ySize);
            if (isShot && (shieldOn == 0)) {
                state--;
                missileSpreadL[i].state = 0;
                if (state == 0) {
                    score+=2;
                    // enemy death - play sound
                    dynamicPlayer.PlayOneShot(dynamicWav[6]);
                }
            }
        }
        for (int i = 0; i < nMissiles; i++)
        {
            isShot = checkCollision(missileSpreadR[i].x, missileSpreadR[i].y, 2, 2, x, y, xSize, ySize);
            if (isShot && (shieldOn == 0)) {
                state--;
                missileSpreadR[i].state = 0;
                if (state == 0) {
                    score+=2;
                    // enemy death - play sound
                    dynamicPlayer.PlayOneShot(dynamicWav[6]);
                }
            }
        }

    }
    // cycle through movement path
    if (state > 0) {
        pathPlace++;
        if (pathPlace < (movementPathX.size() - 1) && (pathPlace < movementPathY.size() - 1)) {
            x = movementPathX[pathPlace];
            y = movementPathY[pathPlace];
            if ((pathPlace % 100) == 0) {
                Fire();
            }
        }
        else {
            //state = 0; // kills enemy
            resetPosition(); // moves enemy back to other side of screen and keeps moving
        }
    }

    // update missiles fired
    for (auto& m : missilesFired) {
        if (m.y >= 800) {
            m.state = 0;
        }
        if (m.state == 1) {
            //m.y += 3;
            m.Move(playerObject);
        }
    }
}


void standardEnemy::Draw()
{
    for (auto& m : missilesFired) {
        if (m.state == 1) {
            /*glColor3ub(0, 0, 0);*/
            glBegin(GL_QUADS);
            glVertex2i(m.x - m.xSize, m.y - m.ySize);
            glVertex2i(m.x + m.xSize, m.y - m.ySize);
            glVertex2i(m.x + m.xSize, m.y + m.ySize);
            glVertex2i(m.x - m.xSize, m.y + m.ySize);
            glEnd();
        }
    }

    if (state > 0) {
        // just drawing a basic shape for now
        /*glColor3ub(255, 0, 0);
        glBegin(GL_QUADS);

        glVertex2i(x - xSize, y - ySize);
        glVertex2i(x + xSize, y - ySize);
        glVertex2i(x + xSize, y + ySize);
        glVertex2i(x - xSize, y + ySize);

        glEnd();*/
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texId[2]);

        glBegin(GL_QUADS);

        glTexCoord2d(0.0, 0.0);
        glVertex2i(x - 30, y - 30);

        glTexCoord2d(1.0, 0.0);
        glVertex2i(x + 30, y - 30);

        glTexCoord2d(1.0, 1.0);
        glVertex2i(x + 30, y + 30);

        glTexCoord2d(0.0, 1.0);
        glVertex2i(x - 30, y + 30);

        glEnd();

        glDisable(GL_TEXTURE_2D);
    }

}

void standardEnemy::resetPosition()
{
    pathPlace = 0;
}

void magnetEnemy::Move(const int playerX, const int playerY)
{
    // enemy gravitates toward player constantly
    if (y >= playerY) {
        if (x < playerX - 3)
        {
            x += 3;
        }
        else if (playerX + 3 < x)
        {
            x -= 3;
        }
    }
    y += 5; // enemy always travels down
}

// end of enemy and enemy missiles class code

//sound effects part
int load_sound(YsSoundPlayer::SoundData& wav, std::string name_str)
{
    char fName[256];
    strcpy(fName, name_str.c_str());
    if (YSOK != wav.LoadWav(fName))
    {
        printf("Failed to read %s\n", fName);
        return 1;
    }
    return 0;
}

//end sound effects


// for testing
int main() {

    srand(time(NULL));

    //init sound objects
    YsSoundPlayer bgPlayer;
    YsSoundPlayer::SoundData bgMusic;
    std::string bgMusicName = "game_music_red.wav";
    load_sound(bgMusic, bgMusicName);
    bgPlayer.Start();
    bgPlayer.PlayBackground(bgMusic);


    load_sound(dynamicWav[0], "bullet.wav");
    load_sound(dynamicWav[1], "get_life.wav");
    load_sound(dynamicWav[2], "get_bullet.wav");
    load_sound(dynamicWav[3], "boom.wav");
    load_sound(dynamicWav[4], "button.wav");
    load_sound(dynamicWav[5], "hit.wav");
    load_sound(dynamicWav[6], "enemy1_down.wav");
    dynamicPlayer.Start();
    //end sound init


    //int health = 5;

    InGameMenu game;
    GameMenu menu;

    Player player;
    int terminate = 0;

    int p=1;
    const int nObstacle = 5;
    const int nCoins=3;
    int coinscreen=3;
    int obsscreen=5;
    int powercount=0;

    std::vector<Obstacles> obstacle(nObstacle);
    for (int i = 0; i < nObstacle; i++)
        obstacle[i].GenerateObstacle();

    std::vector<Life> life;

    std::vector<Shield> shield;

    std::vector<Coins> coins(nCoins);
    for (int i = 0; i < nCoins; i++)
        coins[i].GenerateCoins();
    std::vector<Weapons> weapons;

    const int nEnemy = 6;
    std::vector<standardEnemy> enemies(nEnemy);
    int enemyCount;

    player.e.Initialize();
    FsOpenWindow(16, 16, 600, 800, 1);

    //FsChangeToProgramDir();
    YsRawPngDecoder tutorial;
    tutorial.Decode("tutorial.png");
    tutorial.Flip();

    png[0].Decode("map.png");
    png[1].Decode("player.png");
    png[2].Decode("enemy.png");
    png[3].Decode("asteroid.png");
    png[4].Decode("shield.png");
    png[5].Decode("health.png");
    png[6].Decode("coin.png");
    png[7].Decode("multi_bullet.png");

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
        png[0].wid,
        png[0].hei,
        0,    // Border width, but not supported and needs to be 0.
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        png[0].rgba);

    glGenTextures(1, &texId[1]);  // Reserve one texture identifier
    glBindTexture(GL_TEXTURE_2D, texId[1]);  // Making the texture identifier current (or bring it to the deck)

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D
    (GL_TEXTURE_2D,
        0,
        GL_RGBA,
        png[1].wid,
        png[1].hei,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        png[1].rgba);

    glGenTextures(1, &texId[2]);  // Reserve one texture identifier
    glBindTexture(GL_TEXTURE_2D, texId[2]);  // Making the texture identifier current (or bring it to the deck)

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D
    (GL_TEXTURE_2D,
        0,
        GL_RGBA,
        png[2].wid,
        png[2].hei,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        png[2].rgba);

    glGenTextures(1, &texId[3]);  // Reserve one texture identifier
    glBindTexture(GL_TEXTURE_2D, texId[3]);  // Making the texture identifier current (or bring it to the deck)

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D
    (GL_TEXTURE_2D,
        0,
        GL_RGBA,
        png[3].wid,
        png[3].hei,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        png[3].rgba);

    glGenTextures(1, &texId[4]);  // Reserve one texture identifier
    glBindTexture(GL_TEXTURE_2D, texId[4]);  // Making the texture identifier current (or bring it to the deck)

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D
    (GL_TEXTURE_2D,
        0,
        GL_RGBA,
        png[4].wid,
        png[4].hei,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        png[4].rgba);

    glGenTextures(1, &texId[5]);  // Reserve one texture identifier
    glBindTexture(GL_TEXTURE_2D, texId[5]);  // Making the texture identifier current (or bring it to the deck)

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D
    (GL_TEXTURE_2D,
        0,
        GL_RGBA,
        png[5].wid,
        png[5].hei,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        png[5].rgba);

    glGenTextures(1, &texId[6]);  // Reserve one texture identifier
    glBindTexture(GL_TEXTURE_2D, texId[6]);  // Making the texture identifier current (or bring it to the deck)

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D
    (GL_TEXTURE_2D,
        0,
        GL_RGBA,
        png[6].wid,
        png[6].hei,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        png[6].rgba);

    glGenTextures(1, &texId[7]);  // Reserve one texture identifier
    glBindTexture(GL_TEXTURE_2D, texId[7]);  // Making the texture identifier current (or bring it to the deck)

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D
    (GL_TEXTURE_2D,
        0,
        GL_RGBA,
        png[7].wid,
        png[7].hei,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        png[7].rgba);
    int level = menu.Background();

    if (level == 1)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (0 < 600 && 0 < 800)
        {
            glRasterPos2d(0, 799);
            glDrawPixels(tutorial.wid, tutorial.hei, GL_RGBA, GL_UNSIGNED_BYTE, tutorial.rgba);
            FsSwapBuffers();

            FsSleep(1500);
        }
        while (terminate == 0)

        {
            FsPollDevice();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            glColor4d(1.0, 1.0, 1.0, 2.0);
            //Transparent
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glEnable(GL_TEXTURE_2D);  // Begin using texture mapping
            //Draw Background
            glBindTexture(GL_TEXTURE_2D, texId[0]);

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

            int key = FsInkey();

            //only can control player when not spawning
            //The controls are just for demoing okay to change
            switch (key)
            {
            case FSKEY_ESC:
                terminate = 1;
                break;
            case FSKEY_SPACE:
                player.Shoot();
                dynamicPlayer.PlayOneShot(dynamicWav[0]);
                break;
            case FSKEY_S://switch missile type
                player.ChangeMissile("Standard");
                break;
            case FSKEY_B://Switch missile type
                player.ChangeMissile("Rapid");
                dynamicPlayer.PlayOneShot(dynamicWav[2]);
                break;
            case FSKEY_P://Switch missile type
                player.ChangeMissile("Spread");
                dynamicPlayer.PlayOneShot(dynamicWav[2]);
                break;
            case FSKEY_H://Simulate a hit
                player.health = player.health - 1;
                dynamicPlayer.PlayOneShot(dynamicWav[5]);
                break;
            }
            if (0 != FsGetKeyState(FSKEY_SPACE) && player.missileType == "Rapid")///Change
            {
                player.Shoot();
                dynamicPlayer.PlayOneShot(dynamicWav[0]);
            }
            if (0 != FsGetKeyState(FSKEY_LEFT))
            {
                player.x -= 10;
                if (player.x <= 0)
                {
                    player.x = 0;
                }
            }
            if (0 != FsGetKeyState(FSKEY_RIGHT))
            {
                player.x += 10;
                if (player.x >= 600)
                {
                    player.x = 600;
                }
            }
            if (0 != FsGetKeyState(FSKEY_UP))
            {
                player.y -= 10;
                if (player.y <= 0)
                {
                    player.y = 0;
                }
            }
            if (0 != FsGetKeyState(FSKEY_DOWN))
            {
                player.y += 10;
                if (player.y >= 800)
                {
                    player.y = 800;
                }
            }

            DrawAllMissiles(); //Ensures all missiles the are shot are drawn independent of what weapon type player is currently holding IE: will draw black missiles still in motion even if player switches to red missiles

            player.Initialize(); //does nothing if player state is 1 or not spawning, else will perform respawn behavior and animation


            while(coinscreen<4)
            {
                Coins newcoins;
                coins.emplace_back(newcoins);
                newcoins.GenerateCoins();
                coinscreen++;
            }
            while(obsscreen<5)
            {
                Obstacles newobstacle;
                obstacle.emplace_back(newobstacle);
                newobstacle.GenerateObstacle();
                obsscreen++;
            }
            while(powercount<2)
            {
                p++;
                if(p%11==0)
                {
                    if(playerLives==4)
                    {

                    }
                    else
                    {
                        Life extralife;
                        extralife.GenerateLife();
                        life.emplace_back(extralife);
                        powercount++;
                    }



                }
                if(p%13==0)
                {

                    Shield newshield;
                    newshield.GenerateShield();
                    shield.emplace_back(newshield);
                    powercount++;

                }
                if(p%8==0)
                {

                    Weapons newweapons;
                    newweapons.GenerateWeapons();
                    newweapons.type=2;
                    weapons.emplace_back(newweapons);
                    powercount++;
                }

                if(p%15==0)
                {
                    Weapons newweapons;
                    newweapons.GenerateWeapons();
                    newweapons.type=3;
                    weapons.emplace_back(newweapons);
                    powercount++;
                }
                if(p%17==0)
                {
                    Weapons newweapons;
                    newweapons.type=1;
                    newweapons.GenerateWeapons();
                    weapons.emplace_back(newweapons);
                    powercount++;
                }


            }

            for (int i = 0; i < obstacle.size(); i++)
                obstacle[i].Draw();

            for (int i = 0; i < life.size(); i++)
                life[i].Draw();

            for (int i = 0; i < shield.size(); i++)
                shield[i].Draw();

            for (int i = 0; i < coins.size(); i++)
                coins[i].Draw();

            for (int i = 0; i < weapons.size(); i++)
                weapons[i].Draw();




            enemyCount = 0;
            for (int i = 0; i < enemies.size(); i++) {
                if (enemies[i].state > 0) {
                    enemyCount++;
                }
            }
            while (enemyCount < 5) {
                standardEnemy newEnemy;
                enemies.emplace_back(newEnemy);
                enemyCount++;
            }

            for (auto& e : enemies)
            {
                e.Move(player, missileStandard, missileRapid, missileSpreadM, missileSpreadL, missileSpreadR, shield);
                e.Draw();
            }


            //Health bar for demoing
            //if (player.health > 0) DrawRect(40, 120, 10 * player.health, 20);

            //draw player always when not mid spawning or dead
            if (player.state == 1) player.Draw();

            //Just for demo: force field around player while spawning
            if (player.spawning) {
                DrawCircle(player.x, player.y - 5, 30, 0);
            }

            //If player health reaches 0 and player is not mid spawn, the player explodes
            if (player.health <= 0 && !player.spawning) {
                player.state = 0;
                if (player.e.state == 0)
                {
                    dynamicPlayer.PlayOneShot(dynamicWav[3]);
                }
                if (true == player.e.Begin(player.x, player.y)) {
                    playerLives--;
                }

            }
            //function calls to conitnue explosion
            if (player.e.state == 1) {
                player.e.Move();
                player.e.Draw();
                //Once explosion reaches a certain point player will begin to respawn
                if (player.e.counter > 50) {
                    player.respawn = true;
                    player.spawning = true;
                }


            }

            for(int i=0; i<shield.size(); i++)
            {
                if(shield[i].state==1 && shield[i].Collide(player)==1&&!player.spawning)
                {
                    //sound-powerup shield
                    dynamicPlayer.PlayOneShot(dynamicWav[1]);
                    shield[i].state=0;
                    shield[i].d=1;
                    powercount--;

                }
                if(shield[i].d==1 && shield[i].n>0)
                {
                    DrawCircle(player.x,player.y-5,30,0);
                    shield[i].n--;
                }
                if(shield[i].n<0)
                    shield[i].d=0;

            }
            for(int i=0; i<obstacle.size(); i++)
            {
                if(obstacle[i].state==1 && obstacle[i].Collide(player)==1&&!player.spawning)
                {
                    if(!shield.empty())
                    {
                        if(shield.back().d==1&&shield.back().n>0)
                            obstacle[i].state=1;
                        else
                        {
                            //sound-obstacle collision
                            dynamicPlayer.PlayOneShot(dynamicWav[5]);
                            obstacle[i].state=0;
                            obsscreen--;
                        }
                    }

                    else
                    {
                      //sound-obstacle collision
                      dynamicPlayer.PlayOneShot(dynamicWav[5]);
                      obstacle[i].state=0;
                      obsscreen--;
                    }
                    if(obstacle[i].state==0)
                        player.health--;
                }
            }

            for (int i = 0; i <life.size(); i++)
            {
                if (life[i].state == 1 && life[i].Collide(player) == 1&&!player.spawning)
                {
                    //sound-acquire life powerup
                    dynamicPlayer.PlayOneShot(dynamicWav[1]);
                    life[i].state = 0;
                    if(playerLives>=4)
                        playerLives=4;
                    else
                    {
                        playerLives++;
                    }

                    powercount--;
                }
            }

            for (int i = 0; i <coins.size(); i++)
            {
                if (coins[i].state == 1 && coins[i].Collide(player) == 1&&!player.spawning)
                {
                    //sound-collected coins
                    dynamicPlayer.PlayOneShot(dynamicWav[1]);
                    coins[i].state = 0;
                    score+=10;
                    coinscreen--;
                }
            }

            for (int i = 0; i <weapons.size(); i++)
            {
                if (weapons[i].state == 1 && weapons[i].Collide(player) == 1&&!player.spawning)
                {
                    //sound-change weapon
                    dynamicPlayer.PlayOneShot(dynamicWav[2]);
                    weapons[i].state = 0;
                    if(weapons[i].type==1)
                        player.ChangeMissile("Standard");
                    if(weapons[i].type==2)
                        player.ChangeMissile("Rapid");
                    if(weapons[i].type==3)
                        player.ChangeMissile("Spread");
                    powercount--;
                }
            }

            int health = player.health;

            char* cstr = new char[player.missileType.length() + 1];
            strcpy(cstr, player.missileType.c_str());

            //printf("Live = %d",playerLives);
            char num[3];
            //itoa(score, num, 10);
            string numString = to_string(score);
            strcpy(num, numString.c_str());
            if (playerLives != 0)
            {
                game.Display(playerLives, num, health, cstr);
                delete[] cstr;
            }
            else
            {
                auto n = game.EndMenu();
                if (n == 0)
                {
                    terminate = 1;
                }
                else
                {
                    playerLives = 3;
                    score = 0;
                }
            }

            //for (int i = 0; i < 7; i++)
            //{
            //    while (YSTRUE == dynamicPlayer.IsPlaying(dynamicWav[i]))
            //    {
            //        dynamicPlayer.KeepPlaying();
            //    }

            //}

            glFlush();
            FsSwapBuffers();
            FsSleep(25);
        }

    }
    else
    {
        FsCloseWindow();
    }
    return 0;
}

//Ensures all missiles are drawn.
//Ensures all missiles are drawn.
void DrawAllMissiles() {

    for (auto& m : missileStandard)
    {
        if (m.IsFlying())
        {
            m.Draw();
        }
    }
    for (auto& m : missileStandard)
    {
        m.Move();
    }

    for (auto& m : missileRapid)
    {
        if (m.IsFlying())
        {
            m.Draw();
        }
    }
    for (auto& m : missileRapid)
    {
        m.Move();
    }

    for (auto& m : missileSpreadM)
    {
        if (m.IsFlying())
        {
            m.Draw();
        }
    }
    for (auto& m : missileSpreadM)
    {
        m.Move();
    }

    for (auto& m : missileSpreadL)
    {
        if (m.IsFlying())
        {
            m.Draw();
        }
    }
    for (auto& m : missileSpreadL)
    {
        m.Move();
    }

    for (auto& m : missileSpreadR)
    {
        if (m.IsFlying())
        {
            m.Draw();
        }
    }
    for (auto& m : missileSpreadR)
    {
        m.Move();
    }
}

void DrawRect(int blockX, int blockY, int blockSizeX, int blockSizeY)
{

    /*glColor3ub(0, 0, 255);*/
    glBegin(GL_QUADS);
    glVertex2i(blockX, blockY + blockSizeY);
    glVertex2i(blockX, blockY - blockSizeY);
    glVertex2i(blockX + blockSizeX, blockY - blockSizeY);
    glVertex2i(blockX + blockSizeX, blockY + blockSizeY);
    glEnd();
}

void DrawCircle(int cx, int cy, int rad, int fill)
{

    const double YS_PI = 3.1415927;

    if (0 != fill)
    {
        glBegin(GL_POLYGON);
    }
    else
    {
        glBegin(GL_LINE_LOOP);
    }

    int i;
    for (i = 0; i < 64; i++)
    {
        double angle = (double)i * YS_PI / 32.0;
        double x = (double)cx + cos(angle) * (double)rad;
        double y = (double)cy + sin(angle) * (double)rad;
        glVertex2d(x, y);
    }

    glEnd();
}

//PlayerFunctions
void Player::Initialize() {

    if (state == 0 && spawning) {
        if (respawn) {//Only start ship here when it is first respawning
            x = 300;
            y = 1000;
            respawn = false;
        }
        if (y > 700) y = y - 10;
        else {
            initializeCount++;
            if (initializeCount > 70)
            {
                state = 1;
                health = 3;
                initializeCount = 0;
                spawning = false;
            }
        }
        Draw();
    }

    else return;
}

void Player::Draw()
{
    /*glColor3ub(0, 0, 255);
    glBegin(GL_QUADS);

    glVertex2i(x - 5, y);
    glVertex2i(x - 5, y - 19);
    glVertex2i(x + 4, y - 19);
    glVertex2i(x + 4, y);

    glVertex2i(x - 9, y + 4);
    glVertex2i(x - 9, y - 5);
    glVertex2i(x + 9, y - 5);
    glVertex2i(x + 9, y + 4);

    glEnd();*/
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId[1]);

    glBegin(GL_QUADS);

    glTexCoord2d(0.0, 0.0);
    glVertex2i(x - 25, y - 25);

    glTexCoord2d(1.0, 0.0);
    glVertex2i(x + 25, y - 25);

    glTexCoord2d(1.0, 1.0);
    glVertex2i(x + 25, y + 25);

    glTexCoord2d(0.0, 1.0);
    glVertex2i(x - 25, y + 25);

    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void Player::Shoot()
{
    if (missileType == "Standard") {

        int i = 0;
        for (auto& m : missileStandard)
        {
            if (true == m.TryShoot(x, y, i))
            {
                //player.PlayOneShot(program.missileSE);
                //++nUsed;
                break;
            }

            i++;
        }
    }

    if (missileType == "Rapid") {
        for (auto& m : missileRapid)
        {
            if (true == m.TryShoot(x, y))
            {
                //player.PlayOneShot(program.missileSE);
                //++nUsed;
                break;
            }
        }
    }

    if (missileType == "Spread") {


        for (auto& m : missileSpreadM)
        {
            if (true == m.TryShoot(x, y))
            {
                //player.PlayOneShot(program.missileSE);
                //++nUsed;
                break;
            }

        }

        for (auto& m : missileSpreadL)
        {
            if (true == m.TryShoot(x, y))
            {
                //player.PlayOneShot(program.missileSE);
                //++nUsed;
                break;
            }

        }

        for (auto& m : missileSpreadR)
        {
            if (true == m.TryShoot(x, y))
            {
                //player.PlayOneShot(program.missileSE);
                //++nUsed;
                break;
            }

        }
    }

}

void Player::ChangeMissile(string newType) {

    missileType = newType;

    if (missileType == "Standard") {

        for (auto& m : missileStandard)
        {
            m.Initialize();
        }

    }

    if (missileType == "Rapid") {

        for (auto& m : missileRapid)
        {
            m.Initialize();
        }

    }

    if (missileType == "Spread") {

        for (auto& m : missileSpreadM)
        {
            m.Initialize();
        }

        for (auto& m : missileSpreadL)
        {
            m.Initialize();
        }

    }

}
