#include "mbed.h"
#include "PS_PAD.hpp"
#include <cmath>
#include "Motor2.hpp"

/*PS2コントローラー*/
PS_PAD ps2(PA_7,PA_6,PA_5,PA_4);
int LX,LY,RX;
/*                */

/*制御用変数*/
float vx,vy,vf,vr,vl,rot;
/*         */

Motor2 motor [3] = 
{
    Motor2 (PA_11,PB_5,PA_8, false, 50),
    Motor2 (PF_0,PB_6,PB_1, false, 50),
    Motor2 (PB_7,PB_0,PA_12, false, 50)
};

PwmOut r(PA_9),g(PA_10),b(PB_4);

//led
int led_cnt=0;
int led_cnt_max=138*6;

float checker(float v){
    if(v>=0.97){
        return 0.97;
    } else if(v<=-0.97){
        return -0.97;
    }
    return v;
}

// main() runs in its own thread in the OS
int main()
{
    r.period_us(50);
    g.period_us(50);
    b.period_us(50);
    while (true) {

        ps2.poll();

        LX = ps2.read(PS_PAD::ANALOG_LX);
        LY = ps2.read(PS_PAD::ANALOG_LY);
        RX = ps2.read(PS_PAD::ANALOG_RX);
        /*   */

        if(ps2.read(PS_PAD::PAD_SQUARE)){
            b=0.0;
            r=0.5;
            b=0.0;
        }
        if(ps2.read(PS_PAD::PAD_CIRCLE)){
            b=0.0;
            r=0.0;
            b=0.5;
        }
        if(ps2.read(PS_PAD::PAD_X)){
            b=0.5;
            r=0.0;
            b=0.0;
        }

        //PS2-max:127
        //PWMOutの最大値を0.2ぐらいまでにする
        vx=static_cast<float>(LX)/(128*1.1);
        vy=static_cast<float>(LY)/(128*1.1);
        rot=static_cast<float>(RX)/(128*2);
        vf=-2.0/3.0*vx-rot;
        vl=1.0/3*vx+1/sqrt(3)*vy-rot;
        vr=1.0/3*vx-1/sqrt(3)*vy-rot;

        if(ps2.read(PS_PAD::IS_CONNECTED)){
            motor[0]=checker(vf);
            motor[1]=checker(vr);
            motor[2]=checker(vl);
        }else {
            motor[0]=0.0;
            motor[1]=0.0;
            motor[2]=0.0;
        }
        printf("LX:%d LY:%d Connected:%d P:%f\n",LX,LY,ps2.read(PS_PAD::IS_CONNECTED),vf);
    }
}

