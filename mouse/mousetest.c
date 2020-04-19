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
//compile with: g++ -o mousetest mousetest.c -lwiringPi -pthread
//be sure to have a mouse connected to the raspberry pi

#include <wiringSerial.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <thread>

int mouse(){
    int fd;
    if ((fd = serialOpen ("/dev/input/mouse0", 115200)) < 0)
    {
      fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
      return 1 ;
    }
    while (1) {
        int xneg;
        int yneg;
        int stat;
        printf("datarec: %d %d\n", stat = serialDataAvail(fd), errno);
        if (stat==0) {
            printf("no data to recieve\n");
            continue;
        }
        for (int i = 0; i < 3; i++) {
            char a = serialGetchar(fd);
            //printf("%c\n", a);
            if (i==0) {
                xneg=a & 0b00010000;
                yneg=a & 0b00100000;
                if ((a & 0b00001000)<=0) {
                    return 3;
                }
            }
            if (i == 1) {
                if (xneg==0) {
                    printf("x=%d\n", a);
                }
                else {
                    printf("x=-%d\n", 256-a);
                }
            }
            if (i==2) {
                if (yneg==0) {
                    printf("y=%d\n", a);
                }
                else {
                    printf("y=-%d\n", 256-a);
                }
            }
        }
    }
}

int main(){
    std::thread mousethr(mouse);

    printf("mouse currently executing\n");
    //std::terminate();
    mousethr.join();
    
    return 0;
 
}
