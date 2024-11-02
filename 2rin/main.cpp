#include "mbed.h"
#include "PS_PAD.hpp"
#include <cstdio>
#include "Motor.hpp"

Motor left(PA_12_ALT0,PB_0,false,50);
Motor right(PB_7,PB_6,false,50);

PS_PAD ps2(PA_7,PA_6,PA_5,PA_4);
// PwmOut p1(PA_12_ALT0);
// DigitalOut a1(PB_0);
// PwmOut p2(PB_7);
// DigitalOut a2(PB_6);
int LY,RY,LX,RX;
bool x;
bool pre1,pre2;
bool brake1=false,brake2=false;
bool sw=false;

enum mode{
  twoSticks,
  normal  
};


// main() runs in its own thread in the OS
int main()
{
    mode Mode=twoSticks;
    // p1.period_us(50);
    // p2.period_us(50);

    while (true) {
        ps2.poll();

        LY=ps2.read(PS_PAD::ANALOG_LY);
        RY=ps2.read(PS_PAD::ANALOG_RY);
        LX=ps2.read(PS_PAD::ANALOG_LX);
        RX=ps2.read(PS_PAD::ANALOG_RX);
        x=ps2.read(PS_PAD::PAD_X);

        printf("L:%f R:%f\n",static_cast<float>(LY)/(127*2),static_cast<float>(RY)/(127*2));
        if(ps2.read(PS_PAD::IS_CONNECTED)){
            switch(Mode){
                case twoSticks:
                    // if(LY>0){
                    //     // a2=0;
                    //     // p2=static_cast<float>(LY)/(128*1.1);
                    //     pre2=0;
                    // } else if(LY<0){
                    //     // a2=1;
                    //     // p2=abs(static_cast<float>(LY)/(128*1.1));
                    //     pre2=1;
                    // } else {
                    //         // a2=!pre1;
                    //         // p2=0.0;
                    // }

                    // if(RY>0){
                    //     // a1=0;
                    //     // p1=static_cast<float>(RY)/(128*1.1);
                    //     // pre1=0;
                    // } else if(RY<0){
                    //     // a1=1;
                    //     // p1=abs(static_cast<float>(RY)/(128*1.1));
                    //     // pre1=1;

                    // } else {
                    //         // a1=!pre2;
                    //         // p1=0.0;
                    // }
                    sw=ps2.read(PS_PAD::PAD_TRIANGLE);
                    if(sw){
                        left=static_cast<float>(LY)/(128*1.1);
                        right=static_cast<float>(RY)/(128*1.1);
                    } else {
                        left=static_cast<float>(RY)/(128*1.1);
                        right=static_cast<float>(LY)/(128*1.1);
                    }

                    if(x)Mode=normal;

                    break;
                case normal:
                    float ratio=0;

                    // if(LX>=0){
                    //     ratio=1-static_cast<float>(LX)/126;

                    //     if(LX+LY>=0){
                    //         // a1=0,a2=0;
                    //         // p2=static_cast<float>(LY)/(127*4)+static_cast<float>(LX)/(127*4);
                    //         // p1=static_cast<float>(LY)/(127*2)*ratio;
                    //     } else {
                    //         // a1=1,a2=1;
                    //         // p2=abs(static_cast<float>(LY)/(127*2)+static_cast<float>(LX)/(127*4));
                    //         // p1=abs(static_cast<float>(LY)/(127*2))*ratio;
                    //     }
                    // } else {
                    //     ratio=1-abs(static_cast<float>(LX)/127);

                    //     if(LY+LX>=0){
                    //         a1=0,a2=0;
                    //         p1=static_cast<float>(LY)/(127*4)+static_cast<float>(LX)/(127*4);
                    //         p2=static_cast<float>(LY)/(127*2)*ratio;
                    //     } else {
                    //         a1=1,a2=1;
                    //         p1=abs(static_cast<float>(LY)/(127*2)+static_cast<float>(LX)/(127*4));
                    //         p2=abs(static_cast<float>(LY)/(127*2))*ratio;
                    //     }
                    // }

                    
                    left=static_cast<float>(LY)/(128*2)-static_cast<float>(RX)/(128*4);
                    right=static_cast<float>(LY)/(128*2)+static_cast<float>(RX)/(128*4);

                    if(x)Mode=twoSticks;
                    break;
            }
        }
    }
}

