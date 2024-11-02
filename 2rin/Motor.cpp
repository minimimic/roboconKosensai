#include "Motor.hpp"
#include "mbed.h"

Motor::Motor(PinName pwm, PinName rev, bool ccw, int _period_us):
        _pwm(pwm), _rev(rev) {

    // Set initial condition of PWM
    _pwm.period_us(_period_us);
    _pwm = 0;

    // Initial condition of output enables
    _ccw = ccw;
    _rev = 0;
    dir = 0;
    duty = 0.0;
}

void Motor::speed(float speed) {
    if(maespeed!=0.0&&speed==0.0){
        dir=!jmin;
    }
    else{
        dir = (speed < 0.f);
    }
    jmin=dir;
    maespeed=speed;
    _rev = (_ccw) ? !dir : dir;
    _pwm = abs(speed);
}

void Motor::operator=(float _speed){
    this->speed(_speed);
}

void Motor::operator=(double _speed){
    this->speed(static_cast<float>(_speed));
}
