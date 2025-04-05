#pragma once

#include <Arduino.h>


// I could not get this to work as a template class with TC3, TC4, or TC5 as
// the template parameter. TC3 etc. are #defines like this:
//     #define TC3 ((Tc*)0x42002C00UL)
// I could not get past failures to reinterpret_cast from integer to pointer.


class SamdTc3 {

    public:

        static void show(int verbosity=0);

        static void sync()
        {
            while (TC3->COUNT8.STATUS.bit.SYNCBUSY != 0)
                ;
        }

        static void reset()
        {
            TC3->COUNT8.CTRLA.bit.SWRST = 1;
            sync();
            while (TC3->COUNT8.CTRLA.bit.SWRST != 0)
                ;
        }

        static void set_ctrla(uint16_t value)
        {
            // COUNT8.CTRLA, COUNT16, and COUNT32 are all the same
            TC3->COUNT8.CTRLA.reg = value;
            sync();
        }

        static uint32_t count32()
        {
            return TC3->COUNT32.COUNT.reg;
        }

}; // class SamdTc3


class SamdTc4 {

    public:

        static void show(int verbosity=0);

        static void sync()
        {
            while (TC4->COUNT8.STATUS.bit.SYNCBUSY != 0)
                ;
        }

        static void reset()
        {
            TC4->COUNT8.CTRLA.bit.SWRST = 1;
            sync();
            while (TC4->COUNT8.CTRLA.bit.SWRST != 0)
                ;
        }

        static void set_ctrla(uint16_t value)
        {
            // COUNT8.CTRLA, COUNT16, and COUNT32 are all the same
            TC4->COUNT8.CTRLA.reg = value;
            sync();
        }

        static uint32_t count32()
        {
            return TC4->COUNT32.COUNT.reg;
        }

}; // class SamdTc4


class SamdTc5 {

    public:

        static void show(int verbosity=0);

        static void sync()
        {
            while (TC5->COUNT8.STATUS.bit.SYNCBUSY != 0)
                ;
        }

        static void reset()
        {
            TC5->COUNT8.CTRLA.bit.SWRST = 1;
            sync();
            while (TC5->COUNT8.CTRLA.bit.SWRST != 0)
                ;
        }

        static void set_ctrla(uint16_t value)
        {
            // COUNT8.CTRLA, COUNT16, and COUNT32 are all the same
            TC5->COUNT8.CTRLA.reg = value;
            sync();
        }

        static uint32_t count32()
        {
            return TC5->COUNT32.COUNT.reg;
        }

}; // class SamdTc5
