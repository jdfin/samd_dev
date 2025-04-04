#pragma once

#include <Arduino.h>

class SamdTc {

    public:

        SamdTc(Tc *tc) : _tc(tc)
        {
            if (tc == TC3)
                _tc_num = 3;
            else if (tc == TC4)
                _tc_num = 4;
            else if (tc == TC5)
                _tc_num = 5;
            else
                _tc_num = -1;
        }

        ~SamdTc()
        {
            _tc = nullptr;
        }

        void show(int verbosity=0);

        void sync()
        {
            while (_tc->COUNT8.STATUS.bit.SYNCBUSY != 0)
                ;
        }

        void reset();

        void set_ctrla(uint16_t value)
        {
            // COUNT8.CTRLA, COUNT16, and COUNT32 are all the same
            _tc->COUNT8.CTRLA.reg = value;
            sync();
        }

        uint32_t count32()
        {
            return _tc->COUNT32.COUNT.reg;
        }

    private:
        
        Tc *_tc;

        int _tc_num;

}; // class SamdTc
