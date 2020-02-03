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


#include "AdafruitStepperMotorHAT_CPP/Adafruit_MotorHAT.h"
#include "motors.h"

Adafruit_MotorHAT hat;

Adafruit_DCMotor& M1= hat.getDC(1);
Adafruit_DCMotor& M2= hat.getDC(2);
Adafruit_DCMotor& M3= hat.getDC(3);
Adafruit_DCMotor& M4= hat.getDC(4);

void setMotorDir(int *speed, Adafruit_DCMotor& Motor){
    if (*speed<0) {
        Motor.run(BACKWARD);
        *speed = -*speed;
    }
    else{
        Motor.run(FORWARD);
    }
}

void motorControl(int speeds[4]){

    setMotorDir(&speeds[0], M1);
    M1.setSpeed(speeds[0]);

    setMotorDir(&speeds[1], M2);
    M2.setSpeed(speeds[1]);

    setMotorDir(&speeds[2], M3);
    M3.setSpeed(speeds[2]);

    setMotorDir(&speeds[3], M4);
    M4.setSpeed(speeds[3]);
}

void Mhatreset(){
    hat.resetAll();
}
