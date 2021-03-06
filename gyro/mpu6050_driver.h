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

#ifndef _MPU6050_DRIVER_H
#define _MPU6050_DRIVER_H

struct mpu6050reads{
    float Acc_x=0;
    float Acc_y=0;
    float Acc_z=0;
    float Gyro_x=0;
    float Gyro_y=0;
    float Gyro_z=0;
};
class MPU6050Driver{
private:
    int timeold=0;

private:
    short read_raw_data(int addr);
    void ms_delay(int val);

public:
    void readMPU6050(mpu6050reads& data);
    int MPU6050_Init();
    void resetTimeOld();
};

#endif
