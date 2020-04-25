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

#ifndef _LIDARDRIVER_H
#define _LIDARDRIVER_H

#ifndef  _STDIO_H
#include <stdio.h>
#endif
#include "rplidar.h"
using namespace rp::standalone::rplidar; 

class LidarController{
private:
    
    RPlidarDriver* lidar;
    rplidar_response_measurement_node_hq_t nodes[360];
    size_t numnodes = sizeof(nodes)/sizeof(rplidar_response_measurement_node_hq_t);
    std::vector<RplidarScanMode> scanModes;

public:
    int setupLidar();
    int closeLidar();
    float getLidarFreq();
    int getLidarScan(int vangles[360]);

};

#endif
