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


// robotMy7.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#pragma warning( disable : 26451 6031)



#include "countXY.h"
#include "gyro/mpu6050_driver.h"

#define PI 3.14159265359
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>

inline void rot(int* x, int* y, float ang, int n);
int lastms = 0;

void countXY(int LXold[ARR_LEN], int LYold[ARR_LEN], int Lold[ARR_LEN], int LXnew[ARR_LEN], int LYnew[ARR_LEN], int Lnew[ARR_LEN], int* currentX, int* currentY, float* currentDEG, int socket, char buf[], char* bufptr)
{
	int corners1[ARR_LEN];
	int corners2[ARR_LEN];
	int x1[ARR_LEN];
	int y1[ARR_LEN];
	int x2[ARR_LEN];
	int y2[ARR_LEN];
	int x3[ARR_LEN];
	int y3[ARR_LEN];
	int pairs[ARR_LEN][2];
	int n1, n2, num_pairs;
	int m, m0, mold;
	int xshfttot, yshfttot;
    int ptpaird=3;
	float rottot;
    struct mpu6050reads mpu6050data;
    int gyrochange;

	memset(x1, 0, sizeof(x1));
	memset(y1, 0, sizeof(y1));
	memset(x2, 0, sizeof(x2));
	memset(y2, 0, sizeof(y2));

    //printf("%f\n", mpu6050data.Gyro_z);
    readMPU6050(&mpu6050data);  //read the gyro readings
    //printf("%f\n", mpu6050data.Gyro_z);

	n1 = split_merge(Lold, LXold, LYold, corners1);
	n2 = split_merge(Lnew, LXnew, LYnew, corners2);

    if (lastms==0) {
        struct timespec spec;
        long ms;

        clock_gettime(CLOCK_REALTIME, &spec);

        //s  = spec.tv_sec;
        ms = round(spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds
        if (ms > 999) {
            //s++;
            ms = 0;
        }
        lastms=ms;
    }

    // time stamp stuff
    if (lastms!=0) {
        struct timespec spec;
        long ms;

        clock_gettime(CLOCK_REALTIME, &spec);

        //s  = spec.tv_sec;
        ms = round(spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds
        if (ms > 999) {
            //s++;
            ms = 0;
        }
        //end tt processing
        int mschange = ms-lastms;
        //printf("defag %f\n", mpu6050data.Gyro_z);
        gyrochange = (mpu6050data.Gyro_z)*(mschange/1000);
        //printf("ms %d\n", mschange);
        //printf("g %d\n", gyrochange);

        //ptpaird += gyrochange; 
        lastms=ms;
    }

	num_pairs = pointpairs(LXold, LYold, corners1, n1, LXnew, LYnew, corners2, n2, pairs, ptpaird);

    /*if (num_pairs==0) {
        printf("c1: %d\n", n1);
        printf("c2: %d\n", n2);
        FILE *f;
        f = fopen("ctdbg1111.txt", "w");
        for (int i=0; i<360; i++) {
            fprintf(f, "%d %d %d %d\n", i, LXnew[i], LYnew[i], Lnew[i]);
        }
        for (int i=0; i<360; i++) {
            fprintf(f, "%d %d %d %d\n", i, LXold[i], LYold[i], Lold[i]);
        }*/
        /*fprintf(f, "LX\n");
        for (int i=0; i<360; i++) {
            fprintf(f, "%d %d\n", LXold[i], LXnew[i]);
        }
        fprintf(f, "LY\n");
        for (int i=0; i<360; i++) {
            fprintf(f, "%d %d\n", LYold[i], LYnew[i]);
        }
        fprintf(f, "L\n");
        for (int i=0; i<360; i++) {
            fprintf(f, "%d %d\n", Lold[i], Lnew[i]);
        }
        fprintf(f, "LXnew\n");
        for (int i=0; i<360; i++) {
            fprintf(f, "%d\n", LXnew[i]);
        }
        fprintf(f, "LYnew\n");
        for (int i=0; i<360; i++) {
            fprintf(f, "%d\n", LYnew[i]);
        }
        fprintf(f, "Lnew\n");
        for (int i=0; i<360; i++) {
            fprintf(f, "%d\n", Lnew[i]);
        }*/
        //fclose(f);
    //}

    bufptr = buf;
    sprintf(bufptr, "xydebugcorners");
    bufptr+=strlen(bufptr)+1;
    sprintf(bufptr, "%d", num_pairs);
    bufptr+=strlen(bufptr)+1;
    for (int i=0; i<num_pairs; i++) {
        sprintf(bufptr, "%d", LXnew[pairs[i][0]]);
        bufptr+=strlen(bufptr)+1;
        sprintf(bufptr, "%d", LYnew[pairs[i][0]]);
        bufptr+=strlen(bufptr)+1;
    }
    for (int i=0; i<num_pairs; i++) {
        sprintf(bufptr, "%d", LXnew[pairs[i][1]]);
        bufptr+=strlen(bufptr)+1;
        sprintf(bufptr, "%d", LYnew[pairs[i][1]]);
        bufptr+=strlen(bufptr)+1;
    }
    send(socket, buf, bufptr - buf, 0);

	//num_pairs = 14;
	for (int k1 = 0; k1 < num_pairs; k1++)
	{
		x1[k1] = LXold[pairs[k1][0]];
		y1[k1] = LYold[pairs[k1][0]];
		x2[k1] = LXnew[pairs[k1][1]];
		y2[k1] = LYnew[pairs[k1][1]];
	}
    //rot(LXold, LYold, mpu6050data.Gyro_z, 360);
    //rot(LXnew, LYnew, mpu6050data.Gyro_z, 360);
    //rottot+=mpu6050data.Gyro_z;

	m0 = mismatch(x1, y1, x2, y2, num_pairs);
    
    
	xshfttot = 0;
	yshfttot = 0;
	rottot = 0;
	m = m0;
	memcpy(x3, x2, ARR_LEN * sizeof(int));
	memcpy(y3, y2, ARR_LEN * sizeof(int));
    testmove(x1, y1, x3, y3, 0, 0, gyrochange, &m, &xshfttot, &yshfttot, &rottot, num_pairs);
    //printf("%f %d %d\n", mpu6050data.Gyro_z, m, m0);
    while (1)
	{
		//printf("m: %d\n", m);
		mold = m;
		testmove(x1, y1, x3, y3, 1, 0, 0, &m, &xshfttot, &yshfttot, &rottot, num_pairs);
		testmove(x1, y1, x3, y3, 0, 1, 0, &m, &xshfttot, &yshfttot, &rottot, num_pairs);

		testmove(x1, y1, x3, y3, -1, 0, 0, &m, &xshfttot, &yshfttot, &rottot, num_pairs);

		testmove(x1, y1, x3, y3, 0, -1, 0, &m, &xshfttot, &yshfttot, &rottot, num_pairs);
		if (m < mold) continue;

		testmove(x1, y1, x3, y3, 0, 0, (float)0.1, &m, &xshfttot, &yshfttot, &rottot, num_pairs);
		if (m < mold) continue;

		testmove(x1, y1, x3, y3, 0, 0, (float)-0.1, &m, &xshfttot, &yshfttot, &rottot, num_pairs);
		if (m < mold) continue;

		if (mold <= m){
            break;
        }
	}
	//printf("xshft: %d, yshft: %d, rottot: %f\n", xshfttot, yshfttot, rottot);
	*currentX += xshfttot;
	*currentY += yshfttot;
	*currentDEG += rottot;
    if (rottot < 0) rottot = 360 - rottot;
}


int pointpairs(int LXold[], int LYold[], int corners1[], int corners1_len, int LXnew[], int LYnew[], int corners2[], int corners2_len, int pairs[ARR_LEN][2], const int maxlength)
{
	//int pairs[ARR_LEN][2];
	int pairsindex = 0;
	//const int maxlength = 6;
	memset(pairs, 0, sizeof(int) * ARR_LEN * 2);
	for (int k1 = 0; k1 < corners1_len; k1++)
	{
		int minimum = maxlength + 1;
		int minindex = 0;
		for (int k2 = 0; k2 < corners2_len; k2++)
		{
			int dX = LXnew[corners2[k2]] - LXold[corners1[k1]];
			int dY = LYnew[corners2[k2]] - LYold[corners1[k1]];
			int d = (int)sqrt(dX * dX + dY * dY);
			//printf("%d\n", d);
			if (d < minimum)
			{
				minimum = d;
				minindex = k2;
			}
		}
		if (minimum <= maxlength)
		{
			pairs[pairsindex][0] = corners1[k1];
			pairs[pairsindex][1] = corners2[minindex];
			pairsindex++;
		}

	}
	return pairsindex;
}

int split_merge(int Lszog[], int LX[], int LY[], int corners[])
{
	int mainarc[ARR_LEN][2];
	int mainarc_len = arc(Lszog, LX, LY, mainarc);
	memset(corners, 0, sizeof(int) * ARR_LEN);
	int lencorners = 0;
	int groupindex = 0;
	while (groupindex < mainarc_len)
	{
		int containnot0 = 0;
		for (int k1 = mainarc[groupindex][0] + 1; k1 < mainarc[groupindex][1] - 1; k1++)
		{
			if (LX[k1] != 0 || LY[k1] != 0)
			{
				containnot0 = 1;
				break;
			}
		}

		if (containnot0 == 1)
		{
			lencorners = arcprocess(LX, LY, mainarc, corners, lencorners, groupindex);
		}
		groupindex++;
	}
	return lencorners;
}

int arcprocess(int LX[], int LY[], int arcgroups[ARR_LEN][2], int corner[], int lencorner, int groupindex)
{
	int d = 20;
	int newcorners[ARR_LEN], currentarcgroup[ARR_LEN];
	int newcorners_len = arcgroups[groupindex][1] - arcgroups[groupindex][0] + 1;
	int currentarcgroup_len = 2;
	int lennewcorners = 0;
	memset(newcorners, 0, sizeof(newcorners));
	memset(currentarcgroup, 0, sizeof(currentarcgroup));
	currentarcgroup[0] = arcgroups[groupindex][0];
	currentarcgroup[1] = arcgroups[groupindex][1];
	int k1;

	while (1)
	{
		int x1 = LX[currentarcgroup[0]];
		int y1 = LY[currentarcgroup[0]];
		int x2 = LX[currentarcgroup[1]];
		int y2 = LY[currentarcgroup[1]];
		float maxoffset = 0;
		int maxindex = 0;
		for (k1 = currentarcgroup[0] + 1; k1 < currentarcgroup[1] - 1; k1++)
		{
			if (LY[k1] == 0 && LX[k1] == 0) continue;
			float offset = (float)abs((y2 - y1) * LX[k1] - (x2 - x1) * LY[k1] + x2 * y1 - y2 * x1) / (float)sqrt((y2 - y1) * (y2 - y1) + (x2 - x1) * (x2 - x1));
			if (offset > maxoffset)
			{
				maxoffset = offset;
				maxindex = k1;
			}
		}
		if (maxoffset > d)
		{
			int i;
			// currentarcgroup=[currentarcgroup(1), maxindex, currentarcgroup(2:end)];
			for (i = currentarcgroup_len; i >= 2; i--) currentarcgroup[i] = currentarcgroup[i - 1];
			currentarcgroup[1] = maxindex;
			currentarcgroup_len++;
		}
		else if (currentarcgroup[1] == arcgroups[groupindex][1])
		{
			//lennewcorners++;
			//newcorners[lennewcorners - 1] = currentarcgroup[1];
			newcorners_len = lennewcorners;
			break;
		}
		else
		{
			int i;
			if (currentarcgroup[0]!= 0) {
				lennewcorners++;
				newcorners[lennewcorners - 1] = currentarcgroup[0];
			}

			//currentarcgroup=currentarcgroup(2:end);
			for (i = 0; i < currentarcgroup_len - 1; i++) currentarcgroup[i] = currentarcgroup[i + 1];
			currentarcgroup_len--;
		}
	}

	if (corner[lencorner - 1] != 0 && corner[lencorner - 1] == newcorners[0])
	{
		//corner(lencorner: lencorner+length(newcorners(2:end))-1)=newcorners(2:end);
		for (int i = 0; i < newcorners_len - 1; i++)
		{
			corner[lencorner + i] = newcorners[1 + i];
		}

		//lencorner=lencorner+length(newcorners(2:end));
		lencorner += newcorners_len - 1;
	}
	else
	{
		//corner(lencorner:lencorner+length(newcorners(1:end))-1)=newcorners(1:end);
		for (int i = 0; i < newcorners_len; i++)
		{
			corner[lencorner + i] = newcorners[i];
		}
		//lencorner=lencorner+length(newcorners(1:end));
		lencorner += newcorners_len;
	}
	return lencorner;
}

int arc(int Lszog[], int LX[], int LY[], int validarc[ARR_LEN][2])
{
	const float N = 50;
	const float O = (float)PI / 180;
	int R;
	int arcgroups[ARR_LEN];
	memset(arcgroups, 0, sizeof(arcgroups));
	memset(validarc, 0, ARR_LEN * 2);
	int startindex = 0;
	arcgroups[0] = startindex;
	int arcgroupsindex = 1;
	for (int k1 = startindex + 1; k1 < ARR_LEN; k1++)
	{
		float S, THR;
		if ((LX[k1] == 0 && LY[k1] == 0) || (LX[k1 - 1] == 0 && LY[k1 - 1] == 0)) continue;

		if (Lszog[k1] < Lszog[k1 - 1]) R = Lszog[k1];
		else R = Lszog[k1 - 1];

		S = R * O;
		THR = N * S;
		int point_dist = (int)round(sqrt(Lszog[k1] * Lszog[k1] + Lszog[k1 - 1] * Lszog[k1 - 1] - 2 * Lszog[k1] * Lszog[k1 - 1]  * cos(PI / 180.0)));
		if (point_dist > THR)
		{
			arcgroups[arcgroupsindex] = startindex + k1;
			arcgroupsindex++;
		}
	}
	arcgroups[arcgroupsindex] = 359;

	// cancel clusters which has few points
	//validarc=zeros(length(arcgroups)-1, 2);
	int validarc_len = arcgroupsindex;
	int validarcindex = 0;
	for (int k1 = 0; k1 < validarc_len; k1++)
	{
		if (arcgroups[k1 + 1] - arcgroups[k1] >= 3 && (LX[k1] != 0 || LY[k1] != 0) && (LX[k1 + 1] != 0 || LY[k1 + 1] != 0))
		{
			validarc[validarcindex][0] = arcgroups[k1];
			validarc[validarcindex][1] = arcgroups[k1 + 1];
			validarcindex++;
		}
	}
	return validarcindex;
}


int mismatch(int x1[], int y1[], int x2[], int y2[], int n)
{
	int mism = 0;
	int k3 = 0;
	for (int k1=1;k1<n;k1++){
		mism = (int)floor(mism + ((x1[k1] - x2[k1]) * (x1[k1] - x2[k1]) + (y1[k1] - y2[k1]) * (y1[k1] - y2[k1])));
		k3 = k3 + 1;
	}
	return mism;
}

inline void shft(int* x, int* y, int xshft, int yshft, int n)
{
	//*x += xshft;
	//*y += yshft;
	for (int i = 0; i < n; i++) {
		x[i] += xshft;
		y[i] += yshft;
	}
}

inline void rot(int* x, int* y, float ang, int n)
{
	//*x = round(*x * cos(angrad) - *y * sin(angrad));
	//*y = round(*x * sin(angrad) + *y * cos(angrad));
	float angrad = ang * (float)PI / 180;
	for (int i = 0; i < n; i++) {
		x[i] = (int)round(x[i] * cos(angrad) - y[i] * sin(angrad));
		y[i] = (int)round(x[i] * sin(angrad) + y[i] * cos(angrad));
	}
}


void testmove(int x1[], int y1[], int x2[], int y2[], int xshft, int yshft, float ang, int* m, int* xshfttot, int* yshfttot, float* rottot, int n)
{
	int x3[ARR_LEN], y3[ARR_LEN];
	memcpy(x3, x2, sizeof(x3));
	memcpy(y3, y2, sizeof(y3));
	shft(x3, y3, xshft, yshft, n);
	rot(x3, y3, ang, n);
	int mnew = mismatch(x1, y1, x3, y3, n);
	if (mnew < *m)
	{
		memcpy(x2, x3, sizeof(x3));
		memcpy(y2, y3, sizeof(x3));
		*xshfttot += xshft;
		*yshfttot += yshft;
		*rottot += ang;
		
		*m = mnew;
	}
}
