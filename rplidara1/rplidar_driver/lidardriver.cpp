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
#include <string.h>
#include "lidardriver.h"


//#define SCAN_FILE_DEBUG
//#define numnodes 360
#define ptcorrectdist 40

int LidarController::setupLidar(){
    u_result res;
    lidar = RPlidarDriver::CreateDriver();
    res = lidar->connect("/dev/ttyUSB0", 115200);
    if (IS_FAIL(res)) {
        printf("CRITICAL_CANNOT_CONNECT_TO_LIDAR\n");
        return 1;
    }
    lidar->startMotor();
    res = lidar->getAllSupportedScanModes(scanModes);
    if (IS_FAIL(res)) {
        printf("CRITICAL_CANNOT_GET_SUPPORTED_SCAN_MODES\n");
        return 1;
    }
    lidar->startScanExpress(false, scanModes[0].id);
    return 0;
}

int LidarController::closeLidar(){
    //lidar->stopMotor();
    lidar->disconnect();
    RPlidarDriver::DisposeDriver(lidar);
    return 0;
}

float LidarController::getLidarFreq(){
    float freq=0;
    lidar->getFrequency(scanModes[0], numnodes, freq);
    return freq;
}

int LidarController::getLidarScan(int vangles[360]){
    //float vangles[360];
    memset(vangles, 0, sizeof(int)*360);
    int flagangles[360];
    memset(flagangles, 0, sizeof(int)*360);

    // grab scan data
    
    u_result res = lidar->grabScanDataHq(nodes, numnodes);
    if (IS_FAIL(res))
    {
        printf("CRITICAL_FAILED_TO_GET_SCAN%d\n", res);
    }

    // process scan data
    #ifdef SCAN_FILE_DEBUG
    FILE *f;
    f = fopen("readingoutput.txt", "w");
    #endif
    for (unsigned int i=0; i<numnodes; i++) {
        float angle_in_degrees = nodes[i].angle_z_q14 * 90.f / (1 << 14);
        //int angle_in_degrees = (nodes[i].angle_z_q14 * 90) >> 14;
        //float distance_in_meters = nodes[i].dist_mm_q2 / 1000.f / (1 << 2);
        int quality = nodes[i].quality;
        int flag = nodes[i].flag;
        #ifdef SCAN_FILE_DEBUG
        fprintf(f, "%d angle: %.6f, dist: %d quality: %d flag: %d\n", i, angle_in_degrees, nodes[i].dist_mm_q2, quality, flag);
        #endif
        if (flag==0 && quality>=60) {
            
            int angleint = -(static_cast<int>(angle_in_degrees));
            if (angleint<0) {
                angleint += 360;
            }
            vangles[angleint] = nodes[i].dist_mm_q2 / 40;
            flagangles[angleint] = 1;
        }
    }
    
    for (int i=0; i<360; i++) {
        int closestvalueaft = 0;
        int closestvaluebef = 0;
        const int vcccountlimit=8;
        if (flagangles[i]==0) {
            //printf("yes\n");
            int vcccount=0;
            for (int x=i+1; x!=i-1; x++) {
                if (x==360) {
                    x=0;
                }
                if (flagangles[x] == 1) {
                    closestvalueaft = vangles[x];
                    break;
                }
                vcccount++;
                if (vcccount>=vcccountlimit) {
                    break;
                }
            }
            //printf("passclosest");
            vcccount=0;
            for (int x=i-1; x!=i+1; x--) {
                if (x==-1) {
                    x=359;
                }
                if (flagangles[x] == 1) {
                    closestvaluebef = vangles[x];
                    break;
                }
                if (vcccount>=vcccountlimit) {
                    break;
                }
            }
            if (closestvalueaft!=0 && closestvaluebef!=0) {
                vangles[i]=(closestvalueaft+closestvaluebef)/2;
                //printf("%d %d\n", closestvalueaft, closestvaluebef);
            }
        }
        //printf("%d\n", i);
    }
    /*for (int i=0; i<358; i++) {
        int sub = vangles[i+2]-vangles[i];
        bool subtr;
        if (sub > -ptcorrectdist && sub < ptcorrectdist) {
            subtr = true;
        }
        else{
            subtr = false;
        }
        if (vangles[i]!=0 && vangles[i+1]==0 && vangles[i+2]!=0 && subtr) {
            vangles[i+1] = (vangles[i]+vangles[i+2])/2;
        }
    }*/
    #ifdef SCAN_FILE_DEBUG
    fclose(f);
    f = fopen("readingouputSORTED.txt", "w");
    for (int i=0; i<360; i++) {
        fprintf(f, "angle: %d, dist: %d\n", i, vangles[i]);
    }
    fclose(f);
    #endif
    return 1;
}

