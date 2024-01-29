
// MODIFIE PAR I. VIN (21/11/2017)

/*
  AirQuality library v1.0
  2010 Copyright (c) Seeed Technology Inc.  All right reserved.

  Original Author: Bruce.Qin

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include"mbed.h"
#ifndef __AIRQUALITY_H__
#define __AIRQUALITY_H__
class AirQuality
{
public:
    int i ;
    long vol_standard;
    int init_voltage;
    int first_vol;
    int last_vol;
    int temp;
    int counter;
    bool timer_index;
    bool error;
    void init(PinName pin, PinName pin2);//void(*IRQ)(void));
    //int* 
    void slope(PinName pin, PinName pin2,int *tab);//void);
private:
    PinName _pin, _pin2;
    void avgVoltage(void);
};
#endif
