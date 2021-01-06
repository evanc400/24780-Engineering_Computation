//
//  ps3-2.cpp
//  ps3-2
//
//  Created by Evan Cohen on 9/21/20.
//

#include <stdio.h>
#include "fssimplewindow.h"
#include <math.h>
#include <iostream>

void DrawCircle(int cx,int cy,int rad,int fill);

int main(void){

    const double PI=3.141592;

    double th_w, th_c, v_w, v_a;

  	th_w = 220 * PI / 180.0;
  	v_w = 15;
  	th_c = 235 * PI / 180.0;
  	v_a = 125;

    //Initial Formulation
  	double th_rw = th_w + PI - th_c;
  	double th_wca = asin((-v_w * sin(th_rw)) / v_a);
  	double th_h = th_c + th_wca;
  	double v_g = v_a * cos(th_wca) + v_w * cos(th_rw);
  	double th_h_degrees = th_h * 180.0 / PI;

    //Formulation for plotting
    double v_gx=3*v_g*cos(th_c-PI/2);
    double v_gy=3*v_g*sin(th_c-PI/2);

    double v_wx=3*v_w*cos(th_w-PI/2);
    double v_wy=3*v_w*sin(th_w-PI/2);

    double v_ax=3*v_a*cos(th_h-PI/2);
    double v_ay=3*v_a*sin(th_h-PI/2);

    printf("\n v_gx: %f \n",v_gx);
    printf("v_gy: %f \n \n",v_gy);


    printf("\n Heading angle: %f \n",th_h_degrees);
    printf("Expected ground speed: %f \n \n",v_g);

    FsOpenWindow(16,16,800,600,0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    //v_g
    glColor3f(0.0,0.0,1.0);
    glBegin(GL_LINES);
    glVertex2i(400,300);
    glVertex2i(400+v_gx,300+v_gy);
    glEnd();

    //v_w
    glColor3f(1.0,0.0,0.0);
    glBegin(GL_LINES);
    glVertex2i(400,300);
    glVertex2i(400+v_wx,300+v_wy);
    glEnd();

    //v_a
    glColor3f(0.0,1.0,0.0);
    glBegin(GL_LINES);
    glVertex2i(400,300);
    glVertex2i(400+v_ax,300+v_ay);
    glEnd();

    //Point
    glColor3f(0.0,0.0,1.0);
    glBegin(GL_POINTS);
    glVertex2i(780,580);
    //glVertex2i(v_gx,v_gy);
    glEnd();

    //Circle
    glColor3f(0.0,0.0,0.0);
    DrawCircle(400,300,100,0);

    //Flush
    glFlush();
    FsPollDevice();



    while(FSKEY_NULL==FsInkey()){
        FsPollDevice();
        FsSleep(25);

    }

    return 0;
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
