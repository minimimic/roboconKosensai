#include "mbed.h"
#include "PS_PAD.hpp"
#include "cmath"
#include "Motor2.hpp"

///*------------Encoder------------*/
////TIM2
//TIM_HandleTypeDef htim2;
//int pinA2 = GPIO_PIN_0;//使うピンの数字
//int pinB2 = GPIO_PIN_1;//使うピンの数字
//GPIO_TypeDef *type2 = GPIOA;//使うピンのアルファベット
////TIM3
//TIM_HandleTypeDef htim3;
//int pinA3 = GPIO_PIN_6;//使うピンの数字
//int pinB3 = GPIO_PIN_7;//使うピンの数字
//GPIO_TypeDef *type3 = GPIOC;//使うピンのアルファベット
////TIM4
//TIM_HandleTypeDef htim4;
//int pinA4 = GPIO_PIN_6;//使うピンの数字
//int pinB4 = GPIO_PIN_7;//使うピンの数字
//GPIO_TypeDef *type4 = GPIOB;//使うピンのアルファベット
///*------------Encoder END------------*/
//
//static void encoderInit(TIM_HandleTypeDef *htim,TIM_TypeDef *TIMx);
//void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *htim);
//void printTIM(TIM_HandleTypeDef);
//void checkOverflow(int previous,int now,int& overflowCNT);
//void setTIM(TIM_HandleTypeDef tim,int& pre,int& TIMoverflow,int& intTIM);
//void Einit(TIM_HandleTypeDef* htim,TIM_TypeDef* tim);

PS_PAD ps2(PA_7,PA_6,PB_3,PA_15);
float LX,LY,RX,RY;
bool L1,L2,R1,R2;
bool circle,cross,triangle,square;
bool up,down,right,left;
bool select,start;
float vlf,vlb,vrf,vrb,vx,vy,rot;
//DigitalOut motor1a(PC_0);
//DigitalOut motor1b(PC_1);
//PwmOut motor1p(PB_0);
//DigitalOut motor2a(PB_4);
//DigitalOut motor2b(PB_5);
//PwmOut motor2p(PA_10);
//DigitalOut motor3a(PB_13);
//DigitalOut motor3b(PB_15);
//PwmOut motor3p(PB_14_ALT1);
//DigitalOut motor4a(PB_2);
//DigitalOut motor4b(PB_12);
//PwmOut motor4p(PA_11);

// Motor2 (PB_0, PC_0, PC_1, false, 50),
// Motor2 (PA_10, PB_4, PB_5, false, 50),
// Motor2 (PB_14_ALT1, PB_13, PB_15, false, 50),
// Motor2 (PA_11, PB_2, PB_12, false, 50)

Motor2 motor [4] = 
{
    Motor2 (PA_11, PB_2, PB_12, true, 50),
    Motor2 (PB_14_ALT1, PB_13, PB_15, true, 50),
        Motor2 (PA_10, PB_4, PB_5, true, 50),
    
    Motor2 (PB_0, PC_0, PC_1, true, 50)
    

};

int main()
{
    //ps2.init();
    //Einit(&htim2,TIM2);
    //Einit(&htim3,TIM3);
    //Einit(&htim4,TIM4);
    //int cntTIM2=0,previousTIM2=0,TIM2overflow=0;
    //int cntTIM3=0,previousTIM3=0,TIM3overflow=0;
    //int cntTIM4=0,previousTIM4=0,TIM4overflow=0;

    //エンコーダーの値  
    int intTIM2,intTIM3,intTIM4;

    //コントローラ
    int LX=0,LY=0,RX=0;

    while(1){
        //setTIM(htim2,previousTIM2,TIM2overflow,intTIM2);
        //setTIM(htim3,previousTIM3,TIM3overflow,intTIM3);
        //setTIM(htim4,previousTIM4,TIM4overflow,intTIM4);

        ps2.poll();
        //-128~127
        LX = ps2.read(PS_PAD::ANALOG_LX);
        LY = ps2.read(PS_PAD::ANALOG_LY);
        RX = ps2.read(PS_PAD::ANALOG_RX);

        circle = ps2.read(PS_PAD::PAD_CIRCLE);
        cross = ps2.read(PS_PAD::PAD_X);
        triangle = ps2.read(PS_PAD::PAD_TRIANGLE);
        square = ps2.read(PS_PAD::PAD_SQUARE);

//127*1に戻す
        vx=static_cast<float>(LY)/(127*1);
        vy=static_cast<float>(LX)/(127*1);
        rot=static_cast<float>(RX)/(127*2);

        vlf=vx/(sqrt(2)*2)+vy/(sqrt(2)*2);//ヒダリウエ
        vlb=vx/(sqrt(2)*2)-vy/(sqrt(2)*2);//ヒダリシタ
        vrf=vx/(sqrt(2)*2)-vy/(sqrt(2)*2);//右上
        vrb=vx/(sqrt(2)*2)+vy/(sqrt(2)*2);//右

        if(ps2.read(PS_PAD::IS_CONNECTED)){
            motor[0]=vrf-rot;
            motor[1]=vlf+rot;
            motor[2]=vlb+rot;
            motor[3]=vrb-rot;
        } 
        else {
            motor[0]=0.0;
            motor[1]=0.0;
            motor[2]=0.0;
            motor[3]=0.0;
        }

        if(ps2.read(PS_PAD::PAD_SQUARE)){
            motor[0]=0.3;
        }
        if(ps2.read(PS_PAD::PAD_X)){
            motor[1]=0.3;
        }
        if(ps2.read(PS_PAD::PAD_TRIANGLE)){
            motor[2]=0.3;
        }
        if(ps2.read(PS_PAD::PAD_CIRCLE)){
            motor[3]=0.3;
        }

        wait_us(1000);

        printf("LX:%d LY:%f Connected:%d\n",LX,vrb,ps2.read(PS_PAD::IS_CONNECTED));
    }
}