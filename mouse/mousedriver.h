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

#define MOUSE_ALWAYS1_BIT_NOT1 5

class MouseDriver{
    private:
        int mousex=0; //global mouse x and y change
        int mousey=0; 
        float **heading;

    public:
        void get_mouse_readings(int *mx, int *my);
        int mousethread_run(float *heading);

    private:
        class MouseThreadingControl
        {
            public:
            MouseThreadingControl(float *heading, MouseDriver *md);
            static int mousethr(float *heading, MouseDriver *mobject);
        };
};

