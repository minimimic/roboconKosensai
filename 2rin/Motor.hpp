#pragma once
#include "mbed.h"

/** Interface to control a standard DC motor
 *
 * with an H-bridge using a PwmOut and 2 DigitalOuts
 */
class Motor {
public:

    /** Create a motor control interface
     *
     * @param pwm A PwmOut pin, driving the H-bridge enable line to control the speed
     * @param rev A DigitalOut, set high when the motor should go backwards
     */
    Motor(PinName pwm, PinName rev, bool ccw=false, int period_us=500);

    /** Set the speed of the motor
     *
     * @param speed The speed of the motor as a normalised value between -1.0 and 1.0
     */
    void speed(float speed);
    void operator=(float speed);
    void operator=(double speed);
    bool dir;
    float duty;

protected:
    bool _ccw;
    PwmOut _pwm;
    DigitalOut _rev;
    bool jmin;
    float maespeed;
};
