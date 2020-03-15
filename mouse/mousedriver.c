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
#define mousedpcm 63  //dots per centimeter(dpcm) NOT dots per inch(DPI), convert DPI to dpcm by multiplying DPI by 0.393

int mousex=0;
int mousey=0;

void get_mouse_readings(int *mx, int *my){
    *mx=round(mousex/mousedpcm);
    *my=round(mousey/mousedpcm);
}

int mouse(){
    int fd;
    if ((fd = serialOpen ("/dev/input/mouse0", 115200)) < 0)
    {
      fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
      return 1 ;
    }
    while (1) {
        int xchg;
        int xneg;
        int ychg;
        int yneg;
        //printf("datarec: %d %d\n", stat = serialDataAvail(fd), errno);
        for (int i = 0; i < 3; i++) {
            char a = serialGetchar(fd);
            //printf("%c\n", a);
            if (i==0) {
                xneg=a & 0b00010000;
                yneg=a & 0b00100000;
                if ((a & 0b00001000)<=0) {
                    return MOUSE_ALWAYS1_BIT_NOT1;
                }
            }
            if (i == 1) {
                if (xneg==0) {
                    //printf("x=%d\n", a);
                    xchg=a;
                }
                else {
                    //printf("x=-%d\n", 256-a);
                    xchg=-(256-a);
                }
            }
            if (i==2) {
                if (yneg==0) {
                    //printf("y=%d\n", a);
                    ychg=a;
                }
                else {
                    //printf("y=-%d\n", 256-a);
                    ychg=-(256-a);
                }
            }
        }
        mousex+=xchg;
        mousey+=ychg;
        //printf("%d %d\n", ychg, mousey);
    }
}

int mousethread_run(){
    std::thread mousethr(mouse);

    //printf("mouse currently executing\n");

    mousethr.detach();
    
    return 0;
 
}

