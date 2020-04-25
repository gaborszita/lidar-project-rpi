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

#ifndef _MOUSEDRIVER_H
#define _MOUSEDRIVER_H

#define MOUSE_ALWAYS1_BIT_NOT1 5
#define mousedpcm 441  //dots per centimeter(dpcm) NOT dots per inch(DPI), convert DPI to dpcm by multiplying DPI by 0.393

class MouseDriver{
    private:
        int *x=0; //global mouse x and y change
        int *y=0; 
        float *heading;
        bool *isMoving;

    public:
        int mousethread_run(float *heading, int *x, int *y, bool *isMoving);

    private:
        class MouseThreadingControl
        {
            public:
            MouseThreadingControl(MouseDriver *md);
            static int mousethr(MouseDriver *mobject);
        };
};

#endif
