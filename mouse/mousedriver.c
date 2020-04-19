/* 
* lidar project
* 
* Copyright (C) 2017-2020 Gabor Szita
* 
* This file is part of lidar project.
*
* Lidar project is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 3 as published by
* the Free Software Foundation.
*
* Lidar project is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with lidar project.  If not, see https://www.gnu.org/licenses/ 
*/ 

#include <wiringSerial.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <thread>
#include <math.h>
#include "mousedriver.h"
#define mousedpcm 441  //dots per centimeter(dpcm) NOT dots per inch(DPI), convert DPI to dpcm by multiplying DPI by 0.393
#define PI 3.14159265359

int mousex=0; //global mouse x and y change
int mousey=0;

void get_mouse_readings(int *mx, int *my){ //function which shows mouse reading data
    *mx=round(mousex/mousedpcm);
    *my=round(mousey/mousedpcm);
}

int mouse(float *heading){
    int fd;
    if ((fd = serialOpen ("/dev/input/mouse0", 115200)) < 0) //open mouse device
    {
      fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
      return 1 ;
    }
    while (1) {
        int xchg; //mouse x change
        int xneg; //mouse x negative number indicator
        int ychg; //mouse y change
        int yneg; //mouse y negative number indicator
        //printf("datarec: %d %d\n", stat = serialDataAvail(fd), errno);
        for (int i = 0; i < 3; i++) {
            char a = serialGetchar(fd); //get a character from mouse
            //printf("%c\n", a);
            if (i==0) {
                xneg=a & 0b00010000; //set xneg variable if mouse x change is negative
                yneg=a & 0b00100000; //set yneg variable if mouse y change is negative
                if ((a & 0b00001000)<=0) { //this bit should always be 1, if it is 0 then the thread should exit
                    return MOUSE_ALWAYS1_BIT_NOT1;
                }
            }
            if (i == 1) { //this byte contains the mouse x change
                if (xneg==0) {
                    //printf("x=%d\n", a);
                    xchg=a;
                }
                else {
                    //printf("x=-%d\n", 256-a);
                    xchg=-(256-a);
                }
            }
            if (i==2) { //this byte contains the mouse y change
                if (yneg==0) { //my mouse is backwards
                    //printf("y=%d\n", a);
                    ychg=a;
                }
                else {
                    //printf("y=-%d\n", 256-a);
                    ychg=-(256-a);
                }
            }
        }
        int xchgcorr = xchg * cos(-*heading * PI / 180) - ychg * sin(-*heading * PI / 180);
        int ychgcorr = ychg * cos(-*heading * PI / 180) + xchg * sin(-*heading * PI / 180);
        //printf("%f\n", *heading);
        mousex+=xchgcorr; //add x mouse change to global mouse x change variable
        mousey+=ychgcorr; //add y mouse change to global mouse y change variable
        //printf("%d %d\n", ychg, mousey);
    }
}

int mousethread_run(float *heading){
    std::thread mousethr(mouse, heading); //start mouse thread

    //printf("mouse currently executing\n");

    mousethr.detach(); //allow mouse thread to run by itself, because the program will continue on its own process
    
    return 0;
 
}

