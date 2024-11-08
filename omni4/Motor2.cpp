/* mbed simple H-bridge motor controller
 * Copyright (c) 2007-2010, sford, http://mbed.org
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "Motor2.hpp"

#include "mbed.h"

Motor2::Motor2(PinName pwm, PinName rev1, PinName rev2, bool ccw, int _period_us):
        _pwm(pwm), _rev1(rev1), _rev2(rev2) {

    // Set initial condition of PWM
    _pwm.period_us(_period_us);
    _pwm = 0;

    // Initial condition of output enables
    _ccw = ccw;
    _rev1 = 0;
    _rev2 = 0;
    dir = 0;
    duty = 0.0;
}

void Motor2::speed(float speed) {
    
    if (speed == 0.0)
    {
        _rev1 = 1;
        _rev2 = 1;
        _pwm = 0.0;
    }

    else
    {
        dir = (speed < 0.f);
        _rev1 = (_ccw) ? !dir : dir;
        _rev2 = (_ccw) ? dir : !dir;
        _pwm = abs(speed);
    }
}

void Motor2::operator=(float _speed){
    this->speed(_speed);
}

void Motor2::operator=(double _speed){
    this->speed(static_cast<float>(_speed));
}
