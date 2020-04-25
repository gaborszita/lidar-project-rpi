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


#define PI 3.14159265
#define NUM_ENTRIES 360
#define beta 82*PI/180
//#define b (-25)
#define phi_corr (-8)
#define mism_noise (0)
#define ldata_save 20
//#define ldata_count 





void sync_with_lidar();
//int uartread();
unsigned int calcchecksum(unsigned int data[]);
//void sync_with_lidar();
//int uartread();
unsigned int calcchecksum(unsigned int data[]);
void ctrl_c_handler(int s);
//void countXY(int LXnew[], int LYnew[], int LXold[], int LYold[], int *x, int *y, int *heading);
