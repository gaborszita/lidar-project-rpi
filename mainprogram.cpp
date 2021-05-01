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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <math.h>               //cos, sin
#include <signal.h>             //ctrl_c handling

#include "mainprogram.h"
#include "motors.h"
#include "tcp.h"
#include "countXY.h"
#include "rplidara1/rplidar_driver/lidardriver.h"
#include "mpu6050_driver.h"
#include "mouse/mousedriver.h"
#include "navigation.h"

Navigation navigation;


int main (int, const char **)
{
    signal(SIGINT, ctrl_c_handler);
    sigignore(SIGPIPE);
    char buf[100000];
    char* bufptr;
    TCP tcp;
    tcp.Connect();
    bufptr = buf;
    sprintf(bufptr, "robotbox");
    bufptr+=strlen(bufptr)+1;
    sprintf(bufptr, "host ONLINE");
    bufptr+=strlen(bufptr)+1;
    tcp.sendData(buf, bufptr-buf);

    float speed;
    int laserdataX[2][360], laserdataY[2][360], Ldeg[2][360];
    short int currentWriteLData=0;

    InitData initData;
    navigation.InitializeDevices(&initData);
    //lidar
    if (initData.lidarStatus==0){
        tcp.sendString("robotbox", "lidar ONLINE", buf, bufptr);
    }
    else{
        tcp.sendString("robotbox", "lidar OFFLINE or PROBLEMS PRESENT with lidar", buf, bufptr);
    }
    //mouse
    if (initData.mouseStatus==0){
        tcp.sendString("robotbox", "mouse ONLINE", buf, bufptr);
    }
    else{
        tcp.sendString("robotbox", "mouse OFFLINE or PROBLEMS PRESENT with mouse", buf, bufptr);
    }
    //gyro
    if (initData.mouseStatus==0){
        tcp.sendString("robotbox", "gyro ONLINE", buf, bufptr);
    }
    else{
        tcp.sendString("robotbox", "gyro OFFLINE or PROBLEMS PRESENT with gyro", buf, bufptr);
    }
    //motors
    if (initData.motorsStatus==0){
        tcp.sendString("robotbox", "motors ONLINE", buf, bufptr);
    }
    else{
        tcp.sendString("robotbox", "motors OFFLINE or PROBLEMS PRESENT with motors", buf, bufptr);
    }

    if (initData.lidarStatus==0 && initData.mouseStatus==0 && initData.gyroStatus==0 && initData.motorsStatus==0) {
        tcp.sendString("robotbox", "systems initailized. all systems ONLINE. proceeding.", buf, bufptr);
    }
    else{
        tcp.sendString("robotbox", "some systems detected OFFLINE. program execution may fail.", buf, bufptr);
    }

    while (true) {
        //send speed to computer
        speed = navigation.lidarDevice.getLidarFreq();
        bufptr = buf;
        sprintf(bufptr, "rpm");
        bufptr+=strlen(bufptr)+1;
        sprintf(bufptr, "%f", speed*60);
        bufptr+=strlen(bufptr)+1;
        tcp.sendData(buf, bufptr-buf);

        //process lidar "garphics" readings
        navigation.lidarDevice.getLidarScan(Ldeg[currentWriteLData]);
        for (int i=0; i<360; i++) {
            float phi_rad = (float)i * PI / 180;
            int r = Ldeg[currentWriteLData][i];
            float xp = (r * sin(phi_rad)); 
            float yp = (r * cos(phi_rad));
            laserdataX[currentWriteLData][i] = xp;
            laserdataY[currentWriteLData][i] = yp;
        }

        bufptr = buf;
        sprintf(bufptr, "lidardrawing");
        bufptr+=strlen(bufptr)+1;
        for (int i=0; i<360; i++) {
            sprintf(bufptr, "%d", laserdataX[currentWriteLData][i]);
            bufptr+=strlen(bufptr)+1;
        }
        for (int i=0; i<360; i++) {
            sprintf(bufptr, "%d", laserdataY[currentWriteLData][i]);
            bufptr+=strlen(bufptr)+1;
        }
        int count0 = 0;
        for (int i=0; i<bufptr-buf; i++) {
            if (buf[i]=='\0') {
                count0++;
            }
        }
        tcp.sendData(buf, bufptr-buf);

        //countXY(laserdataX[currentWriteLData == 0 ? 0 : 1], laserdataY[currentWriteLData == 0 ? 0 : 1], Ldeg[currentWriteLData == 0 ? 0 : 1], laserdataX[currentWriteLData == 0 ? 1 : 0], laserdataY[currentWriteLData == 0 ? 1 : 0], Ldeg[currentWriteLData == 0 ? 1 : 0], &x, &y, &heading, socket, buf, bufptr);
        /*navigation.update();


        bufptr = buf;
        sprintf(bufptr, "xyupdate");
        bufptr+=strlen(bufptr)+1;
        sprintf(bufptr, "%d", navigation.getX());
        bufptr+=strlen(bufptr)+1;
        sprintf(bufptr, "%d", navigation.getY());
        bufptr+=strlen(bufptr)+1;
        sprintf(bufptr, "%f", navigation.getHeading());
        bufptr+=strlen(bufptr)+1;
        tcp.sendData(buf, bufptr-buf);*/

        currentWriteLData = currentWriteLData == 0 ? 1 : 0;
        if (errno == EPIPE || errno == ENOBUFS) {
            printf("epipe\n");
            navigation.CloseDevices();
            printf("done, exiting\n");
            exit(0);
        }
        tcp.checkforrecv(&navigation);
    }
    return 0;
}



void ctrl_c_handler(int s)
{
    navigation.CloseDevices();
    std::cout << "exit successful!" << s << std::endl;
    exit(0);
}





