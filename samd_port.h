#pragma once

#include <Arduino.h>


class SamdPort {

    public:

        // input (no pull):
        //   SamdPort gpio(PIN);
        //
        // output (with readback):
        //   SamdPort gpio(PIN, out=true);
        //
        // input (pull up or down):
        //   SamdPort gpio(PIN, out=false, in=true, pull=true);
        //   gpio.write(<pull>); // pull=0 for down, pull=1 for up
        //
        // output (no readback):
        //   SamdPort gpio(PIN, out=true, in=false);
        //
        // output (pulled up or down):
        //   SamdPort gpio(PIN, out=true, in=false, pull=true);
        //   gpio.write(<pull>); // pull=0 for pull down, pull=1 for pull up

        SamdPort(int ard_pin, bool out=false, bool in=true,
                 bool pull=false, bool drvstr=false)
        {
            EPortType port = g_APinDescription[ard_pin].ulPort;
            _pin = g_APinDescription[ard_pin].ulPin;
            _msk = 1UL << _pin;
        
            PORT->Group[port].PINCFG[_pin].reg = (pull ? PORT_PINCFG_PULLEN : 0)
                                               | (in ? PORT_PINCFG_INEN : 0)
                                               | (drvstr ? PORT_PINCFG_DRVSTR : 0);
            if (out)
                PORT->Group[port].DIRSET.reg = _msk;
            else
                PORT->Group[port].DIRCLR.reg = _msk;

            _set = &(PORT->Group[port].OUTSET.reg);
            _clr = &(PORT->Group[port].OUTCLR.reg);
            _in = &(PORT->Group[port].IN.reg);
        }

        void set() { *_set = _msk; }

        void clear() { *_clr = _msk; }

        volatile uint32_t *set_ptr() { return _set; }

        volatile uint32_t *clear_ptr() { return _clr; }

        uint32_t mask() { return _msk; }

        void write(int d)
        {
            if (d != 0)
                *_set = _msk;
            else
                *_clr = _msk;
        }

        int read() { return (*_in >> _pin) & 1; }

    private:

        uint32_t _pin;

        uint32_t _msk;

        volatile uint32_t *_set;

        volatile uint32_t *_clr;

        volatile uint32_t *_in;

}; // class SamdPort
