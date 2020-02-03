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


#define ARR_LEN 360

int pointpairs(int LXold[], int LYold[], int corners1[], int corners1_len, int LXnew[], int LYnew[], int corners2[], int corners2_len, int pairs[ARR_LEN][2], const int maxlength);
int split_merge(int Lszog[], int LX[], int LY[], int corners[]);
int arcprocess(int LX[], int LY[], int arcgroups[ARR_LEN][2], int corner[], int lencorner, int groupindex);
int arc(int Lszog[], int LX[], int LY[], int validarc[ARR_LEN][2]);
void testmove(int x1[], int y1[], int x2[], int y2[], int xshft, int yshft, float ang, int* m, int* xshfttot, int* yshfttot, float* rottot, int n);
int mismatch(int x1[], int y1[], int x2[], int y2[], int n);
void countXY(int LXold[ARR_LEN], int LYold[ARR_LEN], int Lold[ARR_LEN], int LXnew[ARR_LEN], int LYnew[ARR_LEN], int Lnew[ARR_LEN], int* currentX, int* currentY, float* currentDEG, int socket, char buf[], char* bufptr);


