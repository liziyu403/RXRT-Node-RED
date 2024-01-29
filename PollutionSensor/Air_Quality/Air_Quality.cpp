
// MODIFIE PAR I. VIN (21/11/2017)

//
// modified by mbed group for use with the mbed platform
// modification date 9/4/2014
//

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
#include"Air_Quality.h"

// Interrupt Handler Assignment
//Ticker IntHandler;

//Get the avg voltage in 5 minutes.
void AirQuality::avgVoltage()
{
    if(i==150) { //sum 5 minutes
        vol_standard=temp/150;
        temp=0;
        //printf("Vol_standard in 5 minutes: %d\n\r",vol_standard);
        i=0;
    } else {
        temp+=first_vol;
        i++;
    }

}

void AirQuality::init(PinName pin, PinName pin2)//void(*IRQ)(void))
{
    _pin=pin;
    _pin2=pin2;
    AnalogIn sensor(_pin);
    AnalogIn sensor2(_pin2);
    unsigned char i = 0;
    //printf("Air Quality Sensor Starting Up...(20s)\n\r");
    wait(20); //20s
    init_voltage = sensor.read() * 1000; // boost the value to be on a 0 -> 1000 scale for compatibility
    //printf("The initial voltage is %d%% of source \n\r",init_voltage/10);
    while(init_voltage) {
        if((init_voltage < 798) && (init_voltage > 10)) {
            // the init voltage is ok
            first_vol = sensor.read() * 1000;//initialize first value
            last_vol = sensor2.read() * 1000;//first_vol;
            vol_standard = last_vol;
            //printf("Sensor ready.\n\r");
            error = false;
            break;
        } else if(init_voltage > 798 || init_voltage <= 10) {
            // The sensor is not ready, wait a bit for it to cool off
            i++;
            //printf("Sensor not ready (%d), try %d/5, waiting 60 seconds...\n\r",init_voltage,i);
            wait(60);//60s
            init_voltage = sensor.read() * 1000;
            if(i==5) {
                // After 5 minutes warn user that the sensor may be broken
                i = 0;
                error = true;
                //printf("Sensor Error! You may have a bad sensor. :-(\n\r");
            }
        } else
            break;
    }
    // Call AirQualityInterrupt every 2seconds
    //IntHandler.attach(IRQ, 2.0);      // commenté par Isa
    //printf("Test begin...\n\r");
}

void AirQuality::slope(PinName pin, PinName pin2,int *tab)//void)       // les mesures seront retournés dans le tableau "tab"
{
    // rajouts Isa
    _pin=pin;
    _pin2=pin2;
    AnalogIn sensor(_pin);
    AnalogIn sensor2(_pin2);
    //int* tab[3];
    // fin rajouts Isa

    timer_index=true;   //rajout %Isa
    while(timer_index) {
        // rajouté par Isa :
        first_vol = sensor.read() * 1000;//initialize first value
        last_vol = sensor2.read() * 1000;//first_vol;
        vol_standard = last_vol;
        // fin rajouts Isa

        //printf("first_vol-last_vol=%d\n\r",first_vol-last_vol);
        //printf("first_vol-vol_standard=%d\n\r",first_vol-vol_standard);
        //printf("first_vol=%d\n\r",first_vol);
        //printf("last_vol=%d\n\r",last_vol);

        if(first_vol-last_vol > 400 || first_vol > 700) {
            //printf("High pollution! Force signal active.\n\r");
            timer_index = 0;
            avgVoltage();
            tab[0]=0;
            //return 0;
        } else if((first_vol - last_vol > 400 && first_vol < 700) || first_vol - vol_standard > 150) {
            //printf("sensor_value:%d",first_vol);
            //printf("\t High pollution!\n\r");
            timer_index = 0;
            avgVoltage();
            tab[0]=1;
            //return 1;

        } else if((first_vol-last_vol > 200 && first_vol < 700) || first_vol - vol_standard > 50) {
            //printf("sensor_value:%d",first_vol);
            //printf("\t Low pollution!\n\r");
            timer_index = 0;
            avgVoltage();
            tab[0]=2;
            //return 2;
        } else {
            avgVoltage();
            //printf("sensor_value:%d",first_vol);
            //printf("\t Air fresh\n\r");
            timer_index = 0;
            tab[0]=3;
            //return 3;
        }

    }
    tab[1]= first_vol;
    tab[2]= last_vol;
    //return(*tab);
    //return -1;
}
