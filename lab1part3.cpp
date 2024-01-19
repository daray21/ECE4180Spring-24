#include "mbed.h"
#include "PinDetect.h"
//class for 3 PWM color values for RGBLED
class LEDColor
{
public:
    LEDColor(float r, float g, float b);
    float red;
    float green;
    float blue;
};
LEDColor:: LEDColor(float r, float g, float b)
    : red(r), green(g), blue(b)
{
}
//Operator overload to adjust brightness with no color change
LEDColor operator * (const LEDColor& x, const float& b)
{
    return LEDColor(x.red*b,x.green*b,x.blue*b);
}
//Operator overload to add colors
LEDColor operator + (const LEDColor& x, const LEDColor& y)
{
    return LEDColor(x.red+y.red,x.green+y.green,x.blue+y.blue);
}

//Class to control an RGB LED using three PWM pins
class RGBLed
{
public:
    RGBLed(PinName redpin, PinName greenpin, PinName bluepin);
    void write(float red,float green, float blue);
    void write(LEDColor c);
    void writeRed(float red);
    void writeBlue(float blue);
    void writeGreen(float green);
    RGBLed operator = (LEDColor c) {
        write(c);
        return *this;
    };
private:
    PwmOut _redpin;
    PwmOut _greenpin;
    PwmOut _bluepin;
};

RGBLed::RGBLed (PinName redpin, PinName greenpin, PinName bluepin)
    : _redpin(redpin), _greenpin(greenpin), _bluepin(bluepin)
{
    //50Hz PWM clock default a bit too low, go to 2000Hz (less flicker)
    _redpin.period(0.0005);
}

void RGBLed::write(float red,float green, float blue)
{
    _redpin = red;
    _greenpin = green;
    _bluepin = blue;
}
void RGBLed::write(LEDColor c)
{
    _redpin = c.red;
    _greenpin = c.green;
    _bluepin = c.blue;
}
void RGBLed::writeRed(float red)
{
    _redpin = red;
}
void RGBLed::writeGreen(float green)
{
    _greenpin = green;
}
void RGBLed::writeBlue(float blue)
{
    _bluepin = blue;
}


//classes could be moved to include file


//Setup RGB led using PWM pins and class
RGBLed myRGBled(p23,p22,p21); //RGB PWM pins
DigitalIn switchInputRed(p13);
DigitalIn switchInputBlue(p14);
DigitalIn switchInputGreen(p15);
PinDetect pb1(p16);
PinDetect pb2(p17);
//setup some color objects in flash using const's
// const LEDColor red(1.0,0.0,0.0);
// const LEDColor green(0.0,0.2,0.0);
// //brighter green LED is scaled down to same as red and
// //blue LED outputs on Sparkfun RGBLED
// const LEDColor blue(0.0,0.0,1.0);
// const LEDColor yellow(1.0,0.2,0.0);
// const LEDColor white(1.0,0.2,1.0);
bool redOn = false;
bool greenOn = false;
bool blueOn = false;
float brightness = 1.0f;
int oldpb1 = 0;
int oldpb2 = 0;
int main()
{
    wait(.01);
    while(1) {
        switchInputRed.mode(PullUp);
        switchInputGreen.mode(PullUp);
        switchInputBlue.mode(PullUp);
        pb1.mode(PullUp);
        pb2.mode(PullUp);
        redOn = !switchInputRed;
        greenOn = !switchInputGreen;
        blueOn = !switchInputBlue;

        if ((oldpb1 == 1) && (pb1 == 0) && brightness < 1.0f) {
            brightness += 0.1f;
        }
        if ((oldpb2 == 1) && (pb2 == 0) && brightness > 0.0f) {
            brightness -= 0.1f;
        }
        if (redOn) {
            myRGBled.writeRed(brightness);
        }
        else {
            myRGBled.writeRed(0);
        }
        if (greenOn) {
            myRGBled.writeGreen(brightness);
        }
        else {
            myRGBled.writeGreen(0);
        }
        if (blueOn) {
            myRGBled.writeBlue(brightness);
        }
        else {
            myRGBled.writeBlue(0);
        }
        oldpb1 = pb1;
        oldpb2 = pb2;
        wait(.1);
    }
}
