
#include "mbed.h"
#include "DebounceIn.h"
//PwmOut led(LED1); //setup PWM output
PwmOut led(p21);
DebounceIn pb1(p5);
DebounceIn pb2(p6);
int main() {
    pb1.mode(PullUp);
    pb2.mode(PullUp);
    float dimNum = 0.5f;
    int newpb1;
    int oldpb1 = 0;
    int newpb2;
    int oldpb2 = 0;
    wait(0.001);
    while(1) {
        newpb1 = pb1;
        newpb2 = pb2;
        if ((oldpb1 == 0) && (newpb1 == 1)) {
            if (dimNum < 1.0f) {
                dimNum +=0.1;
            }
        }
        if ((oldpb2 == 0) && (newpb2 == 1)) {
           if (dimNum > 0) {
                dimNum -=0.1;
            } 
        }

        led = dimNum;
        oldpb1 = newpb1;
        oldpb2 = newpb2;
       // wait(0.1);
    }
}