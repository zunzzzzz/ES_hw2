#include "mbed.h"
#include <iostream>
#include <cmath> 
#include <string>
Serial pc(USBTX, USBRX);
AnalogOut Aout(DAC0_OUT);
AnalogIn Ain(A0);
DigitalIn  Switch(SW3);
DigitalOut greenLED(LED1);
DigitalOut redLED(LED2);

int i;
float count_ = 0;
int sample = 500;
float ADCdata[500];
float freq = 0;
// float ADCdata;
BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);
char table[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
char dot[10] = {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xEF};

int main(){
    for(i = 0; i < sample; i++) {
        Aout = Ain;
        ADCdata[i] = Ain;
        wait(1./sample);
    }
    for(i = 0; i < sample; i++) {
        pc.printf("%1.3f\r\n", ADCdata[i]);
        if(i != 0) {
            if(ADCdata[i-1] < 0.3 && ADCdata[i] >= 0.3) count_ = count_ + 1; 
        }
    }
    freq = round(count_);
    int tmp = (int) freq;
    // pc.printf("%2.2f\r\n", freq);
    // pc.printf("%d\r\n", tmp);
    int x = 0;
    if((int)freq >= 100) x = 100;
    else x = 10;
    while(1) {
        if(Switch == 0) {
            if(x == 1) {
                display = dot[tmp];
                tmp = (int) freq;
                if((int)freq >= 100) x = 100;
                else x = 10;
            }
            else {
                display = table[tmp/x];
                tmp %= x;
                x /= 10;
            }
            // wait(1);
            redLED = 1;
            greenLED = 0;
        }
        else if(Switch == 1) {
            display = 0x00;
            redLED = 0;
            greenLED = 1;
            // wait(1);
        }


        // for(float j = 0; j < 2; j += 0.001) {
        //     Aout = 0.5 + 0.5 * sin(j * 3.141592 * freq);
        //     wait(0.001);
        // }
        for(float j = 0; j < freq; j +=  0.0005 / (1 / freq)) {
            Aout = 0.5 + 0.5 * sin(j * 3.141592);
            wait(0.0005);
        }
    }
}