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


#include "mainprogram.h"
#include "motors.h"
#include "tcp.h"
#include "countXY.h"
#include "rplidara1/rplidar_driver/lidardriver.h"
#include "mpu6050_driver.h"
#include "mouse/mousedriver.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include <math.h>               //cos, sin
#include <signal.h>             //ctrl_c handling


//#include <sys/time.h>
//#include <time.h>


int main (int, const char **)
{


    signal(SIGINT, ctrl_c_handler);
    sigignore(SIGPIPE);
    char buf[100000];
    char* bufptr;
    bool failsysonline = false;
    //char strsend[1000];
    //int buffer[1024];
    //int lidarindexcount;
    int socket = Connect();
    bufptr = buf;
    sprintf(bufptr, "robotbox");
    bufptr+=strlen(bufptr)+1;
    sprintf(bufptr, "host ONLINE");
    bufptr+=strlen(bufptr)+1;
    send(socket, buf, bufptr-buf, 0);

    //open uart serial device

    float speed;
    int laserdataX[2][360], laserdataY[2][360], Ldeg[2][360];
    int x=0, y=0;
    float heading=0;
    short int currentWriteLData=0; //Put 2 rounds together, 2 rounds make a successful 360 readigns. Int the first round are the odds bad readings, in the second are the even
    mpu6050reads mpu6050data;

    MPU6050_Init();
    bufptr = buf; 
    sprintf(bufptr, "robotbox");
    bufptr+=strlen(bufptr)+1;
    sprintf(bufptr, "mpu6050 gyro ONLINE");
    bufptr+=strlen(bufptr)+1;
    send(socket, buf, bufptr-buf, 0);

    mousethread_run(&heading);
    bufptr = buf; 
    sprintf(bufptr, "robotbox");
    bufptr+=strlen(bufptr)+1;
    sprintf(bufptr, "logitech laser mouse ONLINE");
    bufptr+=strlen(bufptr)+1;
    send(socket, buf, bufptr-buf, 0);


    if (setupLidar()==0){
        bufptr = buf; 
        sprintf(bufptr, "robotbox");
        bufptr+=strlen(bufptr)+1;
        sprintf(bufptr, "lidar ONLINE");
        bufptr+=strlen(bufptr)+1;
        send(socket, buf, bufptr-buf, 0);
    }
    else{
        failsysonline = true;
        bufptr = buf; 
        sprintf(bufptr, "robotbox");
        bufptr+=strlen(bufptr)+1;
        sprintf(bufptr, "lidar OFFLINE");
        bufptr+=strlen(bufptr)+1;
        send(socket, buf, bufptr-buf, 0);
    }

    if (failsysonline==false) {
        bufptr = buf; 
        sprintf(bufptr, "robotbox");
        bufptr+=strlen(bufptr)+1;
        sprintf(bufptr, "systems initailized. all systems ONLINE. proceeding.");
        bufptr+=strlen(bufptr)+1;
        //printf("u");
        send(socket, buf, bufptr-buf, 0);
    }
    else{
        bufptr = buf; 
        sprintf(bufptr, "robotbox");
        bufptr+=strlen(bufptr)+1;
        sprintf(bufptr, "some systems detected OFFLINE. program execution may fail.");
        bufptr+=strlen(bufptr)+1;
        //printf("u");
        send(socket, buf, bufptr-buf, 0);
    }

    while (true) {
        //speed = static_cast<int>(getLidarFreq());
        speed = getLidarFreq();
        //send speed to computer
        bufptr = buf;
        sprintf(bufptr, "rpm");
        bufptr+=strlen(bufptr)+1;
        sprintf(bufptr, "%f", speed);
        bufptr+=strlen(bufptr)+1;
        send(socket, buf, bufptr-buf, 0);

        //process lidar "garphics" readings
        getLidarScan(Ldeg[currentWriteLData]);
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
        send(socket, buf, bufptr-buf, 0);

        //countXY(laserdataX[currentWriteLData == 0 ? 0 : 1], laserdataY[currentWriteLData == 0 ? 0 : 1], Ldeg[currentWriteLData == 0 ? 0 : 1], laserdataX[currentWriteLData == 0 ? 1 : 0], laserdataY[currentWriteLData == 0 ? 1 : 0], Ldeg[currentWriteLData == 0 ? 1 : 0], &x, &y, &heading, socket, buf, bufptr);
        readMPU6050(&mpu6050data);
        if (isRobotMoving()) {
            get_mouse_readings(&x, &y);
            heading+=mpu6050data.Gyro_z;
        }

        bufptr = buf;
        sprintf(bufptr, "xyupdate");
        bufptr+=strlen(bufptr)+1;
        sprintf(bufptr, "%d", x);
        bufptr+=strlen(bufptr)+1;
        sprintf(bufptr, "%d", y);
        bufptr+=strlen(bufptr)+1;
        sprintf(bufptr, "%f", heading);
        bufptr+=strlen(bufptr)+1;
        send(socket, buf, bufptr-buf, 0);

        currentWriteLData = currentWriteLData == 0 ? 1 : 0;
        if (errno == EPIPE || errno == ENOBUFS) {
            printf("epipe\n");
            //digitalWrite (23, LOW);
            closeLidar();
            Mhatreset();
            printf("done, exiting\n");
            exit(0);
        }
        checkforrecv(socket);
    }
    return 0;
}



void ctrl_c_handler(int s)
{
    closeLidar();
    Mhatreset();
    std::cout << "exit successful!" << s << std::endl;
    exit(0);
}





