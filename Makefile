# lidar project
# 
# Copyright (C) 2017-2020 Gabor Szita
# 
# This file is part of lidar project.
#
# Lidar project is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 3 as published by
# the Free Software Foundation.
#
# Lidar project is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with lidar project.  If not, see https://www.gnu.org/licenses/ 

CC=gcc
CXX=g++
LD=g++
HEADERFILES= -I gyro -I rplidara1/rplidar_sdk/sdk/sdk/include -I rplidara1/rplidar_sdk/sdk/sdk/src
CXXFLAGS= -c -Wall $(HEADERFILES) -g
LDFLAGS= -lwiringPi -lpthread -lcrypt -lrt -Wall -std=c++11
OBJS=countXY.o tcp.o mainprogram.o motors.o lidardriver.o mpu6050_driver.o mousedriver.o navigation.o
STATICOBJS=AdafruitStepperMotorHAT_CPP/AdafruitDCMotorHAT.a rplidara1/rplidar_sdk/sdk/output/Linux/Release/librplidar_sdk.a
RM=rm -f
TARGET=main

$(TARGET): $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(STATICOBJS) $(LDFLAGS)

countXY.o: countXY.cpp countXY.h
	$(CXX) $(CXXFLAGS) countXY.cpp

tcp.o: tcp.cpp tcp.h
	$(CXX) $(CXXFLAGS) tcp.cpp

mainprogram.o: mainprogram.cpp mainprogram.h
	$(CXX) $(CXXFLAGS) mainprogram.cpp 

motors.o: motors.cpp motors.h
	$(CXX) $(CXXFLAGS) motors.cpp

lidardriver.o: rplidara1/rplidar_driver/lidardriver.cpp rplidara1/rplidar_driver/lidardriver.h
	$(CXX) $(CXXFLAGS) rplidara1/rplidar_driver/lidardriver.cpp

mpu6050_driver.o: gyro/mpu6050_driver.c gyro/mpu6050_driver.h
	$(CXX) $(CXXFLAGS) gyro/mpu6050_driver.c

mousedriver.o: mouse/mousedriver.cpp mouse/mousedriver.h
	$(CXX) $(CXXFLAGS) mouse/mousedriver.cpp

navigation.o: navigation.cpp navigation.h
	$(CXX) $(CXXFLAGS) navigation.cpp

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) $(TARGET)
