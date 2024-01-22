/*
Copyright (c) 2011 Anthony Buckton (abuckton [at] blackink [dot} net {dot} au)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <mbed.h>
#include <string>
#include <list>

#include <mpr121.h>

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

// Create the interrupt receiver object on pin 26
InterruptIn interrupt(p26);

// Setup the Serial to the PC for debugging
Serial pc(USBTX, USBRX);

// Setup the i2c bus on pins 28 and 27
I2C i2c(p9, p10);

// Setup the Mpr121:
// constructor(i2c object, i2c address of the mpr121)
Mpr121 mpr121(&i2c, Mpr121::ADD_VSS);

void fallInterrupt() {
    int key_code=0;
    int i=0;
    int value=mpr121.read(0x00);
    value +=mpr121.read(0x01)<<8;
    // LED demo mod by J. Hamblen
    //pc.printf("MPR value: %x \r\n", value);
    i=0;
    // puts key number out to LEDs for demo
    for (i=0; i<12; i++) {
        if (((value>>i)&0x01)==1) key_code=i+1;
        }
    led4=key_code & 0x01;
    led3=(key_code>>1) & 0x01;
    led2=(key_code>>2) & 0x01;
    led1=(key_code>>3) & 0x01;
}
int main() {

    pc.printf("\nHello from the mbed & mpr121\n\r");

    unsigned char dataArray[2];
    int key;
    int count = 0;

    pc.printf("Test 1: read a value: \r\n");
    dataArray[0] = mpr121.read(AFE_CFG);
    pc.printf("Read value=%x\r\n\n",dataArray[0]);

    pc.printf("Test 2: read a value: \r\n");
    dataArray[0] = mpr121.read(0x5d);
    pc.printf("Read value=%x\r\n\n",dataArray[0]);

    pc.printf("Test 3: write & read a value: \r\n");
    mpr121.read(ELE0_T);
    mpr121.write(ELE0_T,0x22);
    dataArray[0] = mpr121.read(ELE0_T);
    pc.printf("Read value=%x\r\n\n",dataArray[0]);

    pc.printf("Test 4: Write many values: \r\n");
    unsigned char data[] = {0x1,0x3,0x5,0x9,0x15,0x25,0x41};
    mpr121.writeMany(0x42,data,7);

    // Now read them back ..
    key = 0x42;
    count = 0;
    while (count < 7) {
        char result = mpr121.read(key);
        key++;
        count++;
        pc.printf("Read value: '%x'=%x\n\r",key,result);
    }

    pc.printf("Test 5: Read Electrodes:\r\n");
    key = ELE0_T;
    count = 0;
    while (count < 24) {
        char result = mpr121.read(key);
        pc.printf("Read key:%x value:%x\n\r",key,result);
        key++;
        count++;
    }
    pc.printf("--------- \r\n\n");

    // mpr121.setProximityMode(true);

    pc.printf("ELE_CFG=%x", mpr121.read(ELE_CFG));

    interrupt.fall(&fallInterrupt);
    interrupt.mode(PullUp);

    while (1) {
        wait(5);
        pc.printf(".");
    }
}



