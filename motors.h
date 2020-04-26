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

#ifndef _MOTORS_H
#define _MOTORS_H

//void motorControl(int speeds[4]);
//void Mhatreset();
//bool isRobotMoving();
#include "AdafruitStepperMotorHAT_CPP/Adafruit_MotorHAT.h"

class AdafruitMotorsDriver{
    private: 
        Adafruit_MotorHAT *hat;
        Adafruit_DCMotor* M1;
        Adafruit_DCMotor* M2;
        Adafruit_DCMotor* M3;
        Adafruit_DCMotor* M4;

    public:
        bool robotMoving=false;

    private:
        void setMotorDir(int *speed, Adafruit_DCMotor& Motor);

    public:
        AdafruitMotorsDriver();
        ~AdafruitMotorsDriver();
        void motorControl(int speeds[4]);
        void Mhatreset();
};

#endif
