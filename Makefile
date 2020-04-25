CC=gcc
CXX=g++
LD=g++
HEADERFILES= -I gyro -I rplidara1/rplidar_sdk/sdk/sdk/include -I rplidara1/rplidar_sdk/sdk/sdk/src
CXXFLAGS= -c -Wall $(HEADERFILES)
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
