CC=gcc
CXX=g++
LD=g++
CXXFLAGS= -c -Wall 
LDFLAGS= -lwiringPi -lpthread -lcrypt -lrt -Wall -std=c++11
OBJS=countXY.o tcp.o mainprogram.o motors.o lidardriver.o mpu6050_driver.o
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
	$(CXX) $(CXXFLAGS) -I gyro mainprogram.cpp

motors.o: motors.cpp motors.h
	$(CXX) $(CXXFLAGS) motors.cpp

lidardriver.o: rplidara1/rplidar_driver/lidardriver.cpp rplidara1/rplidar_driver/lidardriver.h
	$(CXX) $(CXXFLAGS) -I rplidara1/rplidar_sdk/sdk/sdk/include -I rplidara1/rplidar_sdk/sdk/sdk/src rplidara1/rplidar_driver/lidardriver.cpp

mpu6050_driver.o: gyro/mpu6050_driver.c gyro/mpu6050_driver.h
	$(CXX) $(CXXFLAGS) gyro/mpu6050_driver.c

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) $(TARGET)
