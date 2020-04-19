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
#include <wiringPi.h>
#include <chrono>
#include <wiringPiI2C.h>
#include "mpu6050_driver.h"

#define Device_Address 0x68	/*Device Address/Identifier for MPU6050*/

#define PWR_MGMT_1   0x6B
#define SMPLRT_DIV   0x19
#define CONFIG       0x1A
#define GYRO_CONFIG  0x1B
#define INT_ENABLE   0x38
#define ACCEL_XOUT_H 0x3B
#define ACCEL_YOUT_H 0x3D
#define ACCEL_ZOUT_H 0x3F
#define GYRO_XOUT_H  0x43
#define GYRO_YOUT_H  0x45
#define GYRO_ZOUT_H  0x47

int fd;
int timeold=0;

void MPU6050_Init(){
	fd = wiringPiI2CSetup(Device_Address);
	wiringPiI2CWriteReg8 (fd, SMPLRT_DIV, 0x07);	/* Write to sample rate register */
	wiringPiI2CWriteReg8 (fd, PWR_MGMT_1, 0x01);	/* Write to power management register */
	wiringPiI2CWriteReg8 (fd, CONFIG, 0);		/* Write to Configuration register */
	wiringPiI2CWriteReg8 (fd, GYRO_CONFIG, 0);	/* Write to Gyro Configuration register */
	wiringPiI2CWriteReg8 (fd, INT_ENABLE, 0x01);	/*Write to interrupt enable register */

	} 
short read_raw_data(int addr){
	short high_byte,low_byte,value;
	high_byte = wiringPiI2CReadReg8(fd, addr);
	low_byte = wiringPiI2CReadReg8(fd, addr+1);
	value = (high_byte << 8) | low_byte;
	return value;
}

void ms_delay(int val){
	int i,j;
	for(i=0;i<=val;i++)
		for(j=0;j<1200;j++);
}

void readMPU6050(mpu6050reads *data){
    int msnow = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    int mschange;
    if (timeold!=0) {
        mschange = msnow-timeold;
        timeold=msnow;
    }
    else{
        mschange=0;
        timeold=msnow;
    }
    float secchange = (float)mschange/1000;
    //printf("old: %d, now: %d, mschg: %d, secchange: %f\n", timeold, msnow, mschange, secchange);
    data->Acc_x = read_raw_data(ACCEL_XOUT_H)/16384.0*secchange;
	data->Acc_y = read_raw_data(ACCEL_YOUT_H)/16384.0*secchange;
	data->Acc_z = read_raw_data(ACCEL_ZOUT_H)/16384.0*secchange;
	
	data->Gyro_x = read_raw_data(GYRO_XOUT_H)/131.0*secchange;
	data->Gyro_y = read_raw_data(GYRO_YOUT_H)/131.0*secchange;
	data->Gyro_z = read_raw_data(GYRO_ZOUT_H)/131.0*secchange;
}

#ifdef dddd
int main(){
    mpu6050reads mpu6050data;
	float Acc_x,Acc_y,Acc_z;
	float Gyro_x,Gyro_y,Gyro_z;
	float Ax=0, Ay=0, Az=0;
	float Gx=0, Gy=0, Gz=0;
	fd = wiringPiI2CSetup(Device_Address);   /*Initializes I2C with device Address*/
	MPU6050_Init();		                 /* Initializes MPU6050 */
	
	while(1)
	{
		/*Read raw value of Accelerometer and gyroscope from MPU6050*/
		/*Acc_x = read_raw_data(ACCEL_XOUT_H);
		Acc_y = read_raw_data(ACCEL_YOUT_H);
		Acc_z = read_raw_data(ACCEL_ZOUT_H);
		
		Gyro_x = read_raw_data(GYRO_XOUT_H);
		Gyro_y = read_raw_data(GYRO_YOUT_H);
		Gyro_z = read_raw_data(GYRO_ZOUT_H);*/
        readMPU6050(&mpu6050data);
        Acc_x = mpu6050data.Acc_x;
		Acc_y = mpu6050data.Acc_y;
		Acc_z = mpu6050data.Acc_z;
		
		Gyro_x = mpu6050data.Gyro_x;
		Gyro_y = mpu6050data.Gyro_y;
		Gyro_z = mpu6050data.Gyro_z;
		
		/* Divide raw value by sensitivity scale factor */
		Ax = Acc_x;
		Ay = Acc_y;
		Az = Acc_z;
		
		Gx = Gyro_x;
		Gy = Gyro_y;
		Gz = Gyro_z;
		
		printf("\n Gx=%.3f °/s\tGy=%.3f °/s\tGz=%.3f °/s\tAx=%.3f g\tAy=%.3f g\tAz=%.3f g\n",Gx,Gy,Gz,Ax,Ay,Az);
		delay(500);
		
	}
	return 0;
}
#endif
